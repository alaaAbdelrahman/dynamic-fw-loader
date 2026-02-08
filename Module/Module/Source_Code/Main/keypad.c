#include <stdint.h>
#include <stdbool.h>
#include "GPIO_Interface.h"
#include "basic_srv.h"



/* ===== MODULE CONSTANTS ===== */

#define MODULE_EXEC_BUFFER  0x0003C000
#define KEYPAD_ERROR        0xFF

/* ===== MODULE HEADER ===== */

#pragma location = ".module_header"
__root const uint32_t module_magic = 0x4D4D;

/* ===== FUNCTION PROTOTYPES ===== */

__root bool keypad_init(void);
__root void keypad_deinit(void);
__root uint8_t keypad_get_key(void);
__root uint8_t keypad_wait_for_key(void);

/* ===== FUNCTION IMPLEMENTATIONS ===== */

#pragma location = ".module_code"
__root __attribute__((section(".module_code")))
 bool keypad_init(void)
{  
    /* hardware init only */
    return true;
}

#pragma location = ".module_code"
__root __attribute__((section(".module_code")))
void keypad_deinit(void)
{
}

#pragma location = ".module_code"
__root __attribute__((section(".module_code")))
uint8_t keypad_get_key(void)
{
  if(!(keypad_init()))
    return KEYPAD_ERROR;
}

#pragma location = ".module_code"
 __root __attribute__((section(".module_code")))

uint8_t keypad_wait_for_key(void)
{
    uint8_t k;
    do {
        k = keypad_get_key();
    } while (k == KEYPAD_ERROR);
    return k;
}

/* ===== FUNCTION TABLE (THIS WAS MISSING BEFORE) ===== */

typedef struct {
    uint32_t addr;
} func_entry_t;

#pragma location = ".module_func_table"
     __root __attribute__((section(".module_func_table")))

 const func_entry_t keypad_func_table[] =
{
    { (uint32_t)keypad_init },
    { (uint32_t)keypad_deinit },
    { (uint32_t)keypad_get_key },
    { (uint32_t)keypad_wait_for_key }
};