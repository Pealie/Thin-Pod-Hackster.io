# power.kicad_sch
## Purpose
Accept `RAW_IN` via JST-PH2 and distribute power as:
- raw feed to CDK battery pads (`VBAT_CDK`)
- regulated 3.3 V to the ADXL1005 (`3V3_A`) via S7V8F3
- optional bench-only 5 V feed to CDK J10 pins 2/4 (`CDK_5V0`) via solder-jumper

## Sheet pins
Inputs
- `RAW_IN`
- `GND`

Outputs
- `RAW_IN_PROT`
- `VBAT_CDK`
- `3V3_A`
- `CDK_5V0` (optional)

## Input connector
- `JBAT1` JST-PH-2 SMT right-angle
  - 1: `RAW_IN`
  - 2: `GND`

## Protection (shared)
- `D1` Schottky diode, series from `RAW_IN` to `RAW_IN_PROT`
- `CIN1` bulk capacitor from `RAW_IN_PROT` to `GND`
- `TP_RAW_IN` on `RAW_IN`
- `TP_RAW_IN_PROT` on `RAW_IN_PROT`
- `TP_GND` on `GND`

## Branch A: CDK battery pads feed
- `R_CDK0` 0 Ω link (or 2-pin jumper) from `RAW_IN_PROT` to `VBAT_CDK`
- `JCDK_PWR1` 1×02 header (or 2-pin JST) that routes to the CDK `+` and `-` pads via two wires
  - 1: `VBAT_CDK` (to CDK `+`)
  - 2: `GND`      (to CDK `-`)
- `TP_VBAT_CDK` on `VBAT_CDK`

## Branch B: Sensor regulated rail via S7V8F3
- `JREG1` 1×03 header matching S7V8F3 pins
  - 1: `RAW_IN_PROT` (VIN)
  - 2: `GND`
  - 3: `3V3_A` (VOUT)

Rail decoupling near `3V3_A`
- `CA1` bulk capacitor `3V3_A` to `GND`
- `CA2` 100 nF `3V3_A` to `GND`
- `TP_3V3_A` on `3V3_A`

Optional sensor enable
- `JP_SENSOR_EN` 2-pin jumper footprint in series with `3V3_A` feeding the sensor sheet (default closed)

## Optional bench-only: CDK J10 5V0 feed
- `SJ5V` solder jumper from `RAW_IN_PROT` to `CDK_5V0` (open by default)
- `TP_CDK_5V0` on `CDK_5V0`

Only close `SJ5V` when `RAW_IN_PROT` is a verified 5 V rail.
