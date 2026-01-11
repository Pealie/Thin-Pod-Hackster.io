# Thin-Pod: bench wiring notes (power + sensor ADC entry)

This repository contains bench-proven wiring patterns for the Thin-Pod node based on a DWM3001C-CDK and an ADXL1005 breakout, intended to translate into a small carrier PCB.

## Current bench-validated wiring (Stage 3 entry conditions)

### Power

- Raw battery input enters via JST_BATT_IN (JST-PH2) as RAW_IN.
- A Pololu S7V8F3 module regulates RAW_IN to 3V3_SYS.
- 3V3_SYS splits into:
  - 3V3_CDK feeding the DWM3001C-CDK via J1 holes ('+' / '-')
  - 3V3_A feeding the ADXL1005 VDD

### Sensor input

- ADXL1005 VOUT feeds the CDK ADC via a first-order RC low-pass:
  - ADXL1005_VOUT -> R_SER -> ADC_NODE -> CDK J10.15
  - ADC_NODE -> C_SHUNT -> ground

Bench values in use:
- R_SER = 8.0 kOhm
- C_SHUNT = 680 pF (about 29 kHz corner frequency with 8 kOhm)

### Header strategy

- CDK power uses J1 as a header interface rather than soldering to battery pads.
- ADC entry uses J10.15 via header access.

## Files

- power.md power entry, regulator, split rails, VBATT sense, and brownout notes
- sensor_frontend.md analogue filter and ADC entry details
- dwm3001_cdk_header.md CDK connection points used in the bench setup
- thin-pod-carrier-v0.1.md carrier PCB intent and net naming

## Known-good status

- CLI and UWB ranging are stable on FreeRTOS.
- J1 injection at 3.3 V from S7V8F3 is bench validated.
- ADXL1005 DC bias is restored with the RC shunt capacitor placed at the ADC node.
