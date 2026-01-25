#include "keypad.h"
#include "GPIO_Interface.h"
#include "basic_srv.h"

// Keypad hardware configuration
#define KEYPAD_ROW_PORT    S_PORT_B
#define KEYPAD_COL_PORT    S_PORT_A

// Row pins (Outputs)
#define KEYPAD_ROW1_PIN    S_GPIO_Pin_0
#define KEYPAD_ROW2_PIN    S_GPIO_Pin_1  
#define KEYPAD_ROW3_PIN    S_GPIO_Pin_2
#define KEYPAD_ROW4_PIN    S_GPIO_Pin_3

// Column pins (Inputs) 
#define KEYPAD_COL1_PIN    S_GPIO_Pin_4
#define KEYPAD_COL2_PIN    S_GPIO_Pin_5
#define KEYPAD_COL3_PIN    S_GPIO_Pin_6
#define KEYPAD_COL4_PIN    S_GPIO_Pin_7

#define KEYPAD_ALL_ROWS    (KEYPAD_ROW1_PIN | KEYPAD_ROW2_PIN | KEYPAD_ROW3_PIN | KEYPAD_ROW4_PIN)
#define KEYPAD_ALL_COLS    (KEYPAD_COL1_PIN | KEYPAD_COL2_PIN | KEYPAD_COL3_PIN | KEYPAD_COL4_PIN)

// Keypad state
static bool g_keypad_initialized = false;
static uint8_t g_last_key = KEYPAD_ERROR;

// Keypad matrix mapping
static const uint8_t KEYPAD_MAP[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'}, 
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// IAR-specific section placement for execution buffer
#pragma location=".exec_code"
bool keypad_init(void)
{
    if (g_keypad_initialized) {
        return true;
    }
    
    // Initialize row pins as outputs
    S_GpioInit_PIN(KEYPAD_ROW_PORT, KEYPAD_ALL_ROWS, S_GPIO_Mode_OUTPUT_CMOS);
    S_GPIO_Set_Bits(KEYPAD_ROW_PORT, KEYPAD_ALL_ROWS);
    
    // Initialize column pins as inputs with pull-up
    S_GpioInit_PIN(KEYPAD_COL_PORT, KEYPAD_ALL_COLS, S_GPIO_Mode_INPUT);
    
    g_keypad_initialized = true;
    g_last_key = KEYPAD_ERROR;
    
    return true;
}

#pragma location=".exec_code"
void keypad_deinit(void)
{
    if (!g_keypad_initialized) {
        return;
    }
    
    // Set all pins to forbidden mode to save power
    S_GpioInit_PIN(KEYPAD_ROW_PORT, KEYPAD_ALL_ROWS, S_GPIO_Mode_FORBIDDEN);
    S_GpioInit_PIN(KEYPAD_COL_PORT, KEYPAD_ALL_COLS, S_GPIO_Mode_FORBIDDEN);
    
    g_keypad_initialized = false;
    g_last_key = KEYPAD_ERROR;
}

#pragma location=".exec_code"
void keypad_debounce_delay(void)
{
    // 20ms debounce delay
    Delay_ms(20);
}

#pragma location=".exec_code"
static uint8_t keypad_scan_matrix(void)
{
    uint8_t row, col;
    
    // Scan each row
    for (row = 0; row < 4; row++) {
        // Activate current row (set low)
        S_GPIO_Clear_Bits(KEYPAD_ROW_PORT, (1 << row));
        
        // Small delay for stabilization
        keypad_debounce_delay();
        
        // Check each column
        for (col = 0; col < 4; col++) {
            if (S_GPIO_Get_Bits(KEYPAD_COL_PORT, (1 << (col + 4))) == 0) {
                // Key pressed - deactivate row and return key
                S_GPIO_Set_Bits(KEYPAD_ROW_PORT, (1 << row));
                return KEYPAD_MAP[row][col];
            }
        }
        
        // Deactivate current row
        S_GPIO_Set_Bits(KEYPAD_ROW_PORT, (1 << row));
    }
    
    return KEYPAD_ERROR;
}

#pragma location=".exec_code"
bool keypad_is_key_pressed(void)
{
    if (!g_keypad_initialized) {
        return false;
    }
    
    uint8_t key = keypad_scan_matrix();
    return (key != KEYPAD_ERROR);
}

#pragma location=".exec_code"
uint8_t keypad_get_key(void)
{
    if (!g_keypad_initialized) {
        return KEYPAD_ERROR;
    }
    
    uint8_t key = keypad_scan_matrix();
    
    // Simple debouncing - only return key if it's stable
    if (key != KEYPAD_ERROR) {
        keypad_debounce_delay();
        uint8_t key2 = keypad_scan_matrix();
        
        if (key == key2) {
            g_last_key = key;
            return key;
        }
    }
    
    return KEYPAD_ERROR;
}

#pragma location=".exec_code"
uint8_t keypad_get_last_key(void)
{
    return g_last_key;
}

#pragma location=".exec_code"
uint8_t keypad_wait_for_key(void)
{
    uint8_t key;
    
    // Wait for key press
    while ((key = keypad_get_key()) == KEYPAD_ERROR) {
        // Small delay to reduce CPU usage
        Delay_ms(10);
    }
    
    return key;
}

// Function table structure for loader
#pragma location=".exec_code"
typedef struct {
    bool (*init)(void);
    void (*deinit)(void);
    bool (*is_key_pressed)(void);
    uint8_t (*get_key)(void);
    void (*debounce_delay)(void);
    uint8_t (*get_last_key)(void);
    uint8_t (*wait_for_key)(void);
} keypad_module_functions_t;

// Function table instance
#pragma location=".exec_code"
const keypad_module_functions_t keypad_functions = {
    keypad_init,
    keypad_deinit,
    keypad_is_key_pressed,
    keypad_get_key,
    keypad_debounce_delay,
    keypad_get_last_key,
    keypad_wait_for_key
};