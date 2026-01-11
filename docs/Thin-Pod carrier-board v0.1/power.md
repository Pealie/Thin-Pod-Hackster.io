# Thin-Pod power plan (battery to DWM3001C-CDK + ADXL1005)

## Summary

Power entry is treated as a single raw battery input feeding a single regulator, then split into two 3.3 V branches:

- RAW_IN via JST_BATT_IN (JST-PH2)
- S7V8F3 generates 3V3_SYS
- 3V3_SYS splits into:
  - 3V3_CDK -> DWM3001C-CDK power via J1 holes ('+' / '-')
  - 3V3_A -> ADXL1005 VDD (analogue branch)

## Block diagram

JST_BATT_IN (RAW_IN) -> (optional PTC fuse) -> (reverse polarity protection) -> S7V8F3 -> 3V3_SYS -> { 3V3_CDK, 3V3_A }

### Recommended protection and passives

- Optional PTC fuse on RAW_IN sized for expected peak current.
- Reverse polarity protection:
  - simplest: Schottky diode in series with RAW_IN (voltage drop accepted), or
  - preferred: ideal diode controller or PFET ideal diode arrangement.
- Input bulk capacitance close to S7V8F3 VIN: 10 uF to 47 uF (low ESR) plus 100 nF.
- Output capacitance close to S7V8F3 VOUT: 10 uF to 47 uF (low ESR) plus 100 nF.

### Rail split guidance

- 3V3_CDK: digital and RF load (DWM3001C-CDK).
- 3V3_A: analogue load (ADXL1005). A ferrite bead (or small resistor) plus local decoupling is recommended to isolate high frequency digital current from the sensor rail.

Typical analogue branch:
- 3V3_SYS -> ferrite bead -> 3V3_A
- 3V3_A decoupling at ADXL: 100 nF close to VDD, plus 1 uF to 4.7 uF nearby.

## DWM3001C-CDK power entry: J1

3V3_CDK is injected via J1 (two holes labelled '+' and '-').

- J1 '+' <- 3V3_CDK
- J1 '-' <- GND

J1 is used instead of small battery pads to keep the interface robust and repeatable via headers.

## Battery sense divider

RAW_IN is routed to an ADC-capable pin via a resistive divider so VBATT logging remains available.

Suggested starting point (for ~4.5 V max AA pack):
- R_TOP = 180 kOhm
- R_BOT = 100 kOhm
- Divider ratio ~ 0.357, so 4.5 V maps to ~1.61 V.

Add a small capacitor from the ADC node to ground to reduce noise and provide a stable sample node:
- C_BATT_SENSE = 10 nF to 100 nF

## Power-source interaction note (USB connected)

When a USB cable is attached to the CDK for CLI access, the USB port can also provide power to the CDK. Simultaneous powering from USB and external 3V3_CDK risks backfeeding through unknown on-board power paths.

Safer bench practice:
- run from a single power source at a time, or
- use a USB data-only cable (VBUS not connected), or
- break the USB VBUS line, or
- add explicit power OR-ing on the carrier when concurrent USB attachment is required.

## Brownout policy (planning note)

A minimum VBATT threshold should be enforced before UWB transmit to avoid unstable radio bursts and brownout resets.

- VBATT_OK threshold: derive from measured current peaks and regulator headroom.
- Policy: 'TX disabled' below threshold, logging continues; optional hard-off via EN.

Optional: route the S7V8F3 EN pin to a GPIO for hard gating of 3V3_SYS.
