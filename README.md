External Flash Module Loader
Prepaid Meter – V94 (Cortex-M0)
1. Purpose

The VX94 microcontroller has limited internal flash.

The prepaid meter firmware contains:

core features (always running)

optional features (used occasionally like keypad, display tools, printer, bootloader, etc.)

Keeping all features permanently in internal flash wastes space and may exceed memory.

To solve this, optional features are stored in external flash and loaded into a small internal flash execution area only when needed.

2. Approach Used

This system uses:

Code Overlay / Dynamic Module Loading

Meaning:

modules are stored outside (external flash)

copied to internal flash when required

executed

replaced later by another module

Only one module is active at a time.

3. Memory Layout
Internal Flash
+----------------------------------+
| Main firmware (core + loader)    |
| Drivers / services               |
|----------------------------------|
| Execution buffer (16 KB)         |  <-- modules copied here
| 0x0003C000 – 0x0003FFFF          |
+----------------------------------+

External Flash
+----------------------------------+
| Keypad module image              |
| Display module image             |
| Printer module image             |
| Bootloader module (future)       |
+----------------------------------+

4. System Components
4.1 Loader (loader.c)

Responsible for:

reading module from external flash

validating header

CRC check

erasing internal flash

copying code to execution buffer

building function pointer table

giving access to module functions

Main APIs
loader_init()
loader_load_module()
loader_unload_current()
loader_get_function()
loader_is_loaded()

Loading steps

When loader_load_module(id) is called:

Read module header

Check magic number

Check size and function count

Erase execution buffer

Copy code in chunks

Calculate CRC

Verify CRC

Read function table

Build function pointer list

After this, functions are ready to call.

4.2 Stub Layer (example: keypad_stub.c)

The application never calls module code directly.

Instead it calls stub functions.

Example:

void Keypad_Init(void)


What stub does:

Check if module loaded

If not → load it

Get real function pointer

Call it

This makes modules transparent to the main application.

Application code looks normal:

Keypad_Init();
Keypad_GetKey();


No need to know anything about loading.

4.3 Module Image (example: keypad module)

Each module is compiled separately.

It contains:

Sections

.module_header

.module_code

.module_func_table

Header

Contains magic number to identify valid module.

Code

All executable functions placed in:

.module_code


These functions are copied to internal flash and executed.

Function Table

List of exported functions:

typedef struct {
    uint32_t addr;
} func_entry_t;


Loader reads this table and builds pointers.

4.4 Linker Scripts
Main firmware linker

Reserves execution buffer:

0x0003C000 – 0x0003FFFF (16 KB)


Nothing else is placed there.

Module linker

Places:

.module_code        → execution region
.module_header     → metadata region
.module_func_table → metadata region


So:

only code goes to execution buffer

metadata stays in external flash

5. Execution Flow Example
Example: keypad used

Application calls:

Keypad_GetKey()


Flow:

Stub checks if keypad loaded

Not loaded → loader_load_module(KEYPAD)

Loader copies module to execution buffer

Loader builds function table

Stub calls real function

Keypad code runs from internal flash

Next calls use the already loaded module (no reload).

6. Why Execute from Internal Flash (not RAM)

Reasons:

RAM is small

code size is large

Cortex-M0 executes reliably from flash

avoids wasting RAM

So internal flash execution is the safest option.

7. Benefits of This Design
Saves internal flash

Only core firmware is permanent.

Supports many features

External flash can store many modules.

Modular

Each feature is separate and independent.

Easy to update

Modules can be replaced without touching core firmware.

Reliable

Core never changes.
Modules are validated with CRC before execution.

Simple runtime

Only one small execution buffer is needed.

Memory Usage Reduction

The system reduces internal flash consumption by storing optional features in external flash and loading only one feature at runtime.

Traditional approach

All features are linked into the main firmware:

Internal Flash =
    Core
  + Keypad
  + Display tools
  + Bootloader
  + Diagnostics
  + ...


Memory usage grows as the sum of all features.

Many of these functions are rarely used, so most of the flash remains idle during normal operation.

Current approach (module loading)

Only two parts are kept in internal flash:

Internal Flash =
    Core
  + Execution buffer (max module size)


Optional features are stored in external flash and copied into the execution buffer only when needed.

Memory usage becomes:

Core + largest module


not

Core + all modules

Example

If:

Core              = 80 KB
Largest module    = 20 KB
Other modules sum = 60 KB


Then:

Traditional design:

80 + 20 + 60 = 160 KB


Module loading:

80 + 20 = 100 KB


Saved:

60 KB (~37%)

Engineering impact

lower internal flash requirement

supports more features without increasing MCU size

better scalability

no RAM cost (execution from flash)

Only one module executes at a time, so sharing one execution buffer is sufficient.

8. Limitations

small delay while loading

flash erase time

only one module at a time

Acceptable because modules are rarely used.

9. Summary

This project implements a simple module loading system:

core firmware in internal flash

optional features in external flash

loader copies module when needed

execution happens from internal flash

stubs hide loading from application

This allows the prepaid meter to support many features while using limited internal flash.