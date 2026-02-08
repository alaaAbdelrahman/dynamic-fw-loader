/*******************************************************************************
 * MODULE LOADER - Header File
 * 
 * File: module_loader.h
 * 
 * Dynamic module loading system for embedded systems with external flash
 * Supports loading compiled modules from FM25W32 external flash to internal
 * flash execution buffer.
 * 
 * Features:
 * - Minimal metadata (12-byte header)
 * - CRC16 integrity verification
 * - Single execution buffer (one module at a time)
 * - Automatic module switching
 * - Multiple module support (keypad, display, printer, etc.)
 * 
 * Author: Your Name
 * Date: 2024
 ******************************************************************************/

#ifndef MODULE_LOADER_H
#define MODULE_LOADER_H

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
 * CONFIGURATION - Adjust for your system
 ******************************************************************************/

// Internal flash configuration
#define MODULE_EXEC_BUFFER      0x0003C000  // Execution buffer base address
#define MODULE_MAX_SIZE         16384       // 16KB max per module
#define FLASH_SECTOR_SIZE       2048        // Internal flash sector size (bytes)

// External flash configuration (FM25W32)
#define MODULE_FLASH_BASE       0x000000    // Base address in external flash
#define MODULE_SLOT_SIZE        0x00100     // 16KB per module slot

// Module limits
#define MAX_FUNCTIONS           8           // Max functions per module

/*******************************************************************************
 * MODULE IDs - Define your modules here
 ******************************************************************************/
typedef enum {
    MODULE_KEYPAD = 0,      // Keypad input module
    MODULE_DISPLAY = 1,     // Display output module
    MODULE_PRINTER = 2,     // Printer module
    MODULE_COUNT            // Total number of modules (keep last!)
} module_id_t;

/*******************************************************************************
 * EXTERNAL FLASH ADDRESSES
 * Each module gets a 64KB slot for easy addressing
 ******************************************************************************/
#define ADDR_MODULE_KEYPAD      (MODULE_FLASH_BASE + (MODULE_KEYPAD * MODULE_SLOT_SIZE))
#define ADDR_MODULE_DISPLAY     (MODULE_FLASH_BASE + (MODULE_DISPLAY * MODULE_SLOT_SIZE))
#define ADDR_MODULE_PRINTER     (MODULE_FLASH_BASE + (MODULE_PRINTER * MODULE_SLOT_SIZE))

/*******************************************************************************
 * STATUS CODES
 ******************************************************************************/
typedef enum {
    LOADER_OK = 0,                  // Success
    LOADER_ERR_FLASH_READ,          // External flash read failed
    LOADER_ERR_FLASH_WRITE,         // Internal flash write failed
    LOADER_ERR_BAD_MAGIC,           // Invalid magic number (not a module)
    LOADER_ERR_CRC_FAIL,            // CRC mismatch (corrupted module)
    LOADER_ERR_TOO_LARGE,           // Module exceeds maximum size
    LOADER_ERR_NO_MEMORY,           // malloc failed
    LOADER_ERR_INVALID_MODULE,      // Invalid module ID or mismatch
    LOADER_ERR_NOT_LOADED           // Module not loaded
} loader_status_t;

/*******************************************************************************
 * MODULE HEADER STRUCTURE (12 bytes)
 * Stored at the beginning of each module in external flash
 ******************************************************************************/
#pragma pack(push, 1)
typedef struct {
    uint16_t magic;          // Magic number: 0x4D4D ("MM")
    uint16_t code_size;      // Size of executable code in bytes
    uint16_t crc16;          // CRC16-CCITT of code section
    uint8_t  func_count;     // Number of functions in module
    uint8_t  module_id;      // Module ID (for verification)
    uint32_t reserved;       // Reserved for future use
} module_header_t;

/*******************************************************************************
 * FUNCTION ENTRY STRUCTURE (4 bytes)
 * Array of these follows the module code in external flash
 ******************************************************************************/
