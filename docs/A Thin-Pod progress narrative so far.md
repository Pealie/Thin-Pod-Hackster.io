## Thin-Pod progress narrative for Hackster.io Best of 2025

Thin-Pod is a compact, UWB-linked vibration sensing node aimed at rotating machinery monitoring. The near-term goal is to prove a reliable end-to-end path from an analogue accelerometer output, through deterministic ADC sampling on a low-power UWB node, across a UWB link, into a gateway where heavier DSP and TinyML can live.

Work to date spans **23 December 2025 to 31 January 2026**, and is structured as a four-stage validation ladder so each subsystem is evidenced before moving up the stack. 

### What has been proven so far

**Stage 1: tooling, flashing, console, known-good firmware**
A repeatable flash and console workflow was stabilised for the UWB node using J-Link and the vendor SDK FreeRTOS binaries. Console and CLI behaviour was brought under control, and device identity plus SDK build versions were confirmed. 

**Stage 2: UWB link bring-up and ranging**
Two UWB boards were brought to a ‘known-good’ baseline where one device continuously receives frames while the other reports repeated successful ranging notifications. Bench sensitivity to cable routing and near-field conditions was recorded as a controlled variable for later measurements. 

**Stage 3: analogue sensor path, electrical validity, first dynamic evidence**
The analogue chain has been pinned down and bench-checked:

* **Sensor:** ADXL1005 analogue output shows a stable mid-rail bias at rest, with tap-correlated transients visible once probe pickup and scope settings were corrected. 
* **ADC boundary:** ADXL1005 Vout is routed through a **series 8 kΩ** resistor into an **ADC node** with **680 pF to ground** and a **1 MΩ pull-down**, then into the UWB module’s ADC input on **J10 pin 15**, mapped to **P0.28 (SAADC AIN4)**. 
* **Lesson learned:** an early loading issue traced to the 680 pF capacitor was isolated and corrected during diagnosis, then re-introduced in a controlled way as part of the defined conditioning network. 

Stage 3 is the current focus: the next proof point is SAADC window sampling and transport integrity.

**Stage 4: end-to-end pipeline, transport to gateway, DSP and TinyML**
A staged plan exists to validate (1) ADC correctness and timing, (2) UWB payload integrity with synthetic frames then real ADC windows, and (3) final-hop verification over SPI into the gateway MCU, including a minimal SPI master reader for deterministic extraction and CRC checking. 

---

## Hardware build and PCB progress

### Bench prototype to carrier PCB

A perfboard ‘biscuit board’ prototype consolidated the UWB node, regulator, and sensor into a more repeatable bench platform, reducing wiring variability ahead of a dedicated carrier PCB. 

### Carrier board v0.2 in KiCad

Thin-Pod has moved from wiring notes into a production-oriented schematic plan with explicit net naming and bring-up intent:

* **Power entry:** battery input plus optional bench input share `RAW_IN`, then a series **1N5817** creates `RAW_PROT` feeding the regulator VIN. 
* **Sensor power control:** a high-side PFET switched rail `ACC_3V3_SW` is defined for deep-off operation, with gate biasing and local decoupling. 
* **ADC conditioning:** `ACC_VOUT → 8 kΩ → ADC_NODE`, with **680 pF** and **1 MΩ** to ground, routed to `J10.15_(ADC_input)` and explicitly verified as isolated from the `3v3+` trunk. 
* **ERC hygiene:** an initial noisy ERC report was driven to **0 errors, 0 warnings**, leaving the schematic ready for PCB layout and DRC. 

### Mechanical integration evidence

Caliper-based measurements were captured for the UWB board envelope and antenna projection to guide keep-outs and docking geometry during layout. 

---

## Firmware and code direction

* **UWB node:** vendor SDK FreeRTOS baseline is proven for bring-up and RF activity, and the planned next increment is SAADC sampling on P0.28 with conservative acquisition time suitable for the 8 kΩ source impedance, using timer-triggered sampling into double-buffered windows. 
* **Gateway:** the gateway architecture is treated as the right home for heavier DSP and TinyML. Wiring has been simplified around a shared SPI bus with clear chip-select separation, and a minimal SPI reader test harness has been specified to validate final-hop data extraction before any higher-level ML work begins. 

---

## How this maps to the judging criteria

### Project documentation

A stepwise, evidence-first bring-up ladder is in place (Stages 1 to 4), with recorded outcomes at each stage, including what failed, why it failed, and what changed to fix it. The carrier-board work has been translated into KiCad-ready blocks with explicit nets, test points, and verification steps. 

### Complete BOM

The project BOM is already converging around a small, repeatable core: UWB node hardware, ADXL1005 analogue accelerometer, a 3.3 V regulator module, reverse-polarity diode, ADC conditioning passives, and optional PFET power gating. Tooling includes J-Link for flash/debug and oscilloscope plus DMM for analogue validation. 

### Schematics

Carrier-board v0.2 schematic intent is captured with named rails (`RAW_IN`, `RAW_PROT`, `3v3+`, `ACC_3V3_SW`, `ADC_NODE`) and the ADC path pinned to the specific header and MCU pin, with ERC brought to a clean state. 

### Code and contribution

Working baseline firmware for the UWB platform is proven, and the next code deliverables are narrowly defined: SAADC window capture on the node, payload transport tests, and a minimal SPI master extractor on the gateway to confirm integrity with CRC and deterministic logging. 

### Creativity

Thin-Pod’s ‘fresh take’ is the combination of UWB capabilities with vibration telemetry in a modular, test-friendly build strategy: CDK-based rapid iteration first, then a docking carrier PCB that hardens the analogue path, power intent, and mechanical repeatability before committing to solder-down module integration. 

---

## Immediate next milestones

1. Complete PCB layout and DRC for the carrier, then fabricate a first docking-ready revision. 
2. Implement SAADC window sampling on P0.28 and confirm timing plus amplitude sanity on captured buffers. 
3. Prove UWB transport integrity with synthetic payloads, then with real ADC windows and statistics matching. 
4. Validate final-hop SPI extraction into the gateway, then begin feature extraction and TinyML inference loops. 

