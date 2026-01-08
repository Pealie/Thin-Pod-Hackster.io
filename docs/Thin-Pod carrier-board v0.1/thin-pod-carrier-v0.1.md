# Thin-Pod Option B carrier board v0.1
## Power + signal architecture (CDK battery pads, not J10 ‘3V3’)

### Key constraint
Although the DWM3001CDK J10 header is physically aligned to Raspberry Pi pins 1–26, J10 **pin 1** and **pin 17** are **NC** on the CDK.
So the usual Raspberry Pi ‘3V3 on pin 1/17’ assumption does not hold for this board.

### Practical v0.1 plan (single JST-PH2 input, battery-powered)
**JST-PH2 IN (RAW_IN) → split**
- **Branch A (CDK power):** `RAW_IN` → CDK **battery pads/connector** labelled `+` and `-` (two-wire feed).
- **Branch B (sensor power):** `RAW_IN` → S7V8F3 → `3V3_A` → ADXL1005 VDD.

**J10 stays signal + ground**
- ADXL1005 Vout → RC (8 kΩ series, 680 pF to GND) → CDK J10.15 (`ADC_IN0`)
- Ground reference to J10.6 (optionally also 9/14/20/25 for extra return)

### Optional bench mode (only if a verified 5 V rail exists)
If a stable 5 V rail is available, the carrier can optionally feed J10 pins 2/4 (5V0) via a solder jumper.
Keep this off for 3×AA unless deliberately verified.

## Sheet structure
- `power.kicad_sch`
- `sensor_frontend.kicad_sch`
- `dwm3001_cdk_header.kicad_sch`

## Net naming
Power
- `RAW_IN`
- `RAW_IN_PROT`
- `GND`
- `VBAT_CDK` (two-wire feed to CDK `+` pad)
- `3V3_A` (regulated sensor rail)

Analogue
- `ACC_VOUT`
- `ADC_IN0`

Optional bench-only
- `CDK_5V0`

## Default analogue population (bench-matching)
- `R_SER1 = 8.0 kΩ`
- `C_SHUNT1 = 680 pF`
- `C_PAR1` footprint present, DNP by default
