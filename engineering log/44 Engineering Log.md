# Engineering Log update: Thin-Pod power strategy and validation plan

## Summary
A bench-pragmatic Thin-Pod power strategy has been formalised and written up as an actionable wiring plan and validation checklist. The approach uses a single S7V8F3 buck regulator to generate a regulated 3.3 V system rail, then splits that rail to power both the ADXL1005 and the DWM3001CDK via its J1 `+ / −` battery/loose-wire input, while keeping the analogue ingest chain unchanged.

## Key decisions
- Selected a single-regulator architecture for prototyping:
  - `RAW_IN` → S7V8F3 → `3V3_SYS`
  - `3V3_SYS` split into:
    - `3V3_A` → ADXL1005 VDD
    - `3V3_CDK` → DWM3001CDK J1 `+` (with J1 `−` to `GND`)
- Introduced branch isolation as a first-class feature:
  - `JP_CDK_EN` to disconnect CDK power when USB is attached for flashing
  - `JP_SENSOR_EN` to isolate the sensor rail during bring-up

## Artefacts produced
- A concrete perfboard wiring diagram with explicit net naming and branch structure.
- A power-validation checklist covering pre-power continuity checks, staged power-up, worst-case activity stability testing, and safe USB coexistence.
- A documented fallback path if 3.3 V into J1 proves unstable under UWB transmit bursts:
  - power the CDK via an intended 5 V entry and keep S7V8F3 3.3 V for the sensor rail only.

## Notes
- The analogue chain remains aligned to the established bench baseline (ADXL1005 Vout → RC low-pass → CDK ADC input).
- The validation plan emphasises recording measured rail droop, current draw, and reset behaviour to de-risk the upcoming PCB carrier design.
