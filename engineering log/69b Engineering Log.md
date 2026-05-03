# 69b Engineering Log.md

**Log date:** 03 May 2026  
**Project:** Thin-Pod Gateway rev 0.1  
**Focus:** Final pre-fabrication additions, through-hole prototype changes, Gerber/drill verification

## High-level summary

Today’s follow-up work moved the Thin-Pod Gateway rev 0.1 board from a clean KiCad design-rule milestone into a more complete first-bench-prototype fabrication package.

After the earlier clean ERC/DRC state and initial Gerber generation, two remaining design omissions were identified before treating the board as fabrication-ready: four corner mounting holes and two SPI chip-select pull-up resistors. Both were added to the design, checked, and incorporated into the PCB.

The four mounting holes were added at the board corners for mechanical fixing. The selected mounting-hole footprints are non-plated through holes, which is acceptable and sensible for this first bench prototype because the holes are mechanical only and are not part of any chassis-grounding or electrical bonding strategy.

The two chip-select pull-up resistors were added to keep the SPI peripherals deselected during reset or before the NUCLEO firmware configures the relevant GPIO pins. The pull-ups were assigned to the two chip-select nets:

```text
DWM\_CS
C6\_CS
```

with pull-up connection to the existing 3.3 V rail:

```text
3V3\_C6
```

The initial resistor footprints were 0805 SMD, but these were deliberately changed to through-hole resistor footprints because the first prototype is intended for bench assembly without an SMD soldering setup. This is a practical and appropriate decision for rev 0.1. The pull-ups are low-speed 10 kΩ bias resistors, so through-hole implementation has no meaningful performance penalty for this prototype.

After updating the schematic and PCB, the project was rechecked. ERC remained clean, and DRC returned to a clean state after the new components were added and routed.

The fabrication outputs were regenerated and inspected. The Gerber and drill outputs were checked far enough to confirm the critical fabrication distinction:

```text
H1-H4 mounting holes appear in the NPTH drill layer.
R1/R2 resistor holes appear in the PTH drill layer.
Header holes appear in the PTH drill layer.
```

This confirms that the mechanical holes and solderable electrical holes are being represented correctly in the manufacturing outputs.

## Main achievements

* Added four corner mounting holes to the PCB.
* Confirmed that the mounting holes are mechanical NPTH holes, acceptable for the first bench prototype.
* Added two SPI chip-select pull-up resistors.
* Connected the pull-ups to:

  * `DWM\_CS`,
  * `C6\_CS`,
  * `3V3\_C6`.
* Replaced the initial SMD resistor footprints with through-hole resistor footprints for easier hand assembly.
* Confirmed that R1/R2 through-hole resistor pads appear as plated through holes.
* Confirmed that the header holes remain plated through holes.
* Confirmed that H1-H4 mounting holes appear in the NPTH drill layer.
* Regenerated fabrication outputs after the mounting holes and resistor footprint changes.
* Checked the Gerber/drill output visually.
* Maintained the project’s clean electrical and physical design-rule state after the additions.

## Current board state

The Thin-Pod Gateway rev 0.1 PCB now includes:

* NUCLEO-N657X0-Q carrier footprint.
* DWM3001C-CDK module/header footprint.
* Seeed Studio XIAO ESP32-C6 DIP/header footprint.
* Consolidated CN3/CN15 NUCLEO wiring strategy.
* Shared SPI5 bus routing.
* DWM3001C-CDK chip-select, IRQ and reset routing.
* XIAO ESP32-C6 chip-select and supporting connections.
* Ground pour.
* Four corner mounting holes.
* Two 10 kΩ through-hole chip-select pull-up resistors.
* Cleaned XIAO silkscreen geometry.
* Readable board silkscreen label.
* Generated Gerber and Excellon drill outputs.

## Pull-up resistor intent

The two pull-up resistors are present to avoid accidental SPI peripheral selection during reset or early boot.

The intended schematic relationship is:

```text
3V3\_C6 -- 10 kΩ -- DWM\_CS

3V3\_C6 -- 10 kΩ -- C6\_CS
```

This means:

|Net|Purpose|
|-|-|
|`DWM\_CS`|Chip-select for the DWM3001C-CDK|
|`C6\_CS`|Chip-select for the Seeed Studio XIAO ESP32-C6|
|`3V3\_C6`|Pull-up rail used to hold both CS lines inactive by default|

The pull-up value of 10 kΩ is appropriate for this kind of digital biasing. It is strong enough to define the idle logic state but weak enough not to interfere with the NUCLEO actively driving the chip-select pins.

## Through-hole decision

The resistors were changed from 0805 SMD to through-hole footprints because this first board is intended as a hand-built bench prototype.

This is appropriate for rev 0.1 because:

* through-hole resistors are easier to solder with basic equipment,
* the pull-up function is not speed-critical,
* the components can be changed easily during bring-up,
* and the board is being used to validate architecture and connectivity rather than final product miniaturisation.

A later rev 0.2 board could return to SMD pull-ups once the electrical design, firmware behaviour and bring-up workflow are validated.

## Mounting-hole decision

The corner holes are non-plated through holes.

This is acceptable because:

* the holes are mechanical only,
* no chassis-ground bonding has been specified,
* non-plated holes avoid accidental copper contact around screws or standoffs,
* and they are conventional for simple mechanical mounting in a prototype board.

If a later version needs chassis grounding, EMI bonding or enclosure-ground strategy, one or more mounting holes can be changed to plated holes with controlled copper pads, keepouts, stitching, or defined chassis-ground connections.

## Fabrication-output verification

The regenerated fabrication outputs were checked, including the important drill-layer distinction:

```text
H1-H4 mounting holes: NPTH drill layer
R1/R2 resistor holes: PTH drill layer
Headers: PTH drill layer
```

This matters because a PCB manufacturer needs to know which holes are mechanical non-plated holes and which holes require plated barrels for soldering.

The current output check suggests that:

* the mounting holes should be drilled but not plated,
* the resistor holes should be drilled and plated,
* the module/header holes should be drilled and plated,
* and the board is suitable for the intended hand-assembled bench prototype workflow.

## Remaining final checks before ordering

Before uploading the fabrication ZIP to a PCB manufacturer, perform one final verification loop:

1. Open the KiCad project.
2. Save the schematic and PCB.
3. Refill all zones with **B**.
4. Run ERC.
5. Run DRC.
6. Regenerate Gerbers.
7. Regenerate Excellon drill files.
8. Open the regenerated Gerbers in Gerber Viewer.
9. Load both PTH and NPTH drill files if KiCad generated them separately.
10. Confirm the board outline is present and closed.
11. Confirm the four corner mounting holes appear correctly.
12. Confirm R1/R2 through-hole resistor drills appear correctly.
13. Confirm all header holes appear as plated drill holes.
14. Confirm the ground pour appears in the intended copper layer.
15. Confirm no silkscreen text is mirrored or placed over pads.
16. Confirm the manufacturer’s online Gerber preview matches KiCad Gerber Viewer.

## Immediate verdict

The Thin-Pod Gateway rev 0.1 board is now a credible first bench-prototype fabrication candidate.

The earlier checkpoint Gerbers have been superseded by the updated design containing the four corner mounting holes and two through-hole chip-select pull-up resistors. The key fabrication distinction between NPTH mounting holes and PTH solderable holes has been checked and appears correct.

The next practical step is to zip the regenerated fabrication outputs and inspect them in the PCB manufacturer’s online viewer before placing the order.

