/*
 * flash_module_programmer.c
 *
 * External flash module programming routine
 * Phase-1 dynamic module loader support
 *
 * Uses flash_intf.h API (sector erase + page write)
 */

#include <stdint.h>
#include <string.h>
#include "flash_intf.h"

/* =========================================================
 * Flash geometry (must match external flash)
 * ========================================================= */
#define EXT_FLASH_SECTOR_SIZE   4096U
#define EXT_FLASH_PAGE_SIZE     256U

/* =========================================================
 * Flash module programming
 * ========================================================= */
uint8_t Flash_Program_Module(uint32_t ext_addr,
                             const uint8_t *image,
                             uint32_t image_size)
{
    uint32_t offset = 0;
    uint32_t erase_size;
    uint8_t status;

    if (image == NULL || image_size == 0)
    {
        return FLASH_ERROR_OP;
    }

    /* -----------------------------------------------------
     * 1. Erase required sectors
     * ----------------------------------------------------- */
    erase_size = (image_size + EXT_FLASH_SECTOR_SIZE - 1) &
                 ~(EXT_FLASH_SECTOR_SIZE - 1);

    for (uint32_t addr = ext_addr;
         addr < (ext_addr + erase_size);
         addr += EXT_FLASH_SECTOR_SIZE)
    {
        status = Flash_erase_sector_by_address(addr);
        if (status != NO_ERROR)
        {
            return FLASH_SECTOR_ERASE_ERROR;
        }
    }

    /* -----------------------------------------------------
     * 2. Write image page by page
     * ----------------------------------------------------- */
    while (offset < image_size)
    {
        uint32_t chunk =
            (image_size - offset > EXT_FLASH_PAGE_SIZE) ?
            EXT_FLASH_PAGE_SIZE :
            (image_size - offset);

        status = FlashWrite(ext_addr + offset,
                            (uint8_t *)&image[offset],
                            (uint16_t)chunk);

        if (status != NO_ERROR)
        {
            return FLASH_WRITE_ERROR;
        }

        offset += chunk;
    }

    return NO_ERROR;
}

/* =========================================================
 * Flash module verification (read-back compare)
 * ========================================================= */
uint8_t Flash_Verify_Module(uint32_t ext_addr,
                            const uint8_t *image,
                            uint32_t image_size)
{
    uint8_t temp[EXT_FLASH_PAGE_SIZE];
    uint32_t offset = 0;
    uint8_t status;

    if (image == NULL || image_size == 0)
    {
        return FLASH_ERROR_OP;
    }

    while (offset < image_size)
    {
        uint32_t chunk =
            (image_size - offset > EXT_FLASH_PAGE_SIZE) ?
            EXT_FLASH_PAGE_SIZE :
            (image_size - offset);

        status = FlashRead(ext_addr + offset,
                           temp,
                           (uint16_t)chunk);

        if (status != NO_ERROR)
        {
            return FLASH_READ_ERROR;
        }

        if (compare_2_buffer((uint8_t *)&image[offset],
                             temp,
                             chunk) != 0)
        {
            return FLASH_COMPARE_ERROR;
        }

        offset += chunk;
    }

    return NO_ERROR;
}

/* =========================================================
 * Example service routine (development / factory use)
 * ========================================================= */
#define EXT_ADDR_KEYPAD_MODULE   0x00100000U

extern const uint8_t keypad_module_bin[];
extern const uint32_t keypad_module_size;

void Service_Flash_Keypad_Module(void)
{
    uint8_t status;

    status = Flash_Program_Module(EXT_ADDR_KEYPAD_MODULE,
                                  keypad_module_bin,
                                  keypad_module_size);
    if (status != NO_ERROR)
    {
        /* handle erase/write error */
        return;
    }

    status = Flash_Verify_Module(EXT_ADDR_KEYPAD_MODULE,
                                 keypad_module_bin,
                                 keypad_module_size);
    if (status != NO_ERROR)
    {
        /* handle verification failure */
        return;
    }

    /* Module successfully programmed */
}
