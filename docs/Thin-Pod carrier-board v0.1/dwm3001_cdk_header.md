# dwm3001_cdk_header.kicad_sch
## Purpose
Provide the mating connector and pin mapping for the DWM3001CDK J10 header.

## J10 ‘Raspberry Pi’ alignment note
The J10 pins are aligned to Raspberry Pi pins 1–26 mechanically, but on the DWM3001CDK:
- Pin 1 is NC
- Pin 17 is NC

So J10 cannot be used as a ‘3V3 in’ on pin 1/17.

## Sheet pins
Inputs
- `GND`
- `ADC_IN0`
- `CDK_5V0` (optional, bench-only)

Optional inputs
- `RESET_N`

## Connector
- `JCDK1` 2×13 socket header (mates to CDK J10)

## J10 mapping (Thin-Pod v0.1)
Signal and ground used for Thin-Pod:
| J10 pin | CDK label | Carrier net | Use |
|--------:|----------|-------------|-----|
| 6       | GND      | `GND`       | Ground reference for ADC |
| 15      | GPIO_RPI | `ADC_IN0`   | ADC input (sensor after RC) |
| 12      | RESET    | `RESET_N`   | Optional reset |

Optional power (bench-only):
| J10 pin | CDK label | Carrier net |
|--------:|----------|-------------|
| 2       | 5V0      | `CDK_5V0`   |
| 4       | 5V0      | `CDK_5V0`   |

Optional extra grounds: 9 / 14 / 20 / 25 can also be tied to `GND`.
