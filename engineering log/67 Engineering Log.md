# 67 Engineering Log

**Date:** 29 April 2026

## Thin-Pod PCB: fabrication order placed

This evening’s work moved **Thin-Pod rev 0.1** from a clean KiCad design into an active PCB fabrication order.

The board had already reached a strong pre-fabrication state, with the schematic ERC and PCB DRC passing cleanly. Today’s work completed the final manufacturing-output workflow: generating Gerbers and drill files, checking them in KiCad’s Gerber Viewer, packaging the fabrication outputs, uploading them to a manufacturer preview tool, and placing the first PCB order.

## Final project checks completed

Before generating fabrication files, the board was checked and saved in its final KiCad state:

```text
1. Refill all zones.
2. Save the project.
3. Run ERC and DRC one final time.
```

These checks were completed successfully before plotting the manufacturing outputs.

The verified design state at this point was:

```text
Schematic ERC: clean
PCB DRC: clean
Zones: filled
Routing: complete
Unconnected pads: 0
Footprint errors: 0
```

## Gerbers and drill files generated

The fabrication outputs were then produced from KiCad.

The required Gerber files were plotted, including the key manufacturing layers:

```text
F.Cu
B.Cu
F.Mask
B.Mask
F.Silkscreen
Edge.Cuts
```

Excellon drill files were also generated for the board holes, including the plated and non-plated hole information required by the PCB manufacturer.

## Gerber Viewer inspection completed

The generated outputs were opened in **KiCad Gerber Viewer** and inspected before ordering.

The following were checked:

```text
F.Cu and B.Cu copper
F.Mask and B.Mask solder-mask openings
F.Silkscreen text and labels
Edge.Cuts board outline
Drill alignment
PTH and NPTH hole representation
```

This provided an independent check that the plotted fabrication files matched the intended PCB layout, rather than relying only on the live PCB Editor view.

## Mechanical and visual checks confirmed

A final mechanical and visual sanity pass was also completed.

The following were checked:

```text
J1 right-angle connector exits in the correct direction.
H1/H2/H3 mounting holes have suitable screw-head clearance.
U5/CDK spacer holes align with the intended support points.
D1 polarity mark is visible and unambiguous.
TP labels remain readable.
Silkscreen is not too close to pads or the board edge.
No component is awkwardly close to the rounded corners.
```

This confirmed that the board was not only electrically clean, but also mechanically credible for a first prototype.

## Fabrication package zipped and uploaded

After the Gerber Viewer check, the fabrication outputs were zipped. The zip contained the plotted Gerber and drill files rather than the full KiCad project folder.

The fabrication package was uploaded to the PCB manufacturer’s preview tool and checked against the expected board appearance.

The manufacturer preview was inspected before ordering, confirming the key board features:

```text
80 mm × 80 mm board outline
2-layer copper
Top and bottom solder mask
Top silkscreen
Correct drill positions
Correct PTH and NPTH hole handling
Right-angle power connector footprint
Fuse-holder footprint
Mounting and spacer holes
```

## First PCB ordered

The first Thin-Pod PCB has now been ordered from a UK PCB manufacturer.

Order status:

```text
Board: Thin-Pod rev 0.1
Quantity: 1
Supplier: UK PCB manufacturer
Lead time: approximately 1 week
```

This is a major project milestone: Thin-Pod has now moved from KiCad design into physical board fabrication.

## Current project state

The current state of the Thin-Pod PCB is:

```text
Board size: 80 mm × 80 mm
Design phase: fabrication order placed
ERC: clean
DRC: clean
Gerbers: generated and inspected
Drill files: generated and inspected
Manufacturer preview: checked
Prototype PCB: ordered
```

The board is now awaiting manufacture and delivery.

## Next steps

The next stage is to prepare for board arrival, inspection, assembly and first electrical bring-up.

Recommended next steps:

```text
1. Prepare a bare-board inspection checklist.
2. Prepare a continuity-test checklist.
3. Prepare a staged assembly plan.
4. Prepare the bring-up test sequence.
5. Gather required components and hardware.
6. Confirm fuse, diode, regulator, connector and test-point parts are available.
7. Prepare bench power supply settings for current-limited first power-up.
8. Prepare oscilloscope and multimeter checks for TP1 to TP7.
```

Suggested first bring-up sequence:

```text
1. Inspect the bare PCB visually when it arrives.
2. Check board outline, holes, silkscreen and solder mask.
3. Check continuity of GND zones and mounting-hole keep-outs.
4. Check that RAW\_IN is not shorted to GND.
5. Check that the fuse path is not bypassed.
6. Fit the power-entry and regulator section first.
7. Power from a current-limited bench supply.
8. Measure TP1 RAW\_IN.
9. Measure TP2 RAW\_FUSED.
10. Measure TP3 RAW\_PROT.
11. Measure TP4 3V3+.
12. Measure TP5 GND reference.
13. Only then proceed to U3, U2 and U5/CDK-related bring-up.
14. Verify PFET switching of /ACC\_3V3\_SW.
15. Verify U2 output on TP6 /ACC\_VOUT.
16. Verify filtered ADC node on TP7 /ADC\_NODE.
```

The next major milestone is the arrival of the physical PCB and successful bare-board inspection followed by staged power bring-up.

