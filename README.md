# Dynamic Module Loader
### External Flash Overlay Execution System (Cortex-M0 / V94XX)

---

## Overview

This project implements a **dynamic module loading (overlay) system** to overcome **limited internal flash memory** on Cortex-M0 based prepaid meters.

Optional features are stored in **external flash** and loaded **on demand** into a small **internal flash execution buffer**.

Only the active module occupies internal flash at runtime.

---

## Problem

Traditional firmware links all features permanently:

Internal Flash =
Core + Keypad + Display + Bootloader + Diagnostics + ...

Problems:
- Flash exhausted quickly
- Rarely-used features waste memory
- Hard to scale
- Requires larger MCU

---

## Solution

Store optional features in **external flash** and load only one module at runtime.

Principle:

Internal flash = core + largest module  
NOT  
Internal flash = core + sum(all modules)

---

# Architecture

## Internal Flash (execution)

```
0x00000000  +-------------------------------+
            | Core firmware                 |
            | loader + drivers + stubs      |
            +-------------------------------+
0x0003C000  | Module execution buffer       |
            | (copied here at runtime)      |
0x0003FFFF  +-------------------------------+
```

---

## External Flash (storage only)

```
Slot N:

+------------------+
| header           |
| module code      |
| function table   |
+------------------+
```

External flash is **never executed directly**.

---

# Runtime Flow

```
Application call
      ↓
Stub function
      ↓
Loader checks module
      ↓
If not loaded:
    - read header
    - verify CRC
    - erase execution buffer
    - copy code to internal flash
    - build function pointers
      ↓
Jump to real function
```

---

# Key Design Choices

## 1. Execute from internal flash (not RAM)

Reasons:
- RAM is limited
- Flash execution is stable
- No relocation needed
- Deterministic timing

---

## 2. Link modules at execution address

Modules are linked at:

0x0003C000

So:

link address == execution address

Benefits:
- No relocation
- Absolute addresses valid
- Simpler loader
- Faster loading

Loader only needs:

copy → run

---

## 3. Stub Functions (important)

### Problem
Calling a module directly would crash if not loaded.

### Solution
Provide stub wrappers inside main firmware.

Flow:

```
Application
    ↓
Stub
    ↓
Load module if needed
    ↓
Call real function
```

### Example

```c
void Keypad_Init(void)
{
    if (!loader_is_loaded(MODULE_KEYPAD))
        loader_load_module(MODULE_KEYPAD);

    ((void(*)(void))loader_get_function(MODULE_KEYPAD, 0))();
}
```

Benefits:
- Transparent to application
- Automatic loading
- Safe
- Clean API

---

# Module Format

## External Flash Layout

```
[ header ][ code ][ function table ]
```

---

## Header (12 bytes)

```c
typedef struct {
    uint16_t magic;
    uint16_t code_size;
    uint16_t crc16;
    uint8_t  func_count;
    uint8_t  module_id;
    uint32_t reserved;
} module_header_t;
```

---

## Function Table

```c
typedef struct {
    uint32_t addr;
} func_entry_t;
```

Absolute addresses are used because modules are linked at the execution address.

---

# Build Flow

## Step 1 — Build module

Produces:

module.bin

Contains:
code + function table only

(No header inside C)

---

## Step 2 — Post-build script

Location:

Module/Module/Compiler/Debug/Exe/make_module_image.py

Script:
- calculates code_size
- calculates CRC16
- sets metadata
- prepends header

Produces:

module_image.bin

---

## Step 3 — Flash

Program:

module_image.bin

to external flash.

Loader handles everything automatically.

---

# Adding a New Module

1. Put functions in `.module_code`
2. Create function table
3. Build
4. Run script
5. Flash image
6. Add stubs

Done.

---

# Memory Saving Principle

Without loader:

Flash = core + all modules

With loader:

Flash = core + largest module only

---

## Example

Without:
Core 80 KB + Modules 60 KB = 140 KB

With:
Core 80 KB + Largest 20 KB = 100 KB

Saved: 40 KB

---

# Advantages

- Reduces internal flash usage
- Scalable
- Supports many optional features
- No RAM execution
- No relocation
- Simple design
- Deterministic behavior

---

# Limitations

- Only one module active at a time
- Small load latency when switching
- Requires external flash

---

# Project Structure

```
loader.c / loader.h        → module loader
stubs/                    → stub wrappers
modules/                  → module sources (code only)
make_module_image.py      → header injection script
```

---

# Status

Implemented:
- loader
- CRC verification
- stub system
- post-build header injection
- module execution from flash

Ready for hardware validation.
