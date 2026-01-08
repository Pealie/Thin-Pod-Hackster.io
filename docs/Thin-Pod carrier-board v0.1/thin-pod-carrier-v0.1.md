# Thin-Pod Option B carrier board v0.1
## Power architecture: single battery input, single buck, split 3V3 to CDK + ADXL1005

### Intent
A single JST-PH2 battery input feeds an S7V8F3 buck regulator. The buck output is a single system 3.3 V rail, then split into two *separately configurable* branches:

- `3V3_CDK` → DWM3001CDK battery input pads/connector (labelled `+` / `-` on the CDK silkscreen)
- `3V3_A` → ADXL1005 VDD

This matches the Thin-Pod bench note where the ADXL1005 VDD is a 3V3 rail and its analogue Vout is taken into the CDK ADC via J10.15.

### CDK power entry note
The DWM3001CDK is documented as being powerable from USB, Raspberry Pi interface, a battery, or an external power supply. The product brief calls out a 'Battery Connector' and shows `VBAT` feeding an on-board DC-DC to generate 3V3 rails.

Because the product brief does not publish a numeric `VBAT` range, treat '3V3 into VBAT' as a bench-validated assumption:
- If the CDK behaves normally (boots, stable current, stable 3V3 test point), keep it.
- If the CDK fails to start or is unstable, revert to powering the CDK via its supported 5V entry (USB or J10 5V0) and keep the S7V8F3 3V3 rail for the sensor only.

### Practical build convention (v0.1)
- Use a 2-wire flying lead from the carrier to the CDK `+` / `-` pads.
- Add an isolation element on the carrier (jumper or 0 Ω link) so the CDK can be disconnected when USB power is attached.

## Sheet structure
- `power.kicad_sch`
- `sensor_frontend.kicad_sch`
- `dwm3001_cdk_header.kicad_sch`

## Net naming
Power
- `RAW_IN`
- `RAW_IN_PROT`
- `GND`
- `3V3_SYS`
- `3V3_CDK`
- `3V3_A`

Analogue
- `ACC_VOUT`
- `ADC_IN0`

Optional bench-only power (kept off by default)
- `CDK_5V0`

## Default analogue population (bench-matching)
- `R_SER1 = 8.0 kΩ`
- `C_SHUNT1 = 680 pF`
- `C_PAR1` footprint present, DNP by default
