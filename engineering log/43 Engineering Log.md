## Engineering Log update: Gateway perfboard rig plan

### Summary

A concrete first-pass perfboard (‘biscuit board’) implementation plan has been defined for the Gateway, mirroring the Thin-Pod approach. The plan focuses on rigid mounting, shorter interconnects, predictable power distribution, and a mechanically stable wiring harness ahead of a dedicated PCB carrier.

### Key outputs

* Defined a Gateway perfboard v0.1 mechanical layout for the core modules:

  * STM32 NUCLEO-N657X0-Q (main MCU)
  * DWM3001C-CDK (UWB)
  * ESP32-C6 module (Wi-Fi backhaul)
* Established a clear interconnect strategy:

  * Shared SPI bus as a ‘bus bar’ on the perfboard, with separate chip-select lines per peripheral.
  * Dedicated control lines for interrupt and reset/enable signals to keep debugging deterministic.
* Locked down a bench-safe power distribution approach:

  * 5 V rail routed to the DWM3001C-CDK power entry.
  * 3.3 V rail routed to the ESP32-C6 supply.
  * Common ground strategy with optional additional ground returns where beneficial.
  * Local decoupling recommended near each consumer on the perfboard rails.
* Produced a minimal bring-up and checkout checklist:

  * Continuity and short checks for all nets.
  * Rail verification under load.
  * Reset/enable line behaviour verification.
  * Basic SPI sanity checks with chip-select isolation.

### Purpose and value

* Reduces breadboard wiring variability and intermittent faults during Gateway integration testing.
* Provides a repeatable, mechanically stable harness for validating end-to-end UWB ingest and Wi-Fi backhaul behaviour before committing to a PCB carrier spin.
