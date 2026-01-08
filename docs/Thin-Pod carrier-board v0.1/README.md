# What changed in this revision
This pack implements the following architecture:

`JST-PH2 battery in → S7V8F3 buck → 3V3_SYS → split to (3V3_CDK + 3V3_A)`

- `3V3_CDK` feeds the DWM3001CDK via its bottom `+` / `-` battery pads/connector.
- `3V3_A` feeds ADXL1005 VDD.

## Practical cautions (v0.1)
- The DWM3001CDK product brief identifies `VBAT` feeding an on-board DC-DC, but does not publish a numeric `VBAT` range. Treat '3V3 into VBAT' as a bench-validated assumption.
- Add `JP_CDK_EN` so the CDK can be disconnected when powering from USB during flashing.
