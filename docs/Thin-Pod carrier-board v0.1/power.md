# power.kicad_sch
## Purpose
Accept `RAW_IN`, protect it, generate `3V3_SENSOR` via the S7V8F3 module footprint, and optionally forward a supply to the DWM3001-CDK J10 power pins (2/4) via `CDK_5V0`.

## Sheet pins
Inputs
- `RAW_IN`
- `GND`

Outputs
- `RAW_IN_PROT`
- `3V3_SENSOR`
- `3V3_A`
- `CDK_5V0`

## Connectors
- `JBAT1` JST-PH-2 (right angle)
  - 1: `RAW_IN`
  - 2: `GND`
- `JBENCH1` optional 1×02 2.54 mm header or 2-pin terminal
  - 1: `RAW_IN`
  - 2: `GND`

## Protection and distribution
- `D1` Schottky diode, series from `RAW_IN` to `RAW_IN_PROT`
- `CIN1` bulk capacitor from `RAW_IN_PROT` to `GND`
- `TP_RAW_IN` on `RAW_IN`
- `TP_RAW_IN_PROT` on `RAW_IN_PROT`
- `TP_GND` on `GND`

## S7V8F3 module footprint (pluggable module)
- `JREG1` 1×03 header matching S7V8F3 pins
  - 1: `RAW_IN_PROT` (VIN)
  - 2: `GND`
  - 3: `3V3_SENSOR` (VOUT)

Rail decoupling on carrier
- `COUT1` bulk capacitor `3V3_SENSOR` to `GND`
- `COUT2` 100 nF `3V3_SENSOR` to `GND`
- `TP_3V3_SENSOR` on `3V3_SENSOR`

## Optional analogue split
- `FB1` ferrite bead footprint from `3V3_SENSOR` to `3V3_A` (DNP default)
- `R0A` 0 Ω link footprint as alternative to `FB1` (one of the two populated)
- `CA1` bulk capacitor `3V3_A` to `GND`
- `CA2` 100 nF `3V3_A` to `GND`
- `TP_3V3_A` on `3V3_A`

## Optional CDK header power feed
- `SJ1` solder jumper from `RAW_IN_PROT` to `CDK_5V0` (open by default)
- `TP_CDK_5V0` on `CDK_5V0`

**Note:** Only close `SJ1` when `RAW_IN_PROT` is a valid supply for the CDK J10 5V0 pins (for example, a bench 5 V rail). Leave open when powering the CDK by USB or its battery connector.
