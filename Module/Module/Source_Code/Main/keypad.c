#include <stdint.h>
#include <stdbool.h>
#include "GPIO_Interface.h"
#include "basic_srv.h"

#define KEYPAD_ERROR  0xFF

/*=================================================
  CODE ONLY (no header here)
=================================================*/

__root __attribute__((section(".module_code")))
bool keypad_init(void)
{
    return true;
}

__root __attribute__((section(".module_code")))
void keypad_deinit(void)
{
}

__root __attribute__((section(".module_code")))
uint8_t keypad_get_key(void)
{
    if (!keypad_init())
        return KEYPAD_ERROR;

    return 1;
}

__root __attribute__((section(".module_code")))
uint8_t keypad_wait_for_key(void)
{
    uint8_t k;

    do {
        k = keypad_get_key();
    } while (k == KEYPAD_ERROR);

    return k;
}

/*=================================================
  FUNCTION TABLE (ABSOLUTE ADDRESSES)
=================================================*/

typedef struct {
    uint32_t addr;
} func_entry_t;

__root __attribute__((section(".module_func_table")))
const func_entry_t keypad_func_table[] =
{
    { (uint32_t)keypad_init },
    { (uint32_t)keypad_deinit },
    { (uint32_t)keypad_get_key },
    { (uint32_t)keypad_wait_for_key }
};
