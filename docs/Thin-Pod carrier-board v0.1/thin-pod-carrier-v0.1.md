# Thin-Pod Option B carrier board v0.1 (updated from bench-ready pinouts)
This defines a bench-grade carrier PCB that removes breadboard wiring and makes the ADXL1005 analogue chain predictable, while keeping the DWM3001-CDK as the compute and radio core.

Source of truth for J10 usage and the Thin-Pod analogue path: the provided ‘Gateway and Thin-Pod simplified, bench-ready pinouts.md’.

## Scope
- Power entry via JST-PH2 and optional bench header
- S7V8F3 module footprint to generate a dedicated 3.3 V rail for the ADXL1005
- ADXL1005 placement with tight decoupling
- RC low-pass between `ACC_VOUT` and `ADC_IN0` matching the current bench wiring
- DWM3001-CDK J10 header mating connector and a minimal pin-map
- Test points for power and analogue nodes

## Non-goals
- No UWB RF redesign
- No integration of the DWM3001C module directly onto the carrier in v0.1
- No enclosure-optimised miniaturisation

## Sheet structure
- `power.kicad_sch`
- `sensor_frontend.kicad_sch`
- `dwm3001_cdk_header.kicad_sch`

## Net naming
Power and grounds
- `RAW_IN` (battery or bench input, pre-protection)
- `RAW_IN_PROT` (post reverse protection, pre distribution)
- `GND`
- `CDK_5V0` (power net delivered to CDK J10 pins 2/4, routed via a solder jumper)

Sensor rail
- `3V3_SENSOR` (S7V8F3 output for ADXL1005 VDD)
- `3V3_A` (optional analogue sub-rail, default linked from `3V3_SENSOR`)

Analogue chain
- `ACC_VOUT` (ADXL1005 output at sensor pin)
- `ADC_IN0` (post-filter node at the CDK header pin)

Test points
- `TP_RAW_IN`, `TP_RAW_IN_PROT`, `TP_CDK_5V0`
- `TP_3V3_SENSOR`, `TP_3V3_A`
- `TP_ACC_VOUT`, `TP_ADC_IN0`, `TP_GND`

## Default analogue population for v0.1 (bench-matching)
- `R_SER1 = 8.0 kΩ`
- `C_SHUNT1 = 680 pF`
- `C_PAR1` footprint present, DNP by default

This matches the current Thin-Pod note: ‘ADXL1005 Vout → RC low-pass (8 kΩ series, 680 pF to GND) → DWM3001-CDK J10.15’.
