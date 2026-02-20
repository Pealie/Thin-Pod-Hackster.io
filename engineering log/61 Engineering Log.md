# 61 Engineering Log

## Summary

A custom KiCad v9 symbol and footprint were produced for the Analog Devices EVAL-ADXL1005Z development board, enabling the module to be treated as a schematic component and PCB footprint within the Thin-Pod design.

## Work completed

* Confirmed the evaluation board pinout and produced a schematic symbol exposing the six header signals: VDD, GND, ST, VOUT, OR, and STANDBY.
* Defined a module footprint using standard 2.54 mm header pitch and the measured edge offsets for the two 1×3 header rows.
* Iteratively refined the mechanical outline and pad coordinates as board size measurements were improved, converging on a 20.35 mm × 20.35 mm outline with consistent pad placement from the measured offsets.
* Resolved a KiCad Footprint Editor import error caused by non-supported comment syntax in the footprint file, then re-exported a clean KiCad v9-compatible footprint that imports successfully.
* Captured import workflows (project footprint library registration and direct footprint open) to avoid clipboard-format parsing issues.
* Selected standard THT footprint assignments for the analogue front-end passives:

  * C1 (680 pF): small leaded ceramic footprint on 2.50 mm pitch
  * C2 (100 nF): small leaded ceramic footprint on 2.50 mm pitch
  * C3 (10 µF): radial electrolytic footprint (D5.0 mm, P2.00 mm)
  * R1 to R4 (1/4 W): DIN0207 axial resistor footprint (L6.3 mm, P7.62 mm, horizontal)

## Outputs

* KiCad v9 symbol library entry for the EVAL-ADXL1005Z module.
* KiCad v9 footprint for the ADXL100xZ breakout board family with a 20.35 mm square outline and measured header geometry.
* Updated footprint assignments for C1 to C3 and R1 to R4 suitable for a THT prototype build.

## Next actions

* If mounting hole positions are to be modelled, capture hole centre offsets with calipers and add NPTH or plated mounting holes to the module footprint.
* Validate physical fit using a 1:1 print or a small test PCB to confirm header alignment against the real development board.
