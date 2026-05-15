# 73 Engineering Log

**Date:** 15 May 2026

## High-level summary

This session moved Thin-Pod rev 0.1 from a confusing power-up fault into a successful first analogue bring-up of the ADXL1005 vibration path.

The initial symptom was that the Qorvo DWM3001CDK would not power from the Thin-Pod board, even though approximately **3.37 V** was measured across the intended J1 positive and negative power pads. The CDK itself was not dead: it powered correctly from the micro-USB connector, with the red LED flashing, and had previously powered directly from a 3.3 V bench supply applied to J1. This ruled out a simple unprogrammed-CDK or dead-board explanation.

The fault was narrowed to the CDK ground reference. Continuity checks showed that the USB shield was not a reliable DC ground reference on the CDK. A proper CDK ground point was identified as the round ground loop beside the cream power connector, which also beeped to Raspberry Pi header pin 6. However, the soldered J1 negative connection did not beep to this confirmed CDK ground loop. This explained the misleading measurements: voltage could be seen across J1, but the CDK electronics were not receiving a valid return path to their own ground plane.

A temporary Dupont jumper was then fitted between **J1−** and the confirmed **CDK ground loop**. With this jumper in place, the CDK powered from the Thin-Pod battery input and the red LED began flashing. This proved that the immediate rev 0.1 issue was a missing or ineffective CDK ground return through the mounted J1 connection, not a failed CDK or a programming problem.

## Ground-link refinement and ADXL1005 dynamic test

After the temporary Dupont ground link confirmed the missing CDK ground return, it was replaced with a short, mechanically secure soldered jumper between the Thin-Pod ground path and the confirmed CDK ground loop. This changed the earlier proof-of-life workaround into a more stable rev 0.1 test configuration.

With the soldered ground jumper fitted, the CDK powered correctly from the battery input and the red LED flashed as expected. The common ground reference was therefore stable enough for continued analogue testing.

The ADXL1005 signal chain was then checked again. The PFET-switched accelerometer rail at TP6, `/ACC\_3V3\_SW`, measured approximately **3.35 V**. The filtered sensor output at TP7, `SENSOR\_SIGNAL\_FILTERED`, measured approximately **1.76 V**, consistent with a mid-rail analogue output from the accelerometer chain.

An oscilloscope was connected to TP7 and referenced to the confirmed common ground. Light mechanical taps on the board produced clear transient bursts on the scope. The captured response showed approximately **539 mV peak-to-peak** during tapping, with visible damped ringing around the baseline. This confirmed that the ADXL1005 analogue output path is not merely biased correctly, but is dynamically responding to mechanical vibration.

Current rev 0.1 bring-up verdict: the CDK power issue has been isolated to a missing ground-return path and temporarily corrected with a soldered jumper. The PFET-switched sensor rail is working. The ADXL1005 output bias is plausible. The oscilloscope tap test confirms a live vibration response at TP7.

For rev 0.2, the CDK ground return must be corrected in the footprint or interconnect design rather than handled by a jumper. The ADXL1005 `STANDBY` and `ST` pins should also be given defined inactive states, most likely with pulldowns to ground, with optional MCU or test-pad access for self-test.

## Supporting measurements

With the soldered ground workaround in place, the PFET high-side switch was retested. The measured values were:

* **PFET source:** 3.36 V
* **PFET gate:** 0 V
* **PFET drain:** 3.35 V

This confirmed that the PFET switch was turning on correctly and passing the 3.3 V rail through to the accelerometer supply. The earlier half-rail behaviour was most likely caused by the unresolved ground-reference problem.

The ADXL1005 analogue chain was then checked at the test points:

* **/3V3+ to GND:** 3.35 V
* **TP6 /ACC\_3V3\_SW:** approximately 3.35 V
* **TP7 SENSOR\_SIGNAL\_FILTERED:** approximately 1.76 V

The TP7 value is consistent with a mid-rail accelerometer output bias when the ADXL1005 is powered from approximately 3.35 V. This gave a credible DC pass for the sensor supply and filtered analogue output.

It was also noted that the ADXL1005 **STANDBY** and **ST/self-test** pins are presently not connected. The part appears to be operating in the current rev 0.1 bring-up, but leaving those control pins floating is not a good final design practice. For rev 0.2, STANDBY should be pulled low to GND, probably with a 100 kΩ pulldown unless MCU-controlled duty-cycling is required. ST/self-test should also be pulled low, with an optional test pad or MCU line added for deliberate self-test activation.

## Current verdict

Thin-Pod rev 0.1 has now achieved a significant bring-up milestone. The CDK can power from the board once the missing ground return was patched, the PFET-switched accelerometer rail is working, the ADXL1005 output is sitting near the expected mid-rail bias, and the filtered output responds visibly to mechanical taps on the oscilloscope.

The main rev 0.1 hardware issue discovered today is the CDK ground-return problem at J1. The soldered jumper is an acceptable rev 0.1 test workaround, but it is not a final design solution. The main rev 0.2 design corrections are to make the CDK ground connection explicit and robust, and to add defined low states for the ADXL1005 STANDBY and ST pins.

## Next steps

Repeat the CDK power-up test from battery with the fitted soldered jumper. Capture a quieter oscilloscope baseline on TP7, then repeat the dynamic test using a more repeatable vibration source such as a small motor, fan, electric toothbrush, phone vibration motor, or mechanically coupled speaker tone.

Begin checking whether the CDK can read the filtered accelerometer output through the intended ADC path on **GPIO\_RPI / P0.28**.

Record the rev 0.2 corrections explicitly: fix the CDK J1/ground footprint or wiring strategy, add a robust CDK ground connection, add STANDBY and ST pulldowns for the ADXL1005, and will an exposing self-test through a test pad or MCU-controlled line to be considered.