typedef struct {
    uint16_t func_id;        // Function identifier (0, 1, 2...)
    uint16_t offset;         // Offset from module base address
} func_entry_t;
#pragma pack(pop)

/*******************************************************************************
 * PUBLIC API FUNCTIONS
 ******************************************************************************/

/**
 * @brief Initialize the module loader system
 * 
 * Must be called once at system startup before using any modules.
 * Initializes both internal and external flash drivers.
 * 
 * @return LOADER_OK on success, error code otherwise
 */
loader_status_t loader_init(void);

/**
 * @brief Load a module from external flash to execution buffer
 * 
 * If a different module is currently loaded, it will be automatically
 * unloaded first. If the requested module is already loaded, this
 * function returns immediately (fast path).
 * 
 * Process:
 * 1. Read and validate module header
 * 2. Erase internal flash execution buffer
 * 3. Read module code from external flash
 * 4. Verify CRC16 integrity
 * 5. Write code to internal flash
 * 6. Build function pointer table
 * 
 * @param module_id Module to load (MODULE_KEYPAD, MODULE_DISPLAY, etc.)
 * @return LOADER_OK on success, error code otherwise
 */
loader_status_t loader_load_module(module_id_t module_id);

/**
 * @brief Unload the currently loaded module
 * 
 * Frees resources and marks execution buffer as empty.
 * Note: Usually you don't need to call this manually, as loader_load_module()
 * automatically unloads the previous module when loading a new one.
 * 
 * @return LOADER_OK on success
 */
loader_status_t loader_unload_current(void);

/**
 * @brief Check if a specific module is currently loaded
 * 
 * @param module_id Module to check
 * @return true if loaded, false otherwise
 */
bool loader_is_loaded(module_id_t module_id);

/**
 * @brief Get the currently loaded module ID
 * 
 * @return Module ID, or MODULE_COUNT if no module loaded
 */
module_id_t loader_get_current_module(void);

/**
 * @brief Get function pointer from loaded module
 * 
 * Used by stub functions to get the actual implementation address.
 * 
 * @param module_id Module ID (must be currently loaded)
 * @param func_id Function ID within module (0, 1, 2...)
 * @return Function pointer on success, NULL on error
 * 
 * @note The returned pointer already has the Thumb bit set (LSB = 1)
 *       and can be called directly.
 * 
 * Example:
 * @code
 * typedef void (*func_t)(void);
 * func_t func = (func_t)loader_get_function(MODULE_KEYPAD, 0);
 * if (func) func();  // Call Keypad_Init
 * @endcode
 */
void* loader_get_function(module_id_t module_id, uint16_t func_id);

/*******************************************************************************
 * UTILITY FUNCTIONS (Optional - for debugging/diagnostics)
 ******************************************************************************/

/**
 * @brief Get loader statistics
 * 
 * @param total_loads Total number of successful module loads
 * @param current_module Currently loaded module ID
 */
void loader_get_stats(uint32_t* total_loads, module_id_t* current_module);

/**
 * @brief Get human-readable error message
 * 
 * @param status Error code
 * @return Error message string
 */
const char* loader_get_error_string(loader_status_t status);

#endif // MODULE_LOADER_H

/*******************************************************************************
 * USAGE EXAMPLE
 * 
 * // In main.c:
 * #include "module_loader.h"
 * 
 * int main(void)
 * {
 *     // Initialize system
 *     SystemInit();
 *     
 *     // Initialize loader
 *     if (loader_init() != LOADER_OK) {
 *         // Handle error
 *         while(1);
 *     }
 *     
 *     // Use modules through stub functions
 *     Keypad_Init();           // Loads keypad module automatically
 *     char key = Keypad_GetKey();
 *     
 *     Display_Print("Hello");  // Loads display module (unloads keypad)
 *     
 *     while(1) {
 *         // Your application code
 *     }
 * }
 * 
 ******************************************************************************/