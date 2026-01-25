#include "loader.h"
#include "Flash.h"          // Internal flash (for execution buffer)
#include <string.h>
#include <stdlib.h>

// Module header structure (stored in external flash)
typedef struct {
    uint16_t magic;
    uint16_t module_id;
    uint16_t code_size;
    uint16_t crc16;
} loader_module_header_t;

typedef struct {
    uint16_t function_id;
    uint16_t function_offset;
} loader_function_entry_t;

typedef struct {
    loader_module_id_t id;
    uint32_t flash_addr;     // External FM25W32 address
    uint16_t size;
    bool loaded;
    uint16_t function_count;
    void** function_table;
} loader_module_info_t;


static loader_module_info_t g_modules[LOADER_MODULE_COUNT] = {0};
static loader_module_id_t g_current_loaded_module = LOADER_MODULE_NONE;

/*******************************************************************************
* Function Name  : calculate_crc16
* Description    : Calculate CRC16 for module validation
*******************************************************************************/
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

/*******************************************************************************
* Function Name  : module_loader_init
* Description    : Initialize module loader with FM25W32 flash
*******************************************************************************/
ldr_status_t module_loader_init(void)
{
    // Initialize internal flash driver (for execution buffer)
    if (FLASH_gInit() != 1) {
        return LDR_ERR_FLASH_WRITE;
    }
    
    // Initialize external FM25W32 flash (for module storage)
    // Note: Your FM25W32 uses the same FLASH_gInit() function
    if (FLASH_gInit() != 1) {
        return LDR_ERR_FLASH_READ;
    }
    
    // Initialize modules registry
    memset(g_modules, 0, sizeof(g_modules));
    
    // Register available modules with their FLASH addresses
    g_modules[LOADER_MODULE_KEYPAD].id = LOADER_MODULE_KEYPAD;
    g_modules[LOADER_MODULE_KEYPAD].flash_addr = LOADER_KEYPAD_MODULE_FLASH_ADDR;
    g_modules[LOADER_MODULE_KEYPAD].size = 4096;
    g_modules[LOADER_MODULE_KEYPAD].loaded = false;
    g_modules[LOADER_MODULE_KEYPAD].function_count = 0;
    g_modules[LOADER_MODULE_KEYPAD].function_table = NULL;
    
    g_modules[LOADER_MODULE_DISPLAY].id = LOADER_MODULE_DISPLAY;
    g_modules[LOADER_MODULE_DISPLAY].flash_addr = LOADER_DISPLAY_MODULE_FLASH_ADDR;
    g_modules[LOADER_MODULE_DISPLAY].size = 8192;
    g_modules[LOADER_MODULE_DISPLAY].loaded = false;
    g_modules[LOADER_MODULE_DISPLAY].function_count = 0;
    g_modules[LOADER_MODULE_DISPLAY].function_table = NULL;
    
    g_modules[LOADER_MODULE_CRYPTO].id = LOADER_MODULE_CRYPTO;
    g_modules[LOADER_MODULE_CRYPTO].flash_addr = LOADER_CRYPTO_MODULE_FLASH_ADDR;
    g_modules[LOADER_MODULE_CRYPTO].size = 6144;
    g_modules[LOADER_MODULE_CRYPTO].loaded = false;
    g_modules[LOADER_MODULE_CRYPTO].function_count = 0;
    g_modules[LOADER_MODULE_CRYPTO].function_table = NULL;
    
    return LDR_OK;
}

