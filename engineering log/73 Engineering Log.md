# 73 Engineering Log

**Date:** 15 May 2026

## High-level summary

This session moved Thin-Pod rev 0.1 from a confusing power-up fault into a successful first analogue bring-up of the ADXL1005 vibration path.

The initial symptom was that the Qorvo DWM3001CDK would not power from the Thin-Pod board, even though approximately **3.37 V** was measured across the intended J1 positive and negative power pads. The CDK itself was not dead: it powered correctly from the micro-USB connector, with the red LED flashing, and had previously powered directly from a 3.3 V bench supply applied to J1. This ruled out a simple ‘unprogrammed CDK’ or dead-board explanation.

The fault was narrowed to the CDK ground reference. Continuity checks showed that the USB shield was not a reliable DC ground reference on the CDK. A proper CDK ground point was identified as the round ground loop beside the cream power connector, which also beeped to Raspberry Pi header pin 6. However, the soldered J1 negative connection did not beep to this confirmed CDK ground loop. This explained the misleading measurements: voltage could be seen across J1, but the CDK electronics were not receiving a valid return path to their own ground plane.

A temporary Dupont jumper was then fitted between **J1−** and the confirmed **CDK ground loop**. With this jumper in place, the CDK powered from the Thin-Pod battery input and the red LED began flashing. This proved that the immediate rev 0.1 issue was a missing or ineffective CDK ground return through the mounted J1 connection, not a failed CDK or a programming problem.

With the CDK ground workaround in place, the PFET high-side switch was retested. The measured values were:

* **PFET source:** 3.36 V
* **PFET gate:** 0 V
* **PFET drain:** 3.35 V

This confirmed that the PFET switch was now turning on correctly and passing the 3.3 V rail through to the accelerometer supply. The earlier half-rail behaviour was most likely caused by the unresolved ground-reference problem.

The ADXL1005 analogue chain was then checked at the test points:

* **/3V3+ to GND:** 3.35 V
* **TP6 /ACC\_3V3\_SW:** approximately 3.35 V
* **TP7 SENSOR\_SIGNAL\_FILTERED:** approximately 1.76 V

The TP7 value is consistent with a mid-rail accelerometer output bias when the ADXL1005 is powered from approximately 3.35 V. This gave a credible DC pass for the sensor supply and filtered analogue output.

It was also noted that the ADXL1005 **STANDBY** and **ST/self-test** pins are presently not connected. The part appears to be operating in the current rev 0.1 bring-up, but leaving those control pins floating is not a good final design practice. For rev 0.2, STANDBY should be pulled low to GND, probably with a 100 kΩ pulldown unless MCU-controlled duty-cycling is required. ST/self-test should also be pulled low, with an optional test pad or MCU line added for deliberate self-test activation.

The first dynamic oscilloscope test was then performed on **TP7 SENSOR\_SIGNAL\_FILTERED**. Finger taps produced clear transient bursts with damped ringing around the baseline. The scope capture showed an approximate **539 mV peak-to-peak** response during tapping. This is a strong practical indication that the ADXL1005 analogue signal chain is alive, biased correctly, and responding mechanically.

## Current verdict

Thin-Pod rev 0.1 has now achieved a significant bring-up milestone. The CDK can power from the board once the missing ground return is patched, the PFET-switched accelerometer rail is working, the ADXL1005 output is sitting near the expected mid-rail bias, and the filtered output responds visibly to mechanical taps on the oscilloscope.

The main rev 0.1 hardware issue discovered today is the CDK ground-return problem at J1. The main rev 0.2 design correction is to make the CDK ground connection explicit and robust, and to add defined low states for the ADXL1005 STANDBY and ST pins.

## Next steps

Replace the temporary Dupont ground link with a short, mechanically secure soldered jumper for continued rev 0.1 testing. The jumper should connect Thin-Pod/J1 ground to the confirmed CDK ground loop or another confirmed CDK ground point.

Repeat the CDK power-up test from battery after the soldered jumper is fitted, confirming that the red LED flashes reliably and that the CDK ground loop is common with Thin-Pod ground.

Capture a quieter oscilloscope baseline on TP7, then repeat the dynamic test using a more repeatable vibration source such as a small motor, fan, electric toothbrush, phone vibration motor, or mechanically coupled speaker tone.

Begin checking whether the CDK can read the filtered accelerometer output through the intended ADC path on **GPIO\_RPI / P0.28**.

Record the rev 0.2 corrections explicitly: fix the CDK J1/ground footprint or wiring strategy, add a robust CDK ground connection, add STANDBY and ST pulldowns for the ADXL1005, and consider exposing self-test through a test pad or MCU-controlled line.

