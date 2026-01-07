# dwm3001_cdk_header.kicad_sch
## Purpose
Provide the mating connector and pin mapping for the DWM3001-CDK J10 (26-pin header) and expose the minimal nets used by Thin-Pod v0.1.

## Sheet pins
Inputs
- `CDK_5V0`
- `GND`
- `ADC_IN0`

Optional inputs
- `RESET_N`

## Connector
- `JCDK1` 2×13 socket header (mates to CDK J10)

## J10 pin mapping table (Thin-Pod carrier v0.1)
This matches the bench-ready pinouts note.

| J10 pin | CDK label  | Carrier net | Thin-Pod use |
|--------:|------------|-------------|--------------|
| 2       | 5V0        | `CDK_5V0`    | Power in (optional via SJ1) |
| 4       | 5V0        | `CDK_5V0`    | Power in (optional via SJ1) |
| 6       | GND        | `GND`        | Ground return |
| 15      | GPIO_RPI   | `ADC_IN0`    | ADC input (sensor signal after RC) |
| 12      | RESET      | `RESET_N`    | Optional reset line |

Ground pins that can be tied to `GND` if a second return is desired
- 9, 14, 20, 25 (all GND options)

## Optional footprints (DNP by default)
- `R_ADC1` small series resistor on `ADC_IN0` near `JCDK1` (DNP if `R_SER1` is sufficient)
- `DCLAMP1` clamp diode footprint to `3V3_SENSOR` and `GND` (DNP default)
