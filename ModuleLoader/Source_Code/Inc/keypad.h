#ifndef KEYPAD_H
#define KEYPAD_H

#include <stdint.h>
#include <stdbool.h>

// Keypad module interface
bool keypad_init(void);
void keypad_deinit(void);
bool keypad_is_key_pressed(void);
uint8_t keypad_get_key(void);
void keypad_debounce_delay(void);

// Key definitions
#define KEYPAD_ERROR 0xFF
#define KEY_0 '0'
#define KEY_1 '1'
#define KEY_2 '2'
#define KEY_3 '3'
#define KEY_4 '4'
#define KEY_5 '5'
#define KEY_6 '6'
#define KEY_7 '7'
#define KEY_8 '8'
#define KEY_9 '9'
#define KEY_STAR '*'
#define KEY_HASH '#'

#endif