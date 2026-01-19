# Engineering Log Addendum: Thin-Pod carrier v0.2 KiCad schematic bring-up (2026-01-19)

## Summary

Work this evening converged the Thin-Pod carrier v0.2 schematic connectivity around the power path, PFET sensor power-gating, and the ADC conditioning network. Net highlighting was used as the primary verification method to confirm correct net separation (power, ground, ADC node, switched sensor rail). An ERC run was captured and the remaining issues were triaged as symbol and power-flag hygiene items rather than topology faults.

## Key outcomes

* Power entry path validated at schematic level:

  * Battery input and optional bench input present upstream of the series Schottky diode (1N5817).
  * Post-diode protected rail routed to the regulator module input.

* PFET high-side power-gating for ADXL1005 supply confirmed:

  * PFET Source tied to the upstream regulated 3V3 rail.
  * PFET Drain tied to a distinct switched sensor supply net feeding ADXL1005 Vdd.
  * Gate network in place: 100 k pull-up (Gate to Source) and 1 k series resistor from CDK GPIO (Pin16).

* Switched sensor rail decoupling completed and verified:

  * 100 nF and 10 µF decouplers placed on the switched ADXL1005 Vdd net with return to ground.

* ADC conditioning network confirmed and isolated from the 3V3 rail:

  * Series resistor (8 k) from ADXL1005 Vout into the ADC node.
  * Shunt capacitor (680 pF) and pull-down resistor (1 M) both returning from the ADC node to ground.
  * ADC node routed to the CDK ADC input (J10.15) without coupling to 3V3.

* Net highlight verification captured:

  * Ground net highlight confirmed common ground continuity (regulator, CDK, sensor, decouplers, ADC network returns).
  * 3V3 net highlight confirmed supply distribution and excluded ADC node.
  * ADC node highlight confirmed inclusion of R1, C1, R4, and J10.15 only.

## ERC status and triage

* ERC report captured with 17 errors and 3 warnings.
* Remaining issues categorised as schematic hygiene tasks:

  * Connector symbol pin numbering and duplicated pins (battery connector and bench header symbols).
  * Power net ‘driven’ declarations (PWR\_FLAG and correct pin electrical types).
  * Regulator SHDN pin requires a defined state.
  * Explicit ‘No Connect’ markers for unused ADXL1005 pins (ORo, ST, STB), or alternative routing to test points or headers.
  * Removal of stray unconnected wire stubs.
  * Library symbol mismatch noted for R4.

## Next actions

* Replace connector symbols with standard KiCad library equivalents and correct pin numbers.
* Add PWR\_FLAG and set appropriate power pin electrical types to clear power-pin-not-driven errors.
* Tie SHDN to a defined level consistent with the desired always-on behaviour.
* Apply ‘No Connect’ markers (or route) for unused ADXL1005 pins.
* Re-run ERC to confirm a clean report before PCB layout and Gerber generation.
