# dwm3001_cdk_header.kicad_sch
## Purpose
Provide the mating connector and pin mapping for the DWM3001CDK J10 header for the Thin-Pod analogue path.

The Thin-Pod bench note uses J10.15 as the ADC input. fileciteturn16file9

## Sheet pins
Inputs
- `GND`
- `ADC_IN0`

Optional inputs
- `RESET_N`

Optional (bench-only)
- `CDK_5V0`

## Connector
- `JCDK1` 2×13 socket header (mates to CDK J10)

## Minimum pin mapping for Thin-Pod v0.1
| J10 pin | Carrier net | Use |
|--------:|-------------|-----|
| 6       | `GND`       | Ground reference for ADC |
| 15      | `ADC_IN0`   | ADC input (sensor after RC) |
| 12      | `RESET_N`   | Optional reset |

Optional bench-only:
| J10 pin | Carrier net |
|--------:|-------------|
| 2       | `CDK_5V0`   |
| 4       | `CDK_5V0`   |

Optional extra grounds: 9 / 14 / 20 / 25 can also be tied to `GND`.