/*******************************************************************************
* Function Name  : load_module
* Description    : Load module from FM25W32 flash to internal execution buffer
*******************************************************************************/
ldr_status_t load_module(loader_module_id_t module_id)
{
    if (module_id >= LOADER_MODULE_COUNT) {
        return LDR_ERR_MODULE_NOT_FOUND;
    }
    
    loader_module_info_t* module = &g_modules[module_id];
    
    // Return if already loaded
    if (module->loaded && g_current_loaded_module == module_id) {
        return LDR_OK;
    }
    
    // Unload current module if different
    if (g_current_loaded_module != LOADER_MODULE_NONE && g_current_loaded_module != module_id) {
        unload_module(g_current_loaded_module);
    }
    
    // Read module header from external FM25W32 flash
    loader_module_header_t header;
    
    // Use FlashRead with linear address
    if (FlashRead(module->flash_addr, (uint8_t*)&header, sizeof(header)) != 1) {
        return LDR_ERR_FLASH_READ;
    }
    
    // Validate module header
    if (header.magic != 0x4D4D) {
        return LDR_ERR_BAD_HEADER;
    }
    
    if (header.module_id != module_id) {
        return LDR_ERR_BAD_HEADER;
    }
    
    if (header.code_size == 0 || header.code_size > LOADER_MODULE_MAX_SIZE) {
        return LDR_ERR_BAD_HEADER;
    }
    
    // Calculate internal flash sector for execution buffer
    // Assuming your internal flash starts at 0x08000000 and sectors are 4KB
    uint16_t flash_sector = (LOADER_MODULE_FLASH_BASE - 0x08000000U) / 4096U;
    
    // Erase internal flash execution buffer (4 sectors = 16KB)
    for (uint8_t i = 0; i < 4; i++) {
        if (FLASH_gEraseSector_ByIndex(flash_sector + i, 80) != 1) {
            return LDR_ERR_FLASH_WRITE;
        }
    }
    
    // Load module code from external FM25W32 to internal flash
    uint32_t ext_flash_current_addr = module->flash_addr + sizeof(loader_module_header_t);
    uint32_t bytes_remaining = header.code_size;
    
    // Read and verify the entire code section first
    uint8_t* code_buffer = (uint8_t*)malloc(header.code_size);
    if (!code_buffer) {
        return LDR_ERR_MEMORY_ALLOC;
    }
    
    // Read entire code section from external flash
    if (FlashRead(ext_flash_current_addr, code_buffer, header.code_size) != 1) {
        free(code_buffer);
        return LDR_ERR_FLASH_READ;
    }
    
    // Verify CRC16 of the code section
    uint16_t calculated_crc = calculate_crc16(code_buffer, header.code_size);
    if (calculated_crc != header.crc16) {
        free(code_buffer);
        return LDR_ERR_CRC_MISMATCH;
    }
    
    // Write code to internal flash using page writes
    uint32_t bytes_written = 0;
    uint16_t page_index = 0;
    uint16_t sector_number = flash_sector;
    
    while (bytes_written < header.code_size) {
        uint16_t chunk_size = (header.code_size - bytes_written > 256) ? 256 : (header.code_size - bytes_written);
        uint16_t byte_index = bytes_written % FLASH_PAGE_SIZE;
        
        // Use FLASH_gWrite_ByIndex for internal flash writing
        if (FLASH_gWrite_ByIndex(byte_index, page_index, sector_number, 
                                &code_buffer[bytes_written], chunk_size) != 1) {
            free(code_buffer);
            return LDR_ERR_FLASH_WRITE;
        }
        
        bytes_written += chunk_size;
        page_index++;
        
        // Move to next sector if we've written 16 pages (4KB)
        if (page_index >= 16) {
            page_index = 0;
            sector_number++;
        }
    }
    
    free(code_buffer);
    
    // Read function table from external flash (located after code)
    uint32_t func_table_addr = module->flash_addr + sizeof(loader_module_header_t) + header.code_size;
    uint16_t function_count;
    
    // Read function count
    if (FlashRead(func_table_addr, (uint8_t*)&function_count, sizeof(function_count)) != 1) {
        return LDR_ERR_FLASH_READ;
    }
    
    func_table_addr += sizeof(function_count);
    
    // Read function entries
    loader_function_entry_t* func_entries = (loader_function_entry_t*)malloc(function_count * sizeof(loader_function_entry_t));
    if (!func_entries) {
        return LDR_ERR_MEMORY_ALLOC;
    }
    
    if (FlashRead(func_table_addr, (uint8_t*)func_entries, 
                   function_count * sizeof(loader_function_entry_t)) != 1) {
        free(func_entries);
        return LDR_ERR_FLASH_READ;
    }
    
    // Build function pointer table in RAM
    module->function_table = (void**)malloc(function_count * sizeof(void*));
    if (!module->function_table) {
        free(func_entries);
        return LDR_ERR_MEMORY_ALLOC;
    }
    
    for (uint16_t i = 0; i < function_count; i++) {
        // Calculate actual function address in internal flash execution buffer
        module->function_table[i] = (void*)(LOADER_MODULE_FLASH_BASE + func_entries[i].function_offset);
    }
    
    module->function_count = function_count;
    module->loaded = true;
    g_current_loaded_module = module_id;
    
    free(func_entries);
    return LDR_OK;
}

/*******************************************************************************
* Function Name  : unload_module
* Description    : Unload module and free resources
*******************************************************************************/
ldr_status_t unload_module(loader_module_id_t module_id)
{
    if (module_id >= LOADER_MODULE_COUNT || !g_modules[module_id].loaded) {
        return LDR_OK;
    }
    
    loader_module_info_t* module = &g_modules[module_id];
    
    // Free function table
    if (module->function_table) {
        free(module->function_table);
        module->function_table = NULL;
    }
    
    module->loaded = false;
    module->function_count = 0;
    
    if (g_current_loaded_module == module_id) {
        g_current_loaded_module = LOADER_MODULE_NONE;
    }
    
    return LDR_OK;
}

/*******************************************************************************
* Function Name  : is_module_loaded
* Description    : Check if module is currently loaded
*******************************************************************************/
bool is_module_loaded(loader_module_id_t module_id)
{
    if (module_id >= LOADER_MODULE_COUNT) {
        return false;
    }
    return g_modules[module_id].loaded;
}

/*******************************************************************************
* Function Name  : get_module_function
* Description    : Get function pointer from loaded module
*******************************************************************************/
void* get_module_function(loader_module_id_t module_id, uint16_t function_id)
{
    if (!is_module_loaded(module_id)) {
        return NULL;
    }
    
    loader_module_info_t* module = &g_modules[module_id];
    
    if (module->function_table == NULL) {
        return NULL;
    }
    
    if (function_id >= module->function_count) {
        return NULL;
    }
    
    return module->function_table[function_id];
}