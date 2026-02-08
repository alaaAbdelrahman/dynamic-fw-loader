#include "loader.h"
#include "Flash.h"
#include "V94XX_FLASH.h"
#include <string.h>

/*=========================================================
  PRIVATE DEFINITIONS
=========================================================*/

#define MODULE_MAGIC   0x4D4D   /* "MM" */

/*=========================================================
  PRIVATE TYPES
=========================================================*/

typedef struct {
    module_id_t current;
    uint8_t func_count;
    void* funcs[MAX_FUNCTIONS];
} loader_state_t;

/* Function table entry (absolute address) */

/*=========================================================
  PRIVATE STATE
=========================================================*/

static loader_state_t g_loader = {
    .current = MODULE_COUNT
};

static const uint32_t g_module_addresses[MODULE_COUNT] = {
    ADDR_MODULE_KEYPAD,
    ADDR_MODULE_DISPLAY,
    ADDR_MODULE_PRINTER
};

static uint32_t g_total_loads = 0;

/* Static buffers (no heap) */
static uint8_t      g_chunk_buffer[EXTERNAL_FLASH_PAGE_SIZE];
static func_entry_t g_func_entries[MAX_FUNCTIONS];

/*=========================================================
  CRC16
=========================================================*/

static uint16_t crc16_update(uint16_t crc, const uint8_t *data, uint32_t len)
{
    while (len--)
    {
        crc ^= (uint16_t)(*data++) << 8;

        for (uint8_t i = 0; i < 8; i++)
            crc = (crc & 0x8000) ? (crc << 1) ^ 0x1021 : (crc << 1);
    }
    return crc;
}

/*=========================================================
  FLASH ADDRESS CONVERSION (LSB first for your driver)
=========================================================*/

static void convert_addr(uint8_t *out, uint32_t addr)
{
    out[0] = (uint8_t)(addr);
    out[1] = (uint8_t)(addr >> 8);
    out[2] = (uint8_t)(addr >> 16);
}

/*=========================================================
  FLASH HELPERS
=========================================================*/

static loader_status_t erase_sector(uint32_t addr)
{
    /* Disable IRQ during internal flash operations */
    __disable_irq();
    FLASH_SectorErase(addr);
    __enable_irq();

    volatile uint8_t *p = (volatile uint8_t*)addr;

    for (uint32_t i = 0; i < INTERNAL_FLASH_PAGE_SIZE; i++)
        if (p[i] != 0xFF)
            return LOADER_ERR_FLASH_WRITE;

    return LOADER_OK;
}

/*-------------------------------------------------------*/

static loader_status_t write_flash(uint32_t addr,
                                   const uint8_t *data,
                                   uint32_t len)
{
    __disable_irq();
    FLASH_ProgramByte(addr, (uint8_t*)data, len);
    __enable_irq();

    if (FLASH_GetWriteStatus() != FLASH_WSTA_FINISH)
        return LOADER_ERR_FLASH_WRITE;

    volatile uint8_t *p = (volatile uint8_t*)addr;

    for (uint32_t i = 0; i < len; i++)
        if (p[i] != data[i])
            return LOADER_ERR_FLASH_WRITE;

    return LOADER_OK;
}

/*-------------------------------------------------------*/

static loader_status_t read_ext(uint32_t addr,
                                uint8_t *buf,
                                uint32_t len)
{
    uint8_t a[3];
    convert_addr(a, addr);

    if (FLASH_gReadPage(a, buf, len) == NO_ERROR)
        return LOADER_OK;

    return LOADER_ERR_FLASH_READ;
}

/*=========================================================
  PUBLIC API
=========================================================*/

loader_status_t loader_init(void)
{
    FLASH_Init(FLASH_CSMODE_DISABLE);
    FLASH_CycleInit();

    if (FLASH_gInit() != 1)
        return LOADER_ERR_FLASH_READ;

    g_loader.current = MODULE_COUNT;
    g_loader.func_count = 0;
    memset(g_loader.funcs, 0, sizeof(g_loader.funcs));
    g_total_loads = 0;

    return LOADER_OK;
}

/*=========================================================*/

loader_status_t loader_load_module(module_id_t id)
{
    if (id >= MODULE_COUNT)
        return LOADER_ERR_INVALID_MODULE;

    if (g_loader.current == id)
        return LOADER_OK;

    uint32_t base = g_module_addresses[id];

    /* ---------- Read header ---------- */

    module_header_t header;

    if (read_ext(base, (uint8_t*)&header, sizeof(header)) != LOADER_OK)
        return LOADER_ERR_FLASH_READ;

    if (header.magic != MODULE_MAGIC ||
        header.module_id != id ||
        header.code_size == 0 ||
        header.code_size > MODULE_MAX_SIZE ||
        header.func_count > MAX_FUNCTIONS)
        return LOADER_ERR_INVALID_MODULE;

    /* ---------- Erase execution buffer ---------- */

    uint32_t pages =
        (header.code_size + INTERNAL_FLASH_PAGE_SIZE - 1)
        / INTERNAL_FLASH_PAGE_SIZE;

    for (uint32_t i = 0; i < pages; i++)
    {
        loader_status_t s =
            erase_sector(MODULE_EXEC_BUFFER + i * INTERNAL_FLASH_PAGE_SIZE);

        if (s != LOADER_OK)
            return s;
    }

    /* ---------- Copy + CRC ---------- */

    uint32_t code_addr = base + sizeof(header);
    uint32_t copied = 0;
    uint16_t crc = 0xFFFF;

    while (copied < header.code_size)
    {
        uint32_t size =
            (header.code_size - copied > EXTERNAL_FLASH_PAGE_SIZE)
            ? EXTERNAL_FLASH_PAGE_SIZE
            : header.code_size - copied;

        if (read_ext(code_addr + copied, g_chunk_buffer, size) != LOADER_OK)
            return LOADER_ERR_FLASH_READ;

        crc = crc16_update(crc, g_chunk_buffer, size);

        loader_status_t s =
            write_flash(MODULE_EXEC_BUFFER + copied, g_chunk_buffer, size);

        if (s != LOADER_OK)
            return s;

        copied += size;
    }

    if (crc != header.crc16)
        return LOADER_ERR_CRC_FAIL;

    /* ---------- Read function table (absolute addresses) ---------- */

    uint32_t table_addr = base + sizeof(header) + header.code_size;

    if (read_ext(table_addr,
                 (uint8_t*)g_func_entries,
                 header.func_count * sizeof(func_entry_t)) != LOADER_OK)
        return LOADER_ERR_FLASH_READ;

    /* ---------- Build pointer table ---------- */

    for (uint8_t i = 0; i < header.func_count; i++)
    {
        uint32_t addr = g_func_entries[i].addr;

        addr |= 1u; /* Thumb bit */

        g_loader.funcs[i] = (void*)addr;
    }

    g_loader.current = id;
    g_loader.func_count = header.func_count;
    g_total_loads++;

    return LOADER_OK;
}

/*=========================================================*/

loader_status_t loader_unload_current(void)
{
    memset(g_loader.funcs, 0, sizeof(g_loader.funcs));
    g_loader.current = MODULE_COUNT;
    g_loader.func_count = 0;
    return LOADER_OK;
}

/*=========================================================*/

bool loader_is_loaded(module_id_t id)
{
    return (g_loader.current == id);
}

module_id_t loader_get_current_module(void)
{
    return g_loader.current;
}

void* loader_get_function(module_id_t id, uint16_t fid)
{
    if (g_loader.current != id || fid >= g_loader.func_count)
        return NULL;

    return g_loader.funcs[fid];
}

void loader_get_stats(uint32_t* loads, module_id_t* current)
{
    if (loads)   *loads   = g_total_loads;
    if (current) *current = g_loader.current;
}
