
#include "loader.h"
#include "Flash.h"          // External flash driver (FM25W32)
#include "V94XX_FLASH.h"    // Internal flash driver
#include <string.h>
#include <stdlib.h>

/*******************************************************************************
 * FLASH SIZES
 ******************************************************************************/
// Internal flash: page size = sector size = 512 bytes (V94XX architecture)
#define INTERNAL_FLASH_PAGE_SIZE    512     // V94XX: 512 bytes per page/sector
#define INTERNAL_FLASH_SECTOR_SIZE  512     // Same as page size for internal flash

#define EXTERNAL_FLASH_PAGE_SIZE    256     // FM25W32: 256 bytes per page
#define EXTERNAL_FLASH_SECTOR_SIZE  4096    // FM25W32: 4KB per sector

/*******************************************************************************
 * PRIVATE DEFINITIONS
 ******************************************************************************/
#define MODULE_MAGIC    0x4D4D  // "MM"

/*******************************************************************************
 * PRIVATE TYPES
 ******************************************************************************/
typedef struct {
    module_id_t current;        // Currently loaded module
    uint8_t func_count;         // Number of functions
    void* funcs[MAX_FUNCTIONS]; // Function pointer table
} loader_state_t;

/*******************************************************************************
 * PRIVATE VARIABLES
 ******************************************************************************/
static loader_state_t g_loader = {
    .current = MODULE_COUNT,
    .func_count = 0,
    .funcs = {NULL}
};

static const uint32_t g_module_addresses[MODULE_COUNT] = {
    ADDR_MODULE_KEYPAD,
    ADDR_MODULE_DISPLAY,
    ADDR_MODULE_PRINTER
};

static uint32_t g_total_loads = 0;

/*******************************************************************************
 * PRIVATE FUNCTION PROTOTYPES
 ******************************************************************************/
static uint16_t calculate_crc16(const uint8_t *data, uint32_t length);
static void convert_to_flash_address(uint8_t *flash_addr, uint32_t address);
static loader_status_t erase_internal_flash_sector(uint32_t sector_address);
static loader_status_t write_internal_flash(uint32_t address, const uint8_t *data, uint32_t length);
static loader_status_t read_external_flash(uint32_t address, uint8_t *data, uint32_t length);

/*******************************************************************************
 * UTILITY FUNCTIONS
 ******************************************************************************/
static uint16_t calculate_crc16(const uint8_t *data, uint32_t length)
{
    uint16_t crc = 0xFFFF;
    
    for (uint32_t i = 0; i < length; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ 0x1021;
            } else {
                crc = crc << 1;
            }
        }
    }
    
    return crc;
}

static void convert_to_flash_address(uint8_t *flash_addr, uint32_t address)
{
    flash_addr[0] = (uint8_t)(address);
    flash_addr[1] = (uint8_t)(address >> 8);
    flash_addr[2] = (uint8_t)(address >> 16);
}

/*******************************************************************************
 * FLASH OPERATIONS
 ******************************************************************************/
static loader_status_t erase_internal_flash_sector(uint32_t sector_address)
{
    FLASH_SectorErase(sector_address);
    
    // Verify erase
    volatile uint8_t *verify = (volatile uint8_t*)sector_address;
    for (uint32_t i = 0; i < INTERNAL_FLASH_PAGE_SIZE; i++) {
        if (verify[i] != 0xFF) {
            return LOADER_ERR_FLASH_WRITE;
        }
    }
    
    return LOADER_OK;
}

static loader_status_t write_internal_flash(uint32_t address, const uint8_t *data, uint32_t length)
{
    FLASH_ProgramByte(address, (uint8_t*)data, length);
    
    if (FLASH_GetWriteStatus() != FLASH_WSTA_FINISH) {
        return LOADER_ERR_FLASH_WRITE;
    }
    
    // Verify write
    volatile uint8_t *verify = (volatile uint8_t*)address;
    for (uint32_t i = 0; i < length; i++) {
        if (verify[i] != data[i]) {
            return LOADER_ERR_FLASH_WRITE;
        }
    }
    
    return LOADER_OK;
}

static loader_status_t read_external_flash(uint32_t address, uint8_t *data, uint32_t length)
{
    uint8_t flash_addr[3];
    convert_to_flash_address(flash_addr, address);
    
    // Retry up to 3 times
    uint8_t retry = 3;
    while (retry--) {
        if (FLASH_gReadPage(flash_addr, data, length) == NO_ERROR) {
            return LOADER_OK;
        }
    }
    
    return LOADER_ERR_FLASH_READ;
}

/*******************************************************************************
 * PUBLIC API IMPLEMENTATION
 ******************************************************************************/
loader_status_t loader_init(void)
{
    FLASH_Init(FLASH_CSMODE_DISABLE);
    FLASH_CycleInit();
    
    if (FLASH_gInit() != 1) {
        return LOADER_ERR_FLASH_READ;
    }
    
    g_loader.current = MODULE_COUNT;
    g_loader.func_count = 0;
    memset(g_loader.funcs, 0, sizeof(g_loader.funcs));
    g_total_loads = 0;
    
    return LOADER_OK;
}

loader_status_t loader_load_module(module_id_t module_id)
{
    loader_status_t status;
    module_header_t header;
    
    if (module_id >= MODULE_COUNT) {
        return LOADER_ERR_INVALID_MODULE;
    }
    
    if (g_loader.current == module_id) {
        return LOADER_OK;
    }
    
    uint32_t ext_flash_addr = g_module_addresses[module_id];
    
    /***************************************************************************
     * STEP 1: READ HEADER
     ***************************************************************************/
    status = read_external_flash(ext_flash_addr, (uint8_t*)&header, sizeof(module_header_t));
    if (status != LOADER_OK) {
        return status;
    }
    
    if (header.magic != MODULE_MAGIC) {
        return LOADER_ERR_BAD_MAGIC;
    }
    
    if (header.module_id != module_id) {
        return LOADER_ERR_INVALID_MODULE;
    }
    
    if (header.code_size == 0 || header.code_size > MODULE_MAX_SIZE) {
        return LOADER_ERR_TOO_LARGE;
    }
    
    if (header.func_count == 0 || header.func_count > MAX_FUNCTIONS) {
        return LOADER_ERR_INVALID_MODULE;
    }
    
    /***************************************************************************
     * STEP 2: ERASE INTERNAL FLASH
     ***************************************************************************/
    uint32_t pages_needed = (header.code_size + INTERNAL_FLASH_PAGE_SIZE - 1) / INTERNAL_FLASH_PAGE_SIZE;
    
    for (uint32_t i = 0; i < pages_needed; i++) {
        uint32_t page_addr = MODULE_EXEC_BUFFER + (i * INTERNAL_FLASH_PAGE_SIZE);
        status = erase_internal_flash_sector(page_addr);
        if (status != LOADER_OK) {
            return status;
        }
    }
    
    /***************************************************************************
     * STEP 3: READ CODE IN CHUNKS
     ***************************************************************************/
    uint8_t *chunk_buffer = (uint8_t*)malloc(EXTERNAL_FLASH_PAGE_SIZE);
    if (!chunk_buffer) {
        return LOADER_ERR_NO_MEMORY;
    }
    
    uint32_t code_addr = ext_flash_addr + sizeof(module_header_t);
    uint32_t bytes_read = 0;
    uint16_t crc = 0xFFFF;
    
    while (bytes_read < header.code_size) {
        uint32_t remaining = header.code_size - bytes_read;
        uint32_t chunk_size = (remaining > EXTERNAL_FLASH_PAGE_SIZE) ? 
                              EXTERNAL_FLASH_PAGE_SIZE : remaining;
        
        status = read_external_flash(code_addr + bytes_read, chunk_buffer, chunk_size);
        if (status != LOADER_OK) {
            free(chunk_buffer);
            return status;
        }
        
        // Update CRC incrementally
        for (uint32_t i = 0; i < chunk_size; i++) {
            crc ^= (uint16_t)chunk_buffer[i] << 8;
            for (uint8_t j = 0; j < 8; j++) {
                if (crc & 0x8000) {
                    crc = (crc << 1) ^ 0x1021;
                } else {
                    crc = crc << 1;
                }
            }
        }
        
        status = write_internal_flash(MODULE_EXEC_BUFFER + bytes_read, chunk_buffer, chunk_size);
        if (status != LOADER_OK) {
            free(chunk_buffer);
            return status;
        }
        
        bytes_read += chunk_size;
    }
    
    free(chunk_buffer);
    
    /***************************************************************************
     * STEP 4: VERIFY CRC
     ***************************************************************************/
    if (crc != header.crc16) {
        return LOADER_ERR_CRC_FAIL;
    }
    
    /***************************************************************************
     * STEP 5: READ FUNCTION TABLE
     ***************************************************************************/
    uint32_t func_table_addr = ext_flash_addr + sizeof(module_header_t) + header.code_size;
    
    func_entry_t *func_entries = (func_entry_t*)malloc(header.func_count * sizeof(func_entry_t));
    if (!func_entries) {
        return LOADER_ERR_NO_MEMORY;
    }
    
    status = read_external_flash(func_table_addr, (uint8_t*)func_entries, 
                                 header.func_count * sizeof(func_entry_t));
    if (status != LOADER_OK) {
        free(func_entries);
        return status;
    }
    
    /***************************************************************************
     * STEP 6: BUILD FUNCTION POINTER TABLE
     ***************************************************************************/
    for (uint8_t i = 0; i < header.func_count; i++) {
        if (func_entries[i].offset >= header.code_size) {
            free(func_entries);
            return LOADER_ERR_INVALID_MODULE;
        }
        
      uint32_t func_addr = func_entries[i].addr;
func_addr |= 0x01;  /* Thumb */
g_loader.funcs[i] = (void*)func_addr;
    }
    
    free(func_entries);
    
    g_loader.current = module_id;
    g_loader.func_count = header.func_count;
    g_total_loads++;
    
    return LOADER_OK;
}

loader_status_t loader_unload_current(void)
{
    memset(g_loader.funcs, 0, sizeof(g_loader.funcs));
    g_loader.current = MODULE_COUNT;
    g_loader.func_count = 0;
    return LOADER_OK;
}

bool loader_is_loaded(module_id_t module_id)
{
    return (g_loader.current == module_id);
}

module_id_t loader_get_current_module(void)
{
    return g_loader.current;
}

void* loader_get_function(module_id_t module_id, uint16_t func_id)
{
    if (g_loader.current != module_id) {
        return NULL;
    }
    
    if (func_id >= g_loader.func_count) {
        return NULL;
    }
    
    return g_loader.funcs[func_id];
}

void loader_get_stats(uint32_t* total_loads, module_id_t* current_module)
{
    if (total_loads) {
        *total_loads = g_total_loads;
    }
    if (current_module) {
        *current_module = g_loader.current;
    }
}