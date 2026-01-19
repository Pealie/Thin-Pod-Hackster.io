# Engineering Log Addendum: Thin-Pod carrier v0.2 KiCad schematic bring-up (2026-01-19)

## Summary

Work this evening converged the Thin-Pod carrier v0.2 schematic around the power path, PFET sensor power-gating, and the ADC conditioning network. Net highlighting was used to confirm correct net separation across `3v3+`, `GND`, `ACC_3V3_SW`, and `ADC_NODE`, including verification that the ADC node does not couple into the 3.3 V trunk and that decouplers return to ground.

ERC was iterated from an initial noisy report to a fully clean result. The final state is suitable to progress into PCB layout and DRC prior to Gerber generation.

## Key outcomes

* Power entry and protection locked in:

  * Battery input (`JST_BATT_IN`) and optional bench input (`J_BENCH_IN`) share the `RAW_IN` net.
  * Series Schottky diode (1N5817) establishes `RAW_PROT` as the protected rail feeding the regulator module `VIN`.

* PFET high-side power-gating for ADXL1005 supply verified:

  * PFET Source on the upstream regulated rail (`3v3+`).
  * PFET Drain on the switched sensor supply net (`ACC_3V3_SW`) feeding ADXL1005 `Vdd`.
  * Gate network in place: 100 k pull-up (Gate to Source) and 1 k series resistor from CDK GPIO (Pin16) to Gate.

* Switched sensor rail decoupling validated:

  * `C2` (100 nF) and `C3` (10 uF) placed on `ACC_3V3_SW` with return to `GND`.

* ADC conditioning network confirmed and isolated from the 3.3 V rail:

  * `ACC_VOUT` routed through `R1` (8 k) into `ADC_NODE`.
  * `ADC_NODE` returned to `GND` via `C1` (680 pF) and `R4` (1 M, populated pull-down).
  * `ADC_NODE` routed to CDK `J10.15_(ADC_input)` without coupling into `3v3+`.

* Net highlight verification captured:

  * `GND` highlight shows common ground continuity (regulator, CDK, ADXL1005, decouplers, ADC returns).
  * `3v3+` highlight shows supply distribution and includes PFET Source, excluding `ADC_NODE`.
  * `ADC_NODE` highlight includes `R1` right, `C1`, `R4`, and `J10.15_(ADC_input)` only.

## ERC progression

* Baseline ERC captured:

  * 17 errors and 3 warnings, dominated by connector symbol definition issues, missing power-source declarations, and library mismatches.

* Remediation actions applied:

  * Connector symbols normalised (pin numbering corrected, duplicated pins removed, passive electrical types used where appropriate).
  * Power intent declared for ERC using `PWR_FLAG` where required (raw rail, ground, and the switched sensor rail).
  * Regulator enable or shutdown pin state set to a defined level.
  * Unused pins marked explicitly with No-Connect markers.
  * Stray unconnected wire stubs removed.
  * Library symbol mismatches resolved.

* Final ERC state:

  * 0 errors, 0 warnings (clean report).

## Next actions

* Transition to PCB layout:

  * Define board outline, mounting holes, and keep-out guidance around the UWB antenna region.
  * Assign footprints for all THT parts and headers.
  * Run DRC continuously during placement and routing.

* Prepare fabrication outputs:

  * Generate Gerbers, drill files, and fabrication notes using a repeatable KiCad jobset.

