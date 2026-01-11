# Thin-Pod carrier board v0.1 (Option B)

## Purpose

A small carrier that turns a DWM3001C-CDK plus an ADXL1005 breakout into a repeatable Thin-Pod node by providing:

- a defined power entry (JST_BATT_IN)
- a single regulator stage (Pololu S7V8F3) generating 3V3_SYS
- a split of 3V3_SYS into 3V3_CDK and 3V3_A
- a simple anti-alias front-end and ADC header routing to the CDK (J10.15)
- a battery sense divider to an ADC-capable pin for VBATT logging

## Power architecture

### Block diagram

JST_BATT_IN (RAW_IN) -> (optional PTC fuse) -> (reverse polarity protection) -> S7V8F3 -> 3V3_SYS -> { 3V3_CDK, 3V3_A }

### Power entry

- Connector: JST_BATT_IN (JST-PH2)
- Net: RAW_IN
- Intended sources: 3xAA pack, Li-ion pack within S7V8F3 VIN range, bench supply

### Regulator

- Module: Pololu S7V8F3
- Input: RAW_IN
- Output: 3V3_SYS (fixed 3.3 V)
- Optional: EN routed to GPIO (hard gating)

### Rail split

- 3V3_CDK: branch to CDK power entry (J1).
- 3V3_A: branch to ADXL1005 VDD, optionally isolated via ferrite bead.

## CDK power entry

The CDK is powered from the carrier via J1 holes on the DWM3001C-CDK.

- J1 '+' <- 3V3_CDK
- J1 '-' <- GND

This avoids relying on small battery pads and keeps the interface header-friendly.

## ADXL1005 power and signal

### Sensor power

- 3V3_A -> ADXL1005 VDD
- Local decoupling at the sensor is expected (100 nF plus 1 uF to 4.7 uF).

### ADC signal

ADXL1005 VOUT is routed through a first-order low-pass into the CDK analogue input.

- ACC_VOUT -> R_SER1 -> ADC_IN0
- ADC_IN0 -> C_SHUNT1 -> GND
- CDK entry: ADC_IN0 -> J10.15

## Battery sense divider

- RAW_IN -> divider -> VBATT_SENSE -> ADC pin
- Add C_BATT_SENSE at the sense node for stability.

Starter values for 3xAA:
- R_TOP = 180 kOhm
- R_BOT = 100 kOhm
- C_BATT_SENSE = 10 nF to 100 nF

## Default analogue population (bench matching)

- R_SER1 = 8.0 kOhm
- C_SHUNT1 = 680 pF
- C_PAR1 footprint present, DNP by default

## Practical cautions

- USB attachment to the CDK can also provide power. Avoid simultaneous powering from USB and 3V3_CDK unless power OR-ing is explicitly designed in.
- Keep GND common across battery, regulator, CDK, and sensor.
- Place decoupling capacitors close to loads and keep the analogue node wiring short.
