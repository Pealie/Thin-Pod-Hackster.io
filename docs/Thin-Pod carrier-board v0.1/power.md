# power.kicad_sch
## Purpose
Accept `RAW_IN` via JST-PH2, regulate to `3V3_SYS` using an S7V8F3 module, then split:

- `3V3_CDK` → CDK battery pads/connector (`+` / `-`)
- `3V3_A`   → ADXL1005 VDD

Optional: keep a bench-only `CDK_5V0` path to J10 pins 2/4 for cases where 3V3 on VBAT is not viable.

## Sheet pins
Inputs
- `RAW_IN`
- `GND`

Outputs
- `RAW_IN_PROT`
- `3V3_SYS`
- `3V3_CDK`
- `3V3_A`
- `CDK_5V0` (optional)

## Input connector
- `JBAT1` JST-PH-2 SMT right-angle
  - 1: `RAW_IN`
  - 2: `GND`

## Protection (recommended)
- `D1` Schottky diode, series from `RAW_IN` to `RAW_IN_PROT` (reverse polarity protection)
- `CIN1` bulk capacitor from `RAW_IN_PROT` to `GND`
- `TP_RAW_IN`, `TP_RAW_IN_PROT`, `TP_GND`

If `D1` drop is undesirable, replace with an ideal-diode PFET stage in a later revision.

## Regulator module header (S7V8F3)
- `JREG1` 1×03 header matching S7V8F3 pins
  - 1: `RAW_IN_PROT` (VIN)
  - 2: `GND`
  - 3: `3V3_SYS` (VOUT)

Decoupling near `3V3_SYS`
- `C3V3_1` bulk capacitor `3V3_SYS` to `GND`
- `C3V3_2` 100 nF `3V3_SYS` to `GND`
- `TP_3V3_SYS`

## 3V3 split (two branches, independently isolatable)
### Branch A: CDK supply
- `FB_CDK` ferrite bead footprint from `3V3_SYS` to `3V3_CDK` (populate as 0 Ω in v0.1 if desired)
- `JP_CDK_EN` 2-pin jumper in series with `3V3_CDK` (default closed)
- `C_CDK1` bulk capacitor from `3V3_CDK` to `GND` near the connector
- `C_CDK2` 100 nF from `3V3_CDK` to `GND`
- `JCDK_PWR1` 1×02 header for the flying lead to CDK pads
  - 1: `3V3_CDK` (to CDK `+`)
  - 2: `GND`     (to CDK `-`)
- `TP_3V3_CDK`

### Branch B: sensor supply
- `FB_A` ferrite bead footprint from `3V3_SYS` to `3V3_A` (populate as 0 Ω in v0.1 if desired)
- `JP_SENSOR_EN` 2-pin jumper in series with `3V3_A` (default closed)
- `C_A1` bulk capacitor from `3V3_A` to `GND`
- `C_A2` 100 nF from `3V3_A` to `GND`
- `TP_3V3_A`

## Optional bench-only: CDK J10 5V0 feed
Keep only as an escape hatch.
- `SJ5V` solder jumper from `RAW_IN_PROT` to `CDK_5V0` (open by default)
- `TP_CDK_5V0`

Only close `SJ5V` when `RAW_IN_PROT` is a verified 5 V rail.
