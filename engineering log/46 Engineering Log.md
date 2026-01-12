# Engineering Log Update: Thin-Pod bench bring-up (ADXL1005 to DWM3001-CDK)

Date: 2026-01-12

## Scope of work

Bench validation of Thin-Pod power integrity, ADXL1005 analogue output behaviour, and oscilloscope workflow on the FNIRSI 2C53T.

## Power integrity and distribution

* Regulated input confirmed stable at approximately 3.37 V.
* CDK 3V3 rail confirmed stable and closely matched the regulator output (millivolt-level difference previously observed).
* CDK pin 15 domain previously observed at ~1.788 V in-circuit; later tests clarified the measurement context (see ‘Signal path verification’).

## ADXL1005 control and mode confirmation

* STANDBY behaviour was inconsistent when left weakly defined. A DVM reading of ~0.192 V was observed once, then 0 V on a subsequent measurement, consistent with a high-impedance node.
* A 10 kΩ pull-down resistor was added between STANDBY and GND.
* Post-change, STANDBY consistently measures near 0 V (meter noise around zero, approximately -0.0004 V), confirming a solid logic-low default state.

## Wiring and pickup reduction

* Lead lengths were shortened to reduce loop area and pickup:

  * ADXL1005 Vout lead to the CDK was shortened.
  * 3.3 V supply leads to the ADXL1005 and CDK were shortened.

* A control test was performed with the probe tip placed on ground. The presence of a similar ~100 Hz pattern under this condition confirmed that earlier large ‘rail waveforms’ were dominated by probe-loop pickup rather than true rail ripple.
* After shortening leads and improving measurement technique, visible pickup reduced significantly, enabling smaller real signals to be observed.

## Signal path verification

* ADXL1005 was disconnected from CDK pin 15 for isolation testing.

  * With the sensor disconnected, CDK pin 15 measured ~0.001 V, consistent with a floating or high-impedance node when undriven.
  * With the sensor disconnected, ADXL1005 Vout measured ~1.78 V at rest, consistent with a midscale-biased analogue output at 3.3 V supply plus device and chain offset.

* Earlier readings near 1.78 V at CDK pin 15 were consistent with the sensor output driving the selected CDK pin rather than an internal fixed 1.8 V rail.

## Oscilloscope workflow and tap visibility

* FNIRSI 2C53T settings were stabilised for repeatable measurement:

  * Manual volts per division control confirmed.
  * DC coupling used for rail checks.
  * AC coupling used to centre small-signal motion around zero for tap visibility.

* Initial tap testing at 2 V per division and 200 ms timebase showed no discernible response, consistent with insufficient vertical sensitivity for tens-of-millivolts transients.
* After tightening vertical scale and using AC coupling on the sensor node, a clear waveform and transient spikes were observed when tapping, indicating mechanical excitation is reaching the ADXL1005 output and is measurable on the bench setup.

## Current status

* Power delivery is stable and repeatable.
* ADXL1005 is confirmed active (STANDBY held low) with a stable rest output near 1.78 V.
* Tap-correlated transients are now visible on the oscilloscope at appropriate scale and coupling, indicating the analogue sensing chain is behaving plausibly at the bench level.

## Next steps

* Capture a clean single-shot tap event with a millisecond timebase and edge trigger just above baseline noise.
* Verify the tap signature at both the ADXL1005 Vout pad and the intended DWM ADC input pin to confirm continuity and loading behaviour.
* Exercise ADXL1005 self-test (ST) if accessible to obtain a known step response for calibration and sanity checking of volts-to-g scaling.
