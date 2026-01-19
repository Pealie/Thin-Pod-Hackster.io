## Engineering Log summary (2026-01-18)

Work this evening advanced Thin-Pod carrier-board v0.2 from design notes into a KiCad-realistic, production-oriented schematic plan, with the key power and analogue control additions fully integrated and verified.

### Key outcomes

* Defined and documented the v0.2 power-entry update:

  * Optional bench supply 2-pin header added in parallel with the battery input on the raw side.
  * Series Schottky diode (1N5817) placed between `RAW_IN` and `RAW_PROT` to protect the downstream rail.
* Finalised the ADXL1005 deep-off strategy:

  * High-side P-channel MOSFET gating implemented so ADXL1005 `Vdd` is fed from the PFET drain on a switched rail, rather than the always-on 3V3 rail.
  * Gate network completed with a 100 k pull-up (Gate to Source) and a 1 k series resistor from CDK GPIO (Pin16).
* Locked down ADC input conditioning to the bench-validated topology:

  * Series resistor (8 k) into the ADC node, shunt capacitor (680 pF) and pull-down resistor (1 M) from `ADC_NODE` to `GND`.
  * Verified the correct net separation and topology using KiCad net highlighting.
* Completed switched-rail decoupling for the power-gated sensor rail:

  * 100 nF and 10 µF placed on the switched ADXL1005 supply net to support clean power-up and stable operation.
* Consolidated the above into KiCad-ready net and pin-list documentation for v0.2, suitable for direct schematic capture and PCB layout planning.
