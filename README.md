# Dynamic Firmware Loading – Phase-1 Architecture and Achievements

## 1. Introduction

This document describes the design, implementation, and current achievements of a Phase-1 dynamic firmware loading system developed for a prepaid electric meter.

The goal of this phase was not feature completeness, but to establish a **correct, deterministic, and scalable foundation** that overcomes internal flash limitations while remaining compatible with a deeply embedded, safety-conscious environment.

---

## 2. Application Context

The target system is a prepaid electric meter with the following characteristics:

- Limited internal flash memory (256 KB)
- External flash available (4 MB)
- Long operational lifetime
- Strict reliability and determinism requirements
- A mix of always-used and rarely-used functionality

Examples of rarely-used functionality include:

- Bootloader and firmware update logic
- RFID card handling
- Factory calibration and diagnostics
- Service and maintenance features

In traditional monolithic firmware designs, these features permanently consume internal flash despite being used infrequently.

---

## 3. Problem Statement

The main challenges addressed in this work are:

1. Internal flash is too small to permanently hold all current and future features
2. Rarely-used functionality wastes valuable internal flash
3. Traditional dynamic loading approaches rely on Position Independent Code (PIC)
4. PIC introduces complexity, overhead, and toolchain challenges
5. Existing application code should remain unchanged to reduce risk

The objective was to solve these problems **without introducing runtime unpredictability**.

---

## 4. Core Architectural Idea

The central idea of the solution is:

- Store rarely-used functionality as standalone modules in external flash
- Load these modules into a reserved internal flash execution window only when needed
- Execute module code directly from internal flash
- Reuse the same execution window for different modules

Only one module is active at any given time.

This approach shifts firmware growth pressure from internal flash to external flash.

---

## 5. Avoiding Position Independent Code (PIC)

### 5.1 The PIC Challenge

Dynamic code execution is often implemented using PIC so that code can run at arbitrary memory locations. However, in a deeply embedded system, PIC causes:

- Larger code size
- Additional runtime overhead
- Complex relocation handling
- Reduced transparency during debugging
- Limited or non-intuitive support in some embedded toolchains (including IAR)

Given the safety and determinism requirements of a prepaid meter, PIC was deemed unsuitable for Phase-1.

---

### 5.2 The Chosen Alternative

Instead of PIC, this design uses **absolute addressing with a fixed execution window**:

- A fixed address range in internal flash is reserved for module execution
- All modules are linked as if they permanently reside at this address
- At runtime, the loader copies the module code to this exact address
- Link-time addresses and runtime execution addresses are identical

Because the CPU executes the code exactly where the linker expects it to be, no relocation or PIC support is required.

---

## 6. Loader Design and Responsibilities

The loader is part of the main firmware and performs the following tasks:

1. Read the module header from external flash
2. Validate module identity and integrity
3. Erase the internal flash execution window
4. Copy module executable code into the execution window
5. Read the module function table
6. Build absolute function pointers
7. Allow execution through a controlled interface

The loader ensures that execution is safe, bounded, and predictable.

---

## 7. Preserving the Main Firmware Codebase

### 7.1 The Challenge

A common drawback of modular systems is that they require widespread changes to application code, increasing maintenance effort and risk.

---

### 7.2 Stub-Based Solution

To avoid this, a stub-based access model is used:

- Application code calls normal-looking functions
- Stub functions transparently handle module loading
- Function signatures remain unchanged
- The application layer is unaware of dynamic loading

This design ensures:

- Zero behavioral change to existing application logic
- Clear separation of concerns
- Easy validation and regression testing

---

## 8. Phase-1 Constraints and Enforcement

To guarantee correctness and determinism, Phase-1 enforces strict constraints:

- Modules contain executable code only
- No `.data`, `.bss`, `.rodata`, or `.const` sections
- No global or static variables inside modules
- No heap usage by modules
- No interrupts implemented inside modules
- Single active module at a time

These constraints are enforced at **link time** using custom IAR linker scripts.

---

## 9. Memory Utilization Results

### 9.1 Internal Flash (256 KB)

Internal flash usage is now bounded and predictable:

- Core firmware and loader are permanently resident
- A fixed-size execution window is reserved (e.g., 16 KB)
- Internal flash usage does not grow with added features

---

### 9.2 External Flash (4 MB)

External flash stores:

- RFID module
- Bootloader module
- Diagnostics and service modules
- Future expansion features

This allows significant feature growth without internal flash pressure.

---

## 10. Concrete Use Case: RFID Transaction

During normal operation, the RFID module is not loaded.

When an RFID card is presented:

1. Core firmware detects card presence
2. RFID stub function is called
3. Loader loads the RFID module into the execution window
4. RFID logic executes from internal flash
5. Credit data is validated and passed to core firmware
6. Execution window becomes available again

RFID code occupies internal flash only during active use.

---

## 11. Toolchain Compatibility

The entire design is compatible with:

- IAR Embedded Workbench for ARM
- Standard IAR linker scripts
- Map-file–based address verification
- Cortex-M absolute addressing model

No ELF parsing, relocation records, or dynamic symbol resolution is required.

---

## 12. Achievements of Phase-1

Phase-1 successfully delivers:

- A working dynamic module loading mechanism
- Verified address correctness via linker map files
- Deterministic runtime behavior
- Minimal impact on existing firmware
- A scalable foundation for future phases

---

## 13. Scope and Future Work

Phase-1 intentionally limits functionality to establish correctness.

Future phases may introduce:

- Controlled read-only data support
- Module-local RAM
- Multiple module coexistence
- Versioning and integrity validation

These enhancements will build upon the stable Phase-1 foundation.

---

## 14. Conclusion

This Phase-1 implementation demonstrates that dynamic firmware loading can be achieved in a prepaid electric meter without PIC, relocation, or invasive code changes.

By aligning architectural decisions with real operational behavior, the system achieves scalability, safety, and determinism within tight embedded constraints.
