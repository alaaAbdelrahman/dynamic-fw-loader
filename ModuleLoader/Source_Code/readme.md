# Module Loader - Complete Line-by-Line Explanation

## Table of Contents
1. [Overview & Architecture](#overview)
2. [Data Structures](#data-structures)
3. [Utility Functions](#utility-functions)
4. [Flash Operations](#flash-operations)
5. [Core Loader Functions](#core-loader-functions)
6. [Complete Flow Example](#complete-flow)

---

## Overview & Architecture {#overview}

### What the Loader Does

The loader is the **brain** of the module system. It:

```
┌─────────────────────────────────────────────────────────┐
│                    MODULE LOADER                        │
├─────────────────────────────────────────────────────────┤
│                                                         │
│  1. Reads module from external flash (FM25W32)         │
│  2. Validates integrity (CRC check)                    │
│  3. Erases internal flash execution buffer             │
│  4. Copies module to internal flash                     │
│  5. Builds function pointer table in RAM               │
│  6. Provides function pointers to stubs                │
│                                                         │
└─────────────────────────────────────────────────────────┘
```

### Memory Architecture

```
EXTERNAL FLASH (FM25W32 - 4MB):
┌──────────────────────────────────┐
│ 0x000000: Keypad Module Package  │ ← Permanent storage
│ 0x010000: Display Module Package │ ← Permanent storage
│ 0x020000: Printer Module Package │ ← Permanent storage
└──────────────────────────────────┘
         ↓ LOADER reads module
         ↓ verifies CRC
         ↓ writes to internal flash

INTERNAL FLASH (256KB):
┌──────────────────────────────────┐
│ 0x00000000: Main Application     │ ← Your code + loader
│ 0x0003C000: Module Exec Buffer   │ ← One module loaded here
└──────────────────────────────────┘
         ↑
         └─ CPU executes from here

RAM:
┌──────────────────────────────────┐
│ g_loader state (34 bytes)        │
│  - current_module: which loaded  │
│  - func_count: how many funcs    │
│  - funcs[8]: function pointers   │
└──────────────────────────────────┘
         ↑
         └─ Stubs use these pointers
```

---

## Data Structures {#data-structures}

### 1. Module Header (12 bytes)

```c
typedef struct {
    uint16_t magic;          // Offset 0-1:  0x4D4D
    uint16_t code_size;      // Offset 2-3:  How many bytes of code
    uint16_t crc16;          // Offset 4-5:  Integrity check
    uint8_t  func_count;     // Offset 6:    Number of functions
    uint8_t  module_id;      // Offset 7:    Which module (0-255)
    uint32_t reserved;       // Offset 8-11: Future use
} module_header_t;
```

**Why each field exists:**

| Field | Purpose | Example | What happens if wrong? |
|-------|---------|---------|----------------------|
| `magic` | Validate this is a module | 0x4D4D | Loader rejects garbage data |
| `code_size` | Know how much to read | 456 bytes | Read wrong amount → crash |
| `crc16` | Detect corruption | 0xABCD | Corrupted code detected → safe |
| `func_count` | How many functions | 3 | Wrong count → crash on call |
| `module_id` | Verify correct module | 0 = Keypad | Load display instead of keypad → chaos |
| `reserved` | Future features | 0 | Allow adding features later |

**Memory layout in external flash:**

```
Address    Bytes         Field
────────────────────────────────────
0x000000:  4D 4D         magic (little-endian)
0x000002:  C8 01         code_size = 456 (0x01C8 in LE)
0x000004:  CD AB         crc16 = 0xABCD
0x000006:  03            func_count = 3
0x000007:  00            module_id = 0
0x000008:  00 00 00 00   reserved = 0
```

---

### 2. Function Entry (4 bytes)

```c
typedef struct {
    uint16_t func_id;        // Which function (0, 1, 2...)
    uint16_t offset;         // Bytes from module start
} func_entry_t;
```

**Example:**

```
Keypad module has 3 functions:
┌──────────────────────────────────────┐
│ func_id=0, offset=0x0000  Keypad_Init    │
│ func_id=1, offset=0x0048  Keypad_Scan    │
│ func_id=2, offset=0x00B4  Keypad_GetKey  │
└──────────────────────────────────────┘

In memory (12 bytes total):
00 00 00 00  ← func 0 at offset 0
01 00 48 00  ← func 1 at offset 0x48
02 00 B4 00  ← func 2 at offset 0xB4
```

**How loader uses this:**

```c
// When stub calls loader_get_function(MODULE_KEYPAD, 1):
// 1. Look up entry: func_id=1 → offset=0x0048
// 2. Calculate address: 0x08020000 + 0x0048 = 0x08020048
// 3. Set Thumb bit: 0x08020048 | 0x01 = 0x08020049
// 4. Return: 0x08020049 (function pointer to Keypad_Scan)
```

---

### 3. Loader State (34 bytes in RAM)

```c
static struct {
    module_id_t current;     // 1 byte:  Which module loaded (0-255, 0xFF=none)
    uint8_t func_count;      // 1 byte:  How many functions in current module
    void* funcs[8];          // 32 bytes: Function pointers (8 × 4 bytes)
} g_loader;
```

**Example state after loading Keypad:**

```
g_loader.current = 0           (MODULE_KEYPAD)
g_loader.func_count = 3        (Keypad has 3 functions)
g_loader.funcs[0] = 0x08020001 (Keypad_Init with Thumb bit)
g_loader.funcs[1] = 0x08020049 (Keypad_Scan with Thumb bit)
g_loader.funcs[2] = 0x080200B5 (Keypad_GetKey with Thumb bit)
g_loader.funcs[3] = NULL
g_loader.funcs[4] = NULL
... (rest are NULL)
```

**Why only 34 bytes?**

```
current:     1 byte
func_count:  1 byte
funcs[8]:    8 × 4 = 32 bytes
─────────────────────────
Total:       34 bytes
```

This is **shared** across all modules! When you load a different module, this gets overwritten.

---

### 4. Module Address Registry

```c
static const uint32_t module_addresses[MODULE_COUNT] = {
    0x000000,  // MODULE_KEYPAD   at 0x000000
    0x010000,  // MODULE_DISPLAY  at 0x010000
    0x020000   // MODULE_PRINTER  at 0x020000
};
```

**Why this exists:**

Quick lookup table: "Where is module X stored in external flash?"

```c
// When loading MODULE_DISPLAY (id=1):
uint32_t addr = module_addresses[1];  // Gets 0x010000
// Now read from external flash at 0x010000
```

**This lives in internal flash (not RAM)** - it's constant data.

---

## Utility Functions {#utility-functions}

### 1. CRC16 Calculation

```c
static uint16_t crc16(const uint8_t *data, uint32_t len)
{
    uint16_t crc = 0xFFFF;  // Start with all 1s
    
    for (uint32_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i] << 8;
        
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x8000)  // Check leftmost bit
                crc = (crc << 1) ^ 0x1021;  // Shift and XOR with polynomial
            else
                crc = crc << 1;  // Just shift
            crc &= 0xFFFF;  // Keep only 16 bits
        }
    }
    return crc;
}
```

**Step-by-step for one byte (0x48):**

```
Initial:  crc = 0xFFFF

Step 1: XOR with byte shifted left 8
        crc = 0xFFFF ^ (0x48 << 8)
        crc = 0xFFFF ^ 0x4800
        crc = 0xB7FF

Step 2: Process 8 bits
   Bit 0: crc = 0xB7FF, MSB is 1 (0xB is 1011)
          crc = (0xB7FF << 1) ^ 0x1021
          crc = 0x6FFE ^ 0x1021 = 0x7FDF
   
   Bit 1: crc = 0x7FDF, MSB is 0
          crc = 0x7FDF << 1 = 0xFFBE
   
   ... continue for 8 bits total

Final: crc = (some value)
```

**Why 0x1021?**

This is the CRC16-CCITT polynomial. It's mathematically proven to detect:
- All single-bit errors
- All double-bit errors
- All errors in bursts up to 16 bits
- 99.998% of all other error patterns

---

### 2. Address Conversion

```c
static void addr_to_flash(uint8_t *out, uint32_t addr)
{
    out[0] = (uint8_t)(addr);        // LSB
    out[1] = (uint8_t)(addr >> 8);   // Middle
    out[2] = (uint8_t)(addr >> 16);  // MSB
}
```

**Example: Convert 0x012345**

```
Input: addr = 0x00012345 (32-bit)

Step 1: out[0] = (uint8_t)(0x00012345)
        Casting to uint8_t takes only lowest 8 bits
        out[0] = 0x45

Step 2: out[1] = (uint8_t)(0x00012345 >> 8)
        0x00012345 >> 8 = 0x00000123
        out[1] = 0x23

Step 3: out[2] = (uint8_t)(0x00012345 >> 16)
        0x00012345 >> 16 = 0x00000001
        out[2] = 0x01

Result: out[] = {0x45, 0x23, 0x01}
        This is sent to FM25W32 as 3-byte address
```

**Why this byte order (little-endian)?**

FM25W32 expects address bytes in this order for SPI commands:
```
Send: [READ_CMD] [0x01] [0x23] [0x45] → Read from 0x012345
```

---

## Flash Operations {#flash-operations}

### 1. Erase Internal Flash Sector

```c
static loader_status_t erase_sector(uint32_t addr)
{
    // Call hardware erase function
    FLASH_SectorErase(addr);
    
    // Verify erase: all bytes should be 0xFF
    volatile uint8_t *p = (volatile uint8_t*)addr;
    for (uint32_t i = 0; i < FLASH_SECTOR_SIZE; i++) {
        if (p[i] != 0xFF)
            return LOADER_ERR_FLASH_WRITE;
    }
    
    return LOADER_OK;
}
```

**Why we verify:**

```
Scenario 1: Erase successful
────────────────────────────
Before: 01 23 45 67 89 AB ...
After:  FF FF FF FF FF FF ...  ✓ All 0xFF

Scenario 2: Erase failed
────────────────────────────
Before: 01 23 45 67 89 AB ...
After:  FF 23 FF 67 FF AB ...  ✗ Some bytes not erased!
Loader detects this → returns error → safe
```

**Why volatile?**

```c
volatile uint8_t *p = (volatile uint8_t*)addr;
```

`volatile` tells compiler: "This memory might change without my code changing it (hardware is involved), so always read from actual memory, don't optimize/cache it."

Without `volatile`:
```c
// Compiler might optimize this loop away!
for (i = 0; i < 100; i++) {
    if (p[i] != 0xFF) error();
}
// Compiler thinks: "p doesn't change in loop, skip it!"
```

With `volatile`:
```c
// Compiler MUST read from memory every time
for (i = 0; i < 100; i++) {
    if (p[i] != 0xFF) error();  // Actually reads memory
}
```

---

### 2. Write to Internal Flash

```c
static loader_status_t write_flash(uint32_t addr, const uint8_t *data, uint32_t len)
{
    // Call hardware write function
    FLASH_ProgramByte(addr, (uint8_t*)data, len);
    
    // Wait for write to complete
    if (FLASH_GetWriteStatus() != FLASH_WSTA_FINISH)
        return LOADER_ERR_FLASH_WRITE;
    
    // Verify write
    volatile uint8_t *p = (volatile uint8_t*)addr;
    for (uint32_t i = 0; i < len; i++) {
        if (p[i] != data[i])
            return LOADER_ERR_FLASH_WRITE;
    }
    
    return LOADER_OK;
}
```

**Step-by-step:**

```
1. Start write operation
   ──────────────────────
   FLASH_ProgramByte(0x08020000, [0xB5, 0x80, ...], 256);
   Hardware starts writing...

2. Wait for completion
   ──────────────────────
   while (FLASH_GetWriteStatus() != FINISH) {
       // Hardware is still writing
   }
   Hardware finished!

3. Verify data
   ──────────────────────
   Read back: 0xB5, 0x80, ...
   Compare with original
   If match: ✓ Success
   If different: ✗ Error (maybe flash worn out, voltage drop, etc.)
```

**Why verify?**

Flash writes can fail silently:
- Flash cell worn out (too many erases)
- Power brownout during write
- Cosmic ray bit flip (rare but possible)

Verification catches these before executing corrupted code!

---

### 3. Read from External Flash

```c
static loader_status_t read_ext_flash(uint32_t addr, uint8_t *data, uint32_t len)
{
    uint8_t flash_addr[3];
    addr_to_flash(flash_addr, addr);  // Convert to 3-byte format
    
    uint8_t retry = 3;
    while (retry--) {
        if (FLASH_gReadPage(flash_addr, data, len) == 1)
            return LOADER_OK;  // Success!
    }
    
    return LOADER_ERR_FLASH_READ;  // All retries failed
}
```

**Why retry 3 times?**

SPI communication can fail occasionally:
- Noise on SPI bus
- Timing issues
- EMI interference

Retrying increases reliability:
```
Success rate per attempt: 99.9%
Success rate with 3 retries: 99.9999%
```

**SPI communication example:**

```
CPU → FM25W32:
───────────────────────────────────────
1. Send READ command (0x03)
2. Send address (0x01, 0x23, 0x45)
3. Read data bytes (as many as needed)

Timeline:
CPU:    [CMD] [0x01] [0x23] [0x45] [read] [read] [read]...
        │     │      │      │      └──────┴──────┴─ Data from flash
        └─────┴──────┴──────┴────── Sent by CPU

FM25W32: [wait] [wait] [wait] [wait] [0xB5] [0x80] [0xAF]...
                                      └──────┴──────┴─ Sends data
```

---

## Core Loader Functions {#core-loader-functions}

### 1. Initialization

```c
loader_status_t loader_init(void)
{
    // Initialize internal flash driver
    FLASH_Init(FLASH_CSMODE_DISABLE);
    FLASH_CycleInit();
    
    // Initialize external flash driver
    if (FLASH_gInit() != 1)
        return LOADER_ERR_FLASH_READ;
    
    // Clear loader state
    g_loader.current = MODULE_COUNT;  // 0xFF = none loaded
    g_loader.func_count = 0;
    memset(g_loader.funcs, 0, sizeof(g_loader.funcs));
    
    return LOADER_OK;
}
```

**What happens:**

```
1. Initialize internal flash controller
   ────────────────────────────────────
   - Set up timing registers
   - Configure write protection
   - Enable flash controller

2. Initialize external SPI flash
   ────────────────────────────────────
   - Initialize SPI peripheral
   - Send RESET command to FM25W32
   - Read flash ID to verify communication
   - If ID correct → flash is working

3. Clear loader state
   ────────────────────────────────────
   RAM before:  [garbage data]
   RAM after:   current=0xFF, func_count=0, funcs={NULL, NULL, ...}
```

**Call this ONCE at system startup!**

```c
int main(void) {
    // Hardware init
    SystemInit();
    
    // Initialize loader - FIRST!
    if (loader_init() != LOADER_OK) {
        // Handle error
        while(1);
    }
    
    // Now you can use modules
    Keypad_Init();
}
```

---

### 2. Load Module - THE BIG ONE!

This is the heart of the loader. Let me break it down into clear sections:

```c
loader_status_t loader_load_module(module_id_t module_id)
{
    // ... full implementation ...
}
```

#### Section 1: Validation

```c
// Validate module ID
if (module_id >= MODULE_COUNT)
    return LOADER_ERR_INVALID_MODULE;

// Already loaded? Fast return!
if (g_loader.current == module_id)
    return LOADER_OK;
```

**Why check if already loaded?**

```
Scenario: Keypad module already loaded
─────────────────────────────────────────
Call: Keypad_Scan()  → loader_load_module(MODULE_KEYPAD)
Check: g_loader.current == MODULE_KEYPAD? YES!
Action: Return immediately (0 overhead!)

Time saved: ~250ms (no reloading needed)
```

#### Section 2: Read and Validate Header

```c
// Get external flash address
uint32_t ext_addr = module_addresses[module_id];

// Read header (12 bytes)
module_header_t hdr;
status = read_ext_flash(ext_addr, (uint8_t*)&hdr, sizeof(hdr));

// Validate magic
if (hdr.magic != 0x4D4D)
    return LOADER_ERR_BAD_MAGIC;

// Validate module ID matches
if (hdr.module_id != module_id)
    return LOADER_ERR_INVALID_MODULE;

// Validate size
if (hdr.code_size == 0 || hdr.code_size > 16384)
    return LOADER_ERR_TOO_LARGE;
```

**What this prevents:**

| Check | Prevents | Example |
|-------|----------|---------|
| Magic | Loading garbage | External flash empty → 0xFFFF not 0x4D4D |
| Module ID | Wrong module | Requested keypad, but display data at that address |
| Size | Buffer overflow | Module claims 50KB but buffer is only 16KB |

#### Section 3: Erase Execution Buffer

```c
// Calculate sectors needed
uint32_t sectors = (hdr.code_size + FLASH_SECTOR_SIZE - 1) / FLASH_SECTOR_SIZE;

// Erase each sector
for (uint32_t i = 0; i < sectors; i++) {
    status = erase_sector(MODULE_EXEC_BUFFER + i * FLASH_SECTOR_SIZE);
    if (status != LOADER_OK)
        return status;
}
```

**Example calculation:**

```
Code size: 3000 bytes
Sector size: 2048 bytes

Sectors needed = (3000 + 2047) / 2048
               = 5047 / 2048
               = 2 sectors (integer division)

Erase:
  Sector 0: 0x08020000 - 0x080207FF (2KB)
  Sector 1: 0x08020800 - 0x08020FFF (2KB)
  Total:    4KB erased for 3KB of code
```

**Why round up?**

```
If we only erased 1 sector:
─────────────────────────────
Sector 0: ERASED (2KB)
Sector 1: OLD DATA (2KB)

Write 3000 bytes:
  Bytes 0-2047:    Written to sector 0 ✓
  Bytes 2048-2999: Written to sector 1 ✗ (not erased!)
  
Result: Corrupted code → CRASH!
```

#### Section 4: Read Code from External Flash

```c
// Allocate temporary buffer
uint8_t *code = (uint8_t*)malloc(hdr.code_size);
if (!code)
    return LOADER_ERR_NO_MEMORY;

// Read code
uint32_t code_addr = ext_addr + sizeof(module_header_t);
status = read_ext_flash(code_addr, code, hdr.code_size);
if (status != LOADER_OK) {
    free(code);
    return status;
}
```

**Memory map during this:**

```
EXTERNAL FLASH:
┌──────────────┐ 0x000000
│ Header (12B) │
├──────────────┤ 0x00000C ← code_addr
│ Code (456B)  │ ← Read from here
├──────────────┤
│ Func Table   │
└──────────────┘

RAM:
┌──────────────┐
│ malloc'd     │ ← code buffer (456 bytes)
│ buffer       │
└──────────────┘
```

**Why use malloc?**

Could we use a static buffer instead?

```c
// Option 1: malloc (current approach)
uint8_t *code = malloc(hdr.code_size);  // Exactly what we need
// Pros: No waste, flexible
// Cons: Need enough heap

// Option 2: Static buffer
static uint8_t code[16384];  // Always 16KB
// Pros: No malloc needed
// Cons: Wastes RAM (always uses 16KB even for 500-byte module)
```

For small modules (< 2KB), malloc saves 14KB of RAM!

#### Section 5: Verify CRC

```c
// Calculate CRC of code we just read
uint16_t calc_crc = crc16(code, hdr.code_size);

// Compare with stored CRC
if (calc_crc != hdr.crc16) {
    free(code);
    return LOADER_ERR_CRC_FAIL;
}
```

**Critical safety check:**

```
Scenario: Bit flip in external flash
─────────────────────────────────────────
Original:     0xB580 0xAF00 0x....
Corrupted:    0xB590 0xAF00 0x....  (bit 4 flipped!)

Original CRC:    0xABCD
Calculated CRC:  0x1234 (completely different!)

Loader detects mismatch → REFUSES to load
System stays safe, doesn't execute corrupted code!
```

Without CRC check:
```
Corrupted code loaded → CPU tries to execute
→ Invalid instruction → HARDFAULT
→ System crash/hang
```

#### Section 6: Write to Internal Flash

```c
uint32_t written = 0;
while (written < hdr.code_size) {
    uint32_t chunk = (hdr.code_size - written > 256) ? 
                     256 : (hdr.code_size - written);
    
    status = write_flash(MODULE_EXEC_BUFFER + written, 
                        &code[written], chunk);
    if (status != LOADER_OK) {
        free(code);
        return status;
    }
    
    written += chunk;
}

free(code);  // Done with temporary buffer
```

**Why 256-byte chunks?**

```
Writing 3000 bytes:

Chunk 1:  Write 256 bytes to 0x08020000
          Verify write successful
          
Chunk 2:  Write 256 bytes to 0x08020100
          Verify write successful
          
...

Chunk 12: Write 208 bytes to 0x08020B00 (last chunk)
          Verify write successful

Total: 12 write operations
```

Benefits of chunking:
1. **Detect errors early:** If chunk 3 fails, we know immediately
2. **Progress tracking:** Could add progress bar
3. **Smaller RAM buffer:** Could process in-place without full malloc

#### Section 7: Read Function Table

```c
// Function table location: after header and code
uint32_t func_table_addr = ext_addr + sizeof(module_header_t) + hdr.code_size;

// Allocate space for function entries
func_entry_t *funcs = (func_entry_t*)malloc(hdr.func_count * sizeof(func_entry_t));
if (!funcs)
    return LOADER_ERR_NO_MEMORY;

// Read function table
status = read_ext_flash(func_table_addr, (uint8_t*)funcs, 
                       hdr.func_count * sizeof(func_entry_t));
if (status != LOADER_OK) {
    free(funcs);
    return status;
}
```

**Memory layout:**

```
EXTERNAL FLASH:
┌─────────────────┐ 0x000000
│ Header (12B)    │
├─────────────────┤ 0x00000C
│ Code (456B)     │
├─────────────────┤ 0x0001D8 ← func_table_addr
│ Func Table:     │
│  Entry 0 (4B)   │  id=0, offset=0x0000
│  Entry 1 (4B)   │  id=1, offset=0x0048
│  Entry 2 (4B)   │  id=2, offset=0x00B4
└─────────────────┘
```

#### Section 8: Build Function Pointer Table

```c
for (uint8_t i = 0; i < hdr.func_count; i++) {
    // Calculate absolute address
    uint32_t func_addr = MODULE_EXEC_BUFFER + funcs[i].offset;
    
    // Set Thumb bit - CRITICAL!
    func_addr |= 0x01;
    
    // Store in loader state
    g_loader.funcs[i] = (void*)func_addr;
}

free(funcs);  // Done with function entries

// Update loader state
g_loader.current = module_id;
g_loader.func_count = hdr.func_count;
```

**Example:**

```
Function table from external flash:
─────────────────────────────────────
Entry 0: func_id=0, offset=0x0000
Entry 1: func_id=1, offset=0x0048
Entry 2: func_id=2, offset=0x00B4

Calculate addresses:
─────────────────────────────────────
func 0: 0x08020000 + 0x0000 = 0x08020000 → 0x08020001 (Thumb)
func 1: 0x08020000 + 0x0048 = 0x08020048 → 0x08020049 (Thumb)
func 2: 0x08020000 + 0x00B4 = 0x080200B4 → 0x080200B5 (Thumb)

Store in RAM:
─────────────────────────────────────
g_loader.funcs[0] = 0x08020001
g_loader.funcs[1] = 0x08020049
g_loader.funcs[2] = 0x080200B5
```

**Why the Thumb bit?**

```
ARM Cortex-M ALWAYS uses Thumb mode (16/32-bit mixed instructions)

Function pointer WITHOUT Thumb bit (0x08020048):
CPU: "This is ARM mode instruction"
     Tries to decode as 32-bit ARM instruction
     → UNDEFINED INSTRUCTION → HARDFAULT!

Function pointer WITH Thumb bit (0x08020049):
CPU: "This is Thumb mode instruction"
     LSB=1 tells CPU: use Thumb decoder
     → Instructions decoded correctly → Works!

Note: CPU ignores LSB when fetching, it's just a mode flag
      Branch to 0x08020049 → fetches from 0x08020048
```

---

### 3. Get Function Pointer

```c
void* loader_get_function(module_id_t module_id, uint16_t func_id)
{
    // Check if requested module is loaded
    if (g_loader.current != module_id)
        return NULL;
    
    // Check if function ID is valid
    if (func_id >= g_loader.func_count)
        return NULL;
    
    // Return function pointer
    return g_loader.funcs[func_id];
}
```

**Example usage from stub:**

```c
// In Keypad_Scan stub:
void (*func)(void) = loader_get_function(MODULE_KEYPAD, 1);
// Returns: 0x08020049 (pointer to Keypad_Scan in module)

if (func)
    func();  // Call the actual implementation
```

**What can go wrong:**