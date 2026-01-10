# Thin-Pod perfboard power plan: S7V8F3 3.3 V rail feeding ADXL1005 and DWM3001CDK via J1

## Decision
A bench-pragmatic power architecture has been selected for immediate prototyping:

- Battery input → S7V8F3 → regulated 3.3 V (`3V3_SYS`)
- `3V3_SYS` splits into two branches:
  - `3V3_A` → ADXL1005 VDD
  - `3V3_CDK` → DWM3001CDK J1 `+` (with J1 `−` to `GND`)

This keeps a single regulated rail for both the sensor and the CDK, while the analogue chain remains unchanged.

A long-term cleaner alternative is included at the end as a fallback path if stability issues appear when powering the CDK from 3.3 V via J1.

---

## Perfboard wiring diagram v0.1

### Net names
- `RAW_IN`      battery input (pre-regulation)
- `GND`         common ground
- `3V3_SYS`     S7V8F3 output
- `3V3_A`       sensor branch (ADXL1005 VDD)
- `3V3_CDK`     CDK branch (to CDK J1 `+`)
- `ACC_VOUT`    ADXL1005 Vout (at sensor pin)
- `ADC_IN0`     post-filter node into CDK ADC input

### Power distribution
```
Battery pack / bench RAW supply
    |
    |  JST-PH2 (JBAT1)
    +---- RAW_IN ---------------------------+
    |                                       |
    +---- GND ------------------------------+---------------------+
                                            |                     |
                                        S7V8F3 (buck)             |
                                            |                     |
                                            +-- 3V3_SYS ----------+----------------------+
                                                                                          |
                                                                                          |
                                           +----------------------------------------------+----------------------------------+
                                           |                                                                                 |
                                      Branch A: CDK power                                                               Branch B: sensor power
                                           |                                                                                 |
                                  3V3_SYS -> JP_CDK_EN -> 3V3_CDK -> 2-wire flying lead -> CDK J1 '+'                      3V3_SYS -> JP_SENSOR_EN -> 3V3_A -> ADXL1005 VDD
                                           |                                      |
                                           |                                      +-> CDK J1 '−' -> GND
                                           |
                                           +-> local decoupling near JCDK_PWR1: 100 nF + 10–100 µF
                                                                                                                     +-> local decoupling at ADXL1005: 100 nF + 1–10 µF
```

### Recommended perfboard parts for robustness
- `JP_CDK_EN` 2-pin jumper or 0 Ω link footprint to isolate CDK power when USB is attached.
- `JP_SENSOR_EN` 2-pin jumper or 0 Ω link footprint to isolate sensor rail during bring-up.
- Local decoupling at both branches:
  - near `3V3_CDK` feed: 100 nF + 10–100 µF
  - near `3V3_A` feed: 100 nF + 1–10 µF
- Test points:
  - `TP_RAW_IN`, `TP_3V3_SYS`, `TP_3V3_CDK`, `TP_3V3_A`, `TP_GND`

### Analogue chain (unchanged)
```
ADXL1005 Vout (ACC_VOUT)
    |
    |  R_SER1 = 8.0 kΩ
    +----/\/\/\----+-------------------------------> CDK J10 pin 15  (ADC input)
                   |
                   |  C_SHUNT1 = 680 pF
                   +----||----- GND
```

### Grounding convention
- A single ‘ground spine’ on perfboard is preferred, with short drops to:
  - CDK J1 `−`
  - CDK J10 pin 6 (and optionally 9/14/20/25 for additional return)
  - ADXL1005 GND
  - S7V8F3 GND

---

## Power-validation checklist (bench)

### A. Pre-power checks
- Visual inspection: polarity markings for CDK J1 `+ / −` and battery input verified.
- Continuity checks:
  - `3V3_SYS` to `GND` shows no short.
  - `3V3_CDK` to `GND` shows no short.
  - `3V3_A` to `GND` shows no short.
  - J1 `+` maps to `3V3_CDK` only when `JP_CDK_EN` is closed.

### B. Regulator-only validation (CDK disconnected)
- `JP_CDK_EN` open.
- Apply `RAW_IN` with a current limit set conservatively.
- Verify:
  - `TP_3V3_SYS` ≈ 3.3 V
  - `TP_3V3_A` ≈ 3.3 V with `JP_SENSOR_EN` closed
- Observe S7V8F3 stability: no oscillation, no excessive heating.

### C. CDK power-on validation (J1 fed from 3.3 V)
- `JP_CDK_EN` closed, USB disconnected from the CDK.
- Verify:
  - `TP_3V3_CDK` ≈ 3.3 V at the perfboard side
  - Voltage at CDK J1 pads remains close to 3.3 V under idle operation
- Record idle current draw.

### D. Worst-case activity stability test
- Run a high-activity mode on the CDK, for example continuous ranging or sustained packet transmission.
- Use an oscilloscope on:
  - `3V3_CDK` at the CDK end
  - `3V3_SYS` at the regulator end
- Acceptance criteria for ‘good enough for v0.1’:
  - No repeated brownouts or unexpected resets during activity.
  - Rail droop remains modest and recoverable, with no long undervoltage periods.

### E. USB coexistence safety
- With the CDK connected to USB for flashing:
  - `JP_CDK_EN` remains open.
  - Confirm no back-feed from USB into `3V3_SYS` through the CDK power path.
- With `JP_CDK_EN` closed:
  - USB power is not applied.

### F. Logging
- Record in the Engineering Log:
  - `RAW_IN` source and value
  - idle current and peak current observed
  - minimum `3V3_CDK` observed during activity
  - any resets or instability

---

## Fallback path if 3.3 V into J1 is unstable
If the CDK fails to boot reliably or browns out under UWB activity when powered from 3.3 V via J1:

- Power the CDK via an intended 5 V entry (USB, or the J10 5V0 pins when a verified 5 V rail is present).
- Keep S7V8F3 3.3 V as the sensor rail only (`3V3_A`).
- Keep `JP_CDK_EN` in the design as a structural isolation point for later revisions.

