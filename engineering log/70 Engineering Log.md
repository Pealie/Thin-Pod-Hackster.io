# 70 Engineering Log.md

**Log date:** 04 May 2026  
**Project:** Thin-Pod Gateway rev 0.1  
**Focus:** Final fabrication-package review, manufacturer upload, and late board-outline amendment

## High-level summary

Today’s work moved the Thin-Pod Gateway rev 0.1 PCB from a local KiCad fabrication-output package into a submitted fabrication candidate with a UK PCB manufacturer.

The board had already reached a clean KiCad state after the addition of four corner mounting holes and two through-hole 10 kΩ chip-select pull-up resistors. Today’s work focused on checking the generated fabrication package, confirming the drill-layer behaviour, uploading the fabrication ZIP to a manufacturer, and making a final board-outline refinement.

A key manufacturing check was the distinction between plated and non-plated holes. The corner mounting holes H1-H4 were confirmed to appear in the NPTH drill layer, while the through-hole pull-up resistor holes R1/R2 and the header holes were confirmed to appear in the PTH drill layer. This is the desired manufacturing split: mechanical holes are non-plated, and solderable component/header holes are plated.

The fabrication ZIP for Thin-Pod Gateway rev 0.1 was then uploaded to a UK PCB manufacturer. The board has not yet been purchased, but the order is intended to be placed within approximately 30 days, by the end of the month.

A late amendment was made after upload/review: 3 mm corner radii were added to the PCB outline. This improves handling and removes sharp 90-degree board corners. The amendment was checked and accepted as OK. The main concern with rounded corners was whether the four corner mounting holes still had adequate edge clearance and whether the manufacturer preview recognised the updated outline correctly. These checks were completed satisfactorily.

The design is now in a stable pre-order state. The project is no longer blocked by schematic errors, DRC violations, missing footprints, unrouted nets, drill-output uncertainty, or obvious Gerber-viewer problems.

## Main achievements

* Confirmed the final fabrication package after adding the mounting holes and through-hole resistors.
* Verified that H1-H4 mounting holes appear in the NPTH drill layer.
* Verified that R1/R2 through-hole resistor holes appear in the PTH drill layer.
* Verified that the header holes appear in the PTH drill layer.
* Confirmed that non-plated corner mounting holes are acceptable for this first bench prototype.
* Confirmed that through-hole pull-up resistors are more appropriate than SMD pull-ups for the first hand-assembled prototype.
* Uploaded the Thin-Pod Gateway rev 0.1 fabrication ZIP to a UK PCB manufacturer.
* Set the purchase target as within 30 days, by the end of the month.
* Added a late board-outline amendment: 3 mm radius corners.
* Checked the rounded-corner amendment and confirmed it is OK.
* Reached a stable pending-order milestone.

## Current fabrication status

```text
Design: Thin-Pod Gateway rev 0.1
KiCad ERC: clean
KiCad DRC: clean
Unconnected pads: 0
Footprint errors: 0
Gerbers: generated
Excellon drill files: generated
PTH/NPTH split: checked
Fabrication ZIP: uploaded to UK PCB manufacturer
Late amendment: 3 mm board-corner radii
Amendment check: OK
Purchase status: pending
Purchase target: by end of month / within 30 days
```

## Manufacturing notes

The current manufacturing intent is:

|Feature|Manufacturing type|Status|
|-|-|-|
|H1-H4 corner mounting holes|NPTH mechanical holes|Checked|
|R1/R2 pull-up resistor holes|PTH solderable holes|Checked|
|Module/header holes|PTH solderable holes|Checked|
|Board outline|Edge.Cuts with 3 mm corner radius|Checked|
|Silkscreen label|Front silkscreen|Checked previously|
|XIAO footprint silkscreen|Cleaned and saved to library|Checked previously|
|Ground pour|Present in Gerber output|Checked previously|

The non-plated mounting holes remain appropriate for rev 0.1 because they are mechanical only and there is no defined chassis-grounding or enclosure-bonding requirement at this stage.

The through-hole pull-up resistors remain appropriate for rev 0.1 because the board is intended for bench assembly without a dedicated SMD soldering setup. The pull-ups are low-speed bias components, so the through-hole implementation does not compromise the prototype’s intended function.

## Late amendment: 3 mm corner radii

The board outline was amended to add 3 mm radius corners.

This is a practical improvement for a prototype PCB because it:

* removes sharp board corners,
* improves handling,
* reduces the chance of corner damage,
* gives the board a more finished physical form,
* and is easy for PCB manufacturers to route.

The amendment was checked as OK. The key checks were:

* the board outline remained valid,
* the manufacturer preview recognised the rounded corners,
* the copper pour respected the new edge shape,
* and the corner mounting holes still had acceptable edge clearance.

## Remaining actions before purchase

Before placing the order, perform one final short check:

1. Confirm that the manufacturer’s online preview still shows the 3 mm rounded corners.
2. Confirm that H1-H4 appear as non-plated drill holes.
3. Confirm that R1/R2 and all header holes appear as plated drill holes.
4. Confirm board dimensions and layer count.
5. Confirm board thickness.
6. Confirm solder-mask colour.
7. Confirm silkscreen side and colour.
8. Confirm copper weight.
9. Confirm quantity and price.
10. Confirm delivery estimate.
11. Download or save a copy of the submitted fabrication ZIP for project records.
12. Save screenshots of the manufacturer preview if available.

## Immediate verdict

Thin-Pod Gateway rev 0.1 has reached a mature pre-order fabrication milestone.

The board is now a credible first bench-prototype carrier for the NUCLEO-N657X0-Q, DWM3001C-CDK and Seeed Studio XIAO ESP32-C6. The core KiCad design is clean, the manufacturing outputs have been generated and checked, the PTH/NPTH drill split has been verified, the board outline has been improved with rounded corners, and the fabrication ZIP has been uploaded to a UK PCB manufacturer.

The next major milestone is placing the PCB order before the end-of-month target, then preparing a bring-up and verification procedure for when the board arrives.

