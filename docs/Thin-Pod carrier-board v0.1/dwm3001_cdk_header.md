# DWM3001C-CDK header notes (Thin-Pod bench wiring)

This note records the specific DWM3001C-CDK connection points used for Thin-Pod bench wiring, with the intent of translating the same connections onto a carrier PCB.

## J1: external 3.3 V power entry (header holes)

Purpose: inject regulated 3V3_CDK directly into the CDK power domain.

- Physical: two through-holes labelled '+' and '-' on the CDK silkscreen.
- Usage: fit a 2-pin 2.54 mm header (or solder fly-leads) to the two holes.

### Net mapping

| CDK feature | Carrier net | Notes |
|---|---|---|
| J1 '+' | 3V3_CDK | Regulated 3.3 V from S7V8F3 (3V3_SYS branch) |
| J1 '-' | GND | Common ground (battery, CDK, ADXL1005) |

Bench validation: 3V3_CDK sourced from an S7V8F3 module into J1 boots the CDK and enables CLI and UWB operation.

## J10: analogue input header

Purpose: route ADXL1005 VOUT into the nRF SAADC input on the CDK.

- Signal: ADXL1005_VOUT (analogue)
- Entry: J10.15 on the CDK header

### Net mapping

| CDK header pin | Carrier net | Function |
|---|---|---|
| J10.15 | ADC_IN0 | SAADC input (mapped to nRF P0.28, AIN4) |

### Physical note

J1 and J10.15 are treated as header-accessible points in the carrier design, rather than soldering to small test pads.
