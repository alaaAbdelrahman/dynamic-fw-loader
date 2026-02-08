#ifndef MODULE_LOADER_H
#define MODULE_LOADER_H

#include <stdint.h>
#include <stdbool.h>

/*=========================================================
  CONFIGURATION
=========================================================*/

/* Internal flash execution window */
#define MODULE_EXEC_BUFFER      0x0003C000u
#define MODULE_MAX_SIZE         16384u      /* 16 KB */

/* Internal flash geometry */
#define INTERNAL_FLASH_PAGE_SIZE   512u

/* External flash geometry */
#define EXTERNAL_FLASH_PAGE_SIZE   256u

/* Module layout in external flash */
#define MODULE_FLASH_BASE       0x000000u
#define MODULE_SLOT_SIZE        0x4000u     /* 16 KB per slot */

/* Limits */
#define MAX_FUNCTIONS           8u

/*=========================================================
  MODULE IDS
=========================================================*/
typedef enum {
    MODULE_KEYPAD = 0,
    MODULE_DISPLAY,
    MODULE_PRINTER,
    MODULE_COUNT
} module_id_t;

/*=========================================================
  EXTERNAL FLASH ADDRESSES
=========================================================*/
#define ADDR_MODULE_KEYPAD   (MODULE_FLASH_BASE + MODULE_KEYPAD  * MODULE_SLOT_SIZE)
#define ADDR_MODULE_DISPLAY  (MODULE_FLASH_BASE + MODULE_DISPLAY * MODULE_SLOT_SIZE)
#define ADDR_MODULE_PRINTER  (MODULE_FLASH_BASE + MODULE_PRINTER * MODULE_SLOT_SIZE)

/*=========================================================
  STATUS
=========================================================*/
typedef enum {
    LOADER_OK = 0,
    LOADER_ERR_FLASH_READ,
    LOADER_ERR_FLASH_WRITE,
    LOADER_ERR_BAD_MAGIC,
    LOADER_ERR_CRC_FAIL,
    LOADER_ERR_TOO_LARGE,
    LOADER_ERR_INVALID_MODULE
} loader_status_t;

/*=========================================================
  MODULE FORMAT
=========================================================*/
#pragma pack(push,1)

typedef struct {
    uint16_t magic;
    uint16_t code_size;
    uint16_t crc16;
    uint8_t  func_count;
    uint8_t  module_id;
    uint32_t reserved;
} module_header_t;

typedef struct {
    uint32_t addr;
} func_entry_t;

/*=========================================================
  API
=========================================================*/

loader_status_t loader_init(void);
loader_status_t loader_load_module(module_id_t module_id);
loader_status_t loader_unload_current(void);

bool        loader_is_loaded(module_id_t module_id);
module_id_t loader_get_current_module(void);

void* loader_get_function(module_id_t module_id, uint16_t func_id);

void loader_get_stats(uint32_t* total_loads, module_id_t* current_module);

#endif
