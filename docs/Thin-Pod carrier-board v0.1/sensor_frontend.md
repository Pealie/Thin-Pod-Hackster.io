# sensor_frontend.kicad_sch
## Purpose
Place ADXL1005 with tight local decoupling and implement the Thin-Pod RC low-pass from `ACC_VOUT` to `ADC_IN0`.

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

## RC low-pass (bench-matching)
- `R_SER1` from `ACC_VOUT` to `ADC_IN0` = 8.0 kΩ (default)
- `C_SHUNT1` from `ADC_IN0` to `GND` = 680 pF (default)
- `C_PAR1` parallel cap footprint (DNP default)

## Test points
- `TP_ACC_VOUT` on `ACC_VOUT`
- `TP_ADC_IN0` on `ADC_IN0`

## Layout note
Place `C_SHUNT1` close to the header-side node and give it a short ground via. Keep `ADC_IN0` short and away from any switching/power routing.
