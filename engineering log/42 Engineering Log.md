# Engineering Log update: Thin-Pod first-pass perfboard prototype

## Summary

A first-pass Thin-Pod prototype has been assembled on a perfboard as an intermediate step ahead of a dedicated PCB carrier. The assembly consolidates the DWM3001CDK, an S7V8F3 buck regulator module, and the ADXL1005 breakout onto a single rigid platform to reduce breadboard wiring variability and improve repeatability during bench testing.

## What was built

* Perfboard ‘biscuit board’ integration of:

  * Qorvo DWM3001CDK (core compute and UWB link)
  * S7V8F3 step-down regulator (3.3 V rail generation)
  * ADXL1005 breakout (analogue vibration sensor front end)

* Shorter, more stable interconnects compared with a breadboard and Dupont leads.

## Purpose and value

* Provides a mechanically stable, legible bench prototype to validate:

  * power distribution behaviour under realistic handling
  * the analogue ingest chain from ADXL1005 output into the CDK ADC path
  * practical connector placement and routing constraints before committing to a PCB carrier spin

* Reduces the risk of chasing wiring-induced artefacts during ADC bring-up and telemetry tests.

## Next steps

* Record a simple bring-up checklist for the perfboard build: rail verification, sensor bias check, and ADC input sanity checks.
* Use observations from the perfboard build to finalise the v0.1 PCB carrier constraints: power split, test points, and analogue grounding strategy.
