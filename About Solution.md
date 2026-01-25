## Application Context: Prepaid Electric Meter

This project targets a **prepaid electric meter**, a constrained embedded system that must support a wide range of features while operating on limited internal flash memory.

Prepaid meters typically include both **time-critical functions** and **administrative or maintenance functions** that are used infrequently but still consume significant firmware space.

---

### Typical Functional Categories

The firmware functionality can be broadly classified into two categories:

#### Always-Used Functions
These functions must be available continuously during normal meter operation:

- Energy measurement and accumulation
- Tariff calculation
- Credit management
- Real-time clock handling
- Display of consumption and balance
- Basic user input

These functions are part of the **core firmware** and remain permanently stored in internal flash.

---

#### Rarely-Used Functions
These functions are required only under specific conditions, such as maintenance, configuration, or updates:

- Bootloader / firmware update routines
- RFID card handling
- Meter configuration and calibration
- Diagnostics and factory test modes
- Communication setup and service menus
- Regulatory or inspection features

Although these features are critical, they are **invoked infrequently** during the meter’s lifetime.

---

### The Memory Challenge

In a traditional monolithic firmware design:

- Rarely-used features permanently occupy internal flash
- Internal flash grows with every added feature
- The 256 KB internal flash becomes the primary scalability bottleneck

This is particularly inefficient in a prepaid meter, where some features may be used only a few times over years of operation.

---

### Architectural Decision

To address this inefficiency, this project adopts a **dynamic module loading approach**:

- Core meter functionality remains resident in internal flash
- Rarely-used features are compiled as standalone modules
- Modules are stored in external flash (4 MB)
- Modules are loaded into internal flash only when required
- After use, the execution window can be reused by another module

Examples of offloaded modules include:

- Bootloader / firmware update logic
- RFID card processing
- Factory calibration tools
- Service and diagnostic interfaces

---

### Why This Approach Fits Prepaid Meters

This architecture is particularly well suited for prepaid electric meters because:

- Internal flash is preserved for always-critical functionality
- External flash provides ample space for feature expansion
- Infrequently used code does not permanently consume internal memory
- New service features can be added without impacting core firmware size
- Field updates can target individual modules rather than the entire firmware

---

### Operational Behavior

From a system perspective:

- The meter operates normally using core firmware
- When a rare feature is needed (e.g., RFID transaction or firmware update):
  - The corresponding module is loaded from external flash
  - Executed from the internal flash execution window
  - Unloaded or replaced after completion

This behavior is transparent to the rest of the system and does not affect normal meter operation.

---

### Summary

By separating always-used and rarely-used functionality, this design:

- Optimizes internal flash usage
- Improves system scalability
- Aligns memory usage with real operational behavior
- Enables feature-rich prepaid meters on constrained hardware

This context drives the architectural choices made in this project and explains the motivation behind dynamic module loading.
