# ADXL1005 analogue front-end and ADC entry (Thin-Pod bench)

## Signal chain

ADXL1005_VOUT -> R_SER -> ADC_NODE -> DWM3001C-CDK J10.15 (ADC_IN0)

ADC_NODE -> C_SHUNT -> GND

This placement keeps the shunt capacitor at the ADC node (to ground) while keeping the sensor output from directly driving a large capacitive load.

## Current bench population

- R_SER = 8.0 kOhm
- C_SHUNT = 680 pF

## What this filter does

The RC network is a first-order low-pass filter.

- Time constant: tau = R * C = 8 kOhm * 680 pF ~ 5.44 us
- Corner frequency: f_c ~ 1 / (2 * pi * R * C) ~ 29 kHz

This is aligned with vibration windowing where the objective is to capture a few kHz to a few tens of kHz, while limiting aliasing above the ADC sampling bandwidth.

## ADC interaction note (why the earlier topology failed)

When the shunt capacitor was placed directly on the sensor output node, the sensor output effectively saw a heavy capacitive load and the bias point was dragged down. Moving the capacitor to the ADC node, with R_SER between sensor and node, restores the expected mid-rail bias at rest and reduces loading.

## Wiring reference (headers)

- ADXL1005 VOUT -> carrier ACC_VOUT
- ACC_VOUT -> R_SER -> ADC_NODE
- ADC_NODE -> CDK J10.15
- ADC_NODE -> C_SHUNT -> ground

## Validation steps (analogue only)

- Confirm ADXL1005 VDD at 3V3_A (stable).
- Confirm VOUT sits near mid-rail at rest (about 1.6 V to 1.7 V is typical on a 3.3 V supply).
- Confirm ADC_NODE follows VOUT at DC and shows deflection during a tap event when viewed on a scope with DC coupling.

A DVM will show the DC bias but will not capture brief vibration excursions, so a scope (or high-rate ADC logging) is required for waveform confirmation.
