# 65 Engineering Log

## Thin-Pod PCB: evening update

This evening’s work moved the Thin-Pod PCB from a routed but still slightly untidy state into a clean ERC/DRC state suitable for manufacturing-prep review.

The main focus was resolving the remaining schematic/PCB consistency issues, checking component polarity and footprint choices, and cleaning up the mechanical mounting arrangement.

## C3 capacitor corrected

C3 was identified in the 3D view as a polarised radial electrolytic footprint, although its role in the circuit is local decoupling/bulk capacitance on the switched accelerometer supply rail.

The design intent was clarified:

```text
C2 = 100 nF local high-frequency decoupling
C3 = 10 uF local bulk decoupling
Both sit between /ACC\_3V3\_SW and /GND
```

For this role, a non-polar ceramic capacitor is the better default choice. It avoids polarity risk, has better local decoupling behaviour, and better matches the intended function near the ADXL1005 supply rail.

C3 was amended to use a non-polar capacitor footprint. After rerouting the `/ACC\_3V3\_SW` connection to C3 pad 2, the PCB DRC returned to a clean state.

## U3 PFET footprint and symbol issue resolved

The earlier U3 problem was clarified as two separate KiCad issues:

```text
PCB footprint issue
Schematic symbol synchronisation issue
```

The footprint side had already been fixed by saving a project-local PFET footprint:

```text
ThinPod:PFET\_ZVP2106A\_GSD\_THT
```

This gave U3 a proper Thin-Pod footprint rather than a locally modified generic connector footprint.

The schematic ERC warning remained because the placed U3 `PFET` symbol no longer matched its library copy. The correct fix was not to create a new symbol from scratch, but to update the selected U3 symbol from its existing library while preserving the already-correct footprint field.

The decisive step was:

```text
Update selected U3 symbol from library
Untick Footprint
Preserve ThinPod:PFET\_ZVP2106A\_GSD\_THT
```

This cleared the ERC warning without disturbing the corrected U3 footprint assignment.

The intended U3 electrical mapping remains:

```text
U3 Source → /3V3+
U3 Gate   → /PFET\_GATE
U3 Drain  → /ACC\_3V3\_SW
```

## Fuse section reviewed

The fuse-holder arrangement was checked to confirm that the PCB copper does not bypass the fuse.

The footprint uses two same-net pads per fuse clip:

```text
F1 pad 1 pair = /RAW\_IN
F1 pad 2 pair = /RAW\_FUSED
```

The short copper links between pads of the same number are correct. They join the two solder legs of each fuse clip. The important rule is that there must be no copper track directly joining pad 1 to pad 2, because that would bypass the fuse.

The intended power path remains:

```text
J1 + → /RAW\_IN → F1 → /RAW\_FUSED → D1 → /RAW\_PROT → U4 VIN
```

## J1 connector orientation reviewed

The vertical JST-PH connector was replaced with a right-angle/side-entry JST-PH footprint so the power cable can enter from the board edge rather than vertically from above.

The intended footprint is:

```text
Connector\_JST:JST\_PH\_S2B-PH-K\_1x02\_P2.00mm\_Horizontal
```

This should be checked in the 3D viewer for cable exit direction and clearance around D1, TP1, and the board edge.

## Mounting-hole arrangement revised

The board mounting holes were reconsidered in relation to U5, the DWM3001C-CDK module.

It was clarified that:

```text
H holes  = board support holes
SH holes = U5/CDK spacer holes
```

A courtyard overlap occurred between H1 and U5. Since H1 would have required increasing the PCB size or intruding into the U5 courtyard, H1 was deleted rather than enlarging the board. The design now proceeds with three board support holes, which is mechanically acceptable for this prototype.

Three board mounting points are sufficient for a flat mounting plane and avoid unnecessary board growth. U5 remains mechanically separate and is intended to sit clear of the PCB on spacers.

## 3D model completeness deferred

The missing 3D bodies for parts such as F1, U5, U2 and U4 were noted. These are useful for visual polish and enclosure checking, but not essential for fabrication.

The current priority is electrical and fabrication correctness. Detailed or simplified 3D models for those modules can be added later if enclosure design or mechanical presentation becomes the next focus.

## Current verification status

The latest reports are clean:

```text
ERC-44:
0 errors
0 warnings

DRC-45:
0 DRC violations
0 unconnected pads
0 footprint errors
```

This confirms that the schematic and PCB are now synchronised, the routing is complete, and the remaining library mismatch warnings have been cleared.

## Current board state

The board is now in a credible manufacturing-prep state:

```text
Board size: 80 mm × 80 mm
Routing: complete
GND zone: filled
C3: corrected to non-polar
U3: project-local PFET footprint assigned
J1: right-angle connector fitted
H1: deleted to avoid U5 courtyard conflict
ERC: clean
DRC: clean
```

## Next steps

Before exporting Gerbers, run a final manual inspection:

```text
1. Confirm J1 right-angle connector faces the desired board edge.
2. Confirm D1 cathode stripe faces /RAW\_PROT and U4 VIN.
3. Confirm F1 is not bypassed by copper.
4. Confirm U3 pads:
   S = /3V3+
   G = /PFET\_GATE
   D = /ACC\_3V3\_SW
5. Confirm C3 is non-polar and sits between /ACC\_3V3\_SW and /GND.
6. Confirm U5/CDK spacer holes and board mounting holes are mechanically sensible.
7. Refill all zones.
8. Save the project.
9. Run ERC and DRC once more after any movement or edits.
```

After that, prepare the fabrication outputs:

```text
Gerber files
Drill files
Board outline on Edge.Cuts
Optional fabrication drawing
BOM or placement notes for hand assembly
```

The next major project milestone is to generate and inspect the Gerbers, then prepare the board package for a small prototype run.

