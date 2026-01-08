# sensor_frontend.kicad_sch
## Purpose
Place ADXL1005 with local decoupling and implement the Thin-Pod RC low-pass from `ACC_VOUT` to `ADC_IN0`.

The Thin-Pod bench note sets:
- ADXL1005 VDD on a 3V3 rail
- ADXL1005 Vout → RC (8 kΩ series, 680 pF to GND) → CDK J10.15 (ADC input) fileciteturn16file9

## Sheet pins
Inputs
- `3V3_A`
- `GND`

Outputs
- `ACC_VOUT`
- `ADC_IN0`

## Sensor and decoupling
- `UACC1` ADXL1005
  - VDD: `3V3_A`
  - GND: `GND`
  - VOUT: `ACC_VOUT`

- `CACC1` 100 nF from `3V3_A` to `GND` (tight to `UACC1`)
- `CACC2` 1 µF to 10 µF from `3V3_A` to `GND` (near sensor)

## RC low-pass (bench-matching defaults)
- `R_SER1` from `ACC_VOUT` to `ADC_IN0` = 8.0 kΩ
- `C_SHUNT1` from `ADC_IN0` to `GND` = 680 pF
- `C_PAR1` parallel cap footprint (DNP)

## Test points
- `TP_ACC_VOUT` on `ACC_VOUT`
- `TP_ADC_IN0` on `ADC_IN0`
