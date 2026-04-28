# 66 Engineering Log

**Date:** 28 April 2026

## Thin-Pod PCB: evening update

This evening’s work focused on correcting the remaining symbol, footprint, and mechanical details in the Thin-Pod PCB before moving towards Gerber export and fabrication review.

The session began from a largely routed and DRC-clean board, but further checking uncovered several important issues that needed to be resolved before treating the design as manufacturing-ready.

## U2 ADXL1005 pin mapping corrected

A detailed check of U2 showed that the schematic symbol pin numbering did not match the intended physical pad numbering of the footprint.

The corrected U2 pin mapping is now:

```text
Pin 1  STB       unconnected
Pin 2  ORo       unconnected
Pin 3  Vout      /ACC\_VOUT
Pin 4  ST        unconnected
Pin 5  Gnd       /GND
Pin 6  Vdd\_3V3   /ACC\_3V3\_SW
```

This was an important correction because KiCad maps schematic pins to PCB footprint pads by pin number, not by visible pin name. After updating the U2 symbol and then updating the PCB from the schematic, two affected routes were repaired:

```text
/ACC\_VOUT
/ACC\_3V3\_SW
```

After rerouting those links, PCB DRC returned to a clean state.

## U2 and U4 library-symbol warnings cleared

After the U2 pin correction, ERC showed symbol-library mismatch warnings for U2 and U4:

```text
U2 = ADXL1005
U4 = Pololu\_S7V8F3\_S7V8x\_Module
```

These were housekeeping warnings rather than electrical errors, but they were worth resolving before fabrication output.

The relevant library symbols were updated, and the schematic was then updated from those corrected library symbols. This cleared the ERC warnings while preserving the corrected U2 pin mapping and the valid PCB routing.

Final reports confirmed:

```text
ERC-52:
0 errors
0 warnings

DRC-53:
0 DRC violations
0 unconnected pads
0 footprint errors
```

This is now a clean schematic and PCB state.

## Mounting-hole keep-outs added

Keep-out areas were added around the board mounting holes to allow clearance for M2.5 screw heads or washers.

The rule areas were configured to keep out:

```text
Tracks
Vias
Zone fills
```

on both copper layers. Pads were not kept out, avoiding unnecessary conflicts with the mounting-hole footprints themselves.

The keep-outs pull copper back from the screw/washer areas while preserving the intended board mounting scheme. DRC confirmed that the keep-outs are acceptable.

## Mechanical arrangement clarified

The distinction between board mounting holes and U5/CDK spacer holes was clarified:

```text
H holes  = board support holes
SH holes = U5/CDK spacer holes
```

The board now proceeds without the earlier conflicting H1 position, avoiding the need to enlarge the PCB just to preserve a fourth corner mounting hole. The remaining mounting-hole arrangement is acceptable for this prototype, especially with U5 supported separately on spacers.

## Current design state

The Thin-Pod PCB is now at a strong pre-Gerber checkpoint:

```text
Board size: 80 mm × 80 mm
Schematic ERC: clean
PCB DRC: clean
Routing: complete
Zones: filled
U2 pin mapping: corrected
U3 PFET mapping: corrected
U4 regulator symbol: synchronised
C3: non-polar 10 uF capacitor footprint
J1: right-angle JST-PH power connector
F1: fuse holder verified as not bypassed by copper
Mounting-hole keep-outs: added and DRC-clean
```

## Next steps

The next stage is to generate and inspect the fabrication outputs.

Recommended next sequence:

```text
1. Refill all zones.
2. Save the project.
3. Run ERC and DRC one final time.
4. Plot Gerber files.
5. Generate Excellon drill files.
6. Open the outputs in KiCad Gerber Viewer.
7. Inspect F.Cu and B.Cu copper.
8. Inspect F.Mask and B.Mask solder-mask openings.
9. Inspect F.Silkscreen text and labels.
10. Inspect Edge.Cuts board outline.
11. Inspect drill alignment.
12. Confirm PTH and NPTH holes are represented correctly.
13. Zip the fabrication outputs.
14. Upload to the PCB manufacturer’s preview tool.
15. Check the manufacturer preview before ordering.
```

The next major milestone is a verified Gerber/drill package suitable for a small prototype PCB run.

