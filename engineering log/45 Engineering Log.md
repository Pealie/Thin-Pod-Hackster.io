# Engineering Log Summary: Thin-Pod bring-up, power integrity and scope workflow

## Scope and measurement workflow

* FNIRSI 2C53T configuration was stabilised: volts/div successfully changed from the default 10 V/div, and coupling was moved to DC where appropriate for rail validation.
* The difference between Auto setup behaviour and manual, repeatable measurement settings was established, enabling controlled captures rather than ‘autoset’ artefacts.
* Single-shot capture and edge trigger usage was explored, including diagnosis of the ‘Await Trigger…’ condition as trigger threshold or source mismatch.

## Power integrity validation

* 3.3 V supply chain was verified as stable end-to-end:

  * S7V8F3 VOUT measured at ~3.34–3.37 V steady.
  * CDK 3V3 measured at ~3.37 V steady and matched S7V8F3 VOUT closely (millivolt-level difference).
* CDK pin 15 measured at ~1.788 V steady, consistent with an internal 1.8 V domain being present and stable.
* Earlier ‘bursty’ scope patterns were reinterpreted as measurement-mode artefacts rather than genuine brownout or regulator hiccup, based on the stable DC measurements.

## Noise and pickup characterisation

* A persistent ~100 Hz waveform observed on the scope was investigated.
* A definitive control test was performed: with the probe tip placed on ground, the same ~100 Hz pattern remained, demonstrating significant probe-loop pickup rather than real rail ripple.
* The measurement conclusion was updated accordingly: rail stability is supported by DMM readings, and scope traces at low timebases can be dominated by pickup unless probe grounding and bandwidth are controlled.

## Mechanical excitation observations

* Taps on the Thin-Pod produced repeatable visual changes on the scope, confirming that mechanical excitation can be detected in the measurement chain.
* Based on the pickup control result, the priority was refined: tap validation should be performed at the sensor output or ADC input node with appropriate timebase and coupling, rather than on the 3.3 V rail.

## Current status

* Power delivery to the CDK is confirmed stable (3V3 and 1V8 domains present).
* Scope configuration is now usable for rail integrity checks and for preparing a clean tap-capture workflow.
* Next focus is clean triggering and capture on the actual sensor signal path (ADXL output and the DWM ADC input), using DC coupling, millisecond timebases, and improved probe grounding to minimise pickup.
