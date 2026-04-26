# 62 Engineering Log

**Project:** Thin-Pod  
**Date:** 25 April 2026  
**Session focus:** KiCad PCB Editor placement, test point import, DRC interpretation, power connector diagnosis, CDK connectivity check, and pre-routing assessment.

[Back to contents](#contents)

## Contents

1. [Overview](#overview)
2. [Board size and placement state](#board-size-and-placement-state)
3. [Work completed](#work-completed)
4. [Test point implementation](#test-point-implementation)
5. [Latest DRC status](#latest-drc-status)
6. [Power connector footprint issue](#power-connector-footprint-issue)
7. [CDK / U7 connectivity issue](#cdk--u7-connectivity-issue)
8. [Routing strategy established](#routing-strategy-established)
9. [Open issues](#open-issues)
10. [Next session checklist](#next-session-checklist)
11. [Notes](#notes)

[Back to contents](#contents)

## Overview

This session moved the Thin-Pod PCB layout from schematic synchronisation and missing test point investigation into a stable pre-routing placement state. The newly added test points are now present in PCB Editor and positioned on the board. Dangling track warnings have been removed, footprint errors are no longer reported, and the latest DRC state shows no formal DRC violations. The board is therefore structurally cleaner than earlier in the evening.

The remaining issues are now connectivity and footprint-definition issues rather than general layout corruption. The two most important items to resolve before routing are the power connector footprint and the CDK / U7 connectivity. The current power connector still appears to have only one electrical pad, with its other visible pads acting only as mechanical pads. The CDK footprint also appears to have no ratsnest lines, suggesting that KiCad is not currently assigning schematic nets to the CDK pads.

[Back to contents](#contents)

## Board size and placement state

The current PCB outline is approximately:

```text
80 mm x 80 mm
```

This is a straightforward and easily manufactured prototype size. It sits comfortably within the normal small-board range used by low-cost PCB manufacturers. The square outline is also convenient for first-spin debugging because it gives sufficient room for the DWM3001C-CDK style footprint, sensor section, power path, fuse, diode, regulator, and test points.

The current placement is a useful first-pass debug arrangement:

|Area|Placement observation|
|-|-|
|U7 / CDK footprint|Large module footprint placed on the left side of the board.|
|U2 / sensor board|Placed at the upper right.|
|U6 / regulator and U3 / switching section|Placed in the central-right power and control area.|
|R1 to R4 and C1 to C3|Grouped near the signal-conditioning and switched-supply area.|
|F1, D1, TP1 to TP5|Grouped near the lower power-entry area.|
|TP6 and TP7|Placed at the right edge near the analogue signal nodes for scope access.|

The layout is now suitable for routing trials once U1 and U7 are corrected.

[Back to contents](#contents)

## Work completed

* Imported TP1 to TP7 into PCB Editor from the schematic.
* Confirmed that the test point footprints are recognised by KiCad.
* Deleted dangling track fragments previously reported by DRC.
* Re-ran DRC after removing dangling tracks.
* Placed the test point footprints on the board area.
* Moved several components to a more coherent pre-routing arrangement.
* Reviewed KiCad Appearance settings and confirmed that ratsnest display is enabled.
* Confirmed that the current 80 mm x 80 mm board outline is a normal, easy-to-manufacture prototype size.
* Diagnosed the input power connector footprint after observing only one ratsnest line.
* Identified that U7 / CDK currently has no obvious ratsnest lines, indicating a probable schematic-symbol, footprint-pad, or net-assignment mismatch.
* Established a first-pass manual routing order for power, switched power, gate control, sensor output, ADC node, and ground.

[Back to contents](#contents)

## Test point implementation

The test point set is now physically present in PCB Editor. The useful diagnostic mapping is:

|Test point|Net|Purpose|
|-|-|-|
|TP1|RAW\_IN / input positive|Raw input voltage before fuse and protection path.|
|TP2|D1 anode side|Fuse output / diode input side.|
|TP3|D1 cathode side|Protected input rail after reverse-polarity diode.|
|TP4|3V3+|Regulator output / main 3.3 V rail.|
|TP5|GND|Ground reference test point.|
|TP6|ACC\_VOUT|Raw ADXL1005 sensor output before the RC filter.|
|TP7|ADC\_NODE|Filtered accelerometer signal at the ADC input node.|

This measurement arrangement separates power-entry debugging from signal-chain debugging. TP6 and TP7 are particularly valuable because they allow direct comparison between the raw ADXL1005 output and the conditioned ADC input node.

[Back to contents](#contents)

## Latest DRC status

The latest DRC report, after the current placement update, shows:

```text
Found 0 DRC violations
Found 27 unconnected pads
Found 0 Footprint errors
```

This is a meaningful improvement over the earlier state. The previous dangling-track warnings are gone, and the earlier DRC violations have been cleared. The board is still unrouted, so the 27 unconnected pads are expected at this stage.

The unconnected-pad list currently describes the unrouted electrical map rather than a failed layout. It includes the expected power-entry, regulator, ground, switched-supply, PFET gate, accelerometer output, and ADC-node connections.

Key unrouted nets include:

```text
Net-(U1-RAW\_IN\_{slash}\_+)
Net-(D1-A)
Net-(D1-K)
/3V3+
/GND
/ACC\_3V3\_SW
/PFET\_GATE
/ACC\_VOUT
/ADC\_NODE
```

The latest DRC report does not show U7 / CDK in the unconnected-pad list, which is important. If the CDK is intended to connect to ADC\_NODE, PFET\_GATE, GND, or other Thin-Pod nets, then its absence from the DRC connectivity list suggests that KiCad currently sees U7 as unconnected or electrically undefined.

[Back to contents](#contents)

## Power connector footprint issue

The input connector remains one of the two most important unresolved items.

The visible PCB footprint still appears to show:

```text
Pad 1 = electrical pad, labelled '+'
MP pads = mechanical pads only
No electrical pad 2 visible for GND / negative
```

This means the placed footprint is still effectively a one-electrical-pin connector in the PCB view. The two large pads marked 'MP' are mechanical pads and do not replace the missing negative power pin. As a result, KiCad correctly shows only one ratsnest connection from the connector.

A correct two-wire Thin-Pod power connector should show two numbered electrical pads:

```text
Pad 1 = RAW\_IN / +
Pad 2 = GND / -
```

The intended footprint family for a vertical JST-PH 2-pin connector is:

```text
Connector\_JST:JST\_PH\_B2B-PH-K\_1x02\_P2.00mm\_Vertical
```

The corresponding right-angle version is:

```text
Connector\_JST:JST\_PH\_S2B-PH-K\_1x02\_P2.00mm\_Horizontal
```

The next session should confirm whether only the 3D model path was changed, rather than the actual footprint, or whether the schematic symbol is still a one-pin symbol. The schematic symbol should be a two-pin connector, for example:

```text
Connector\_Generic:Conn\_01x02
```

with pin 1 connected to RAW\_IN / + and pin 2 connected to GND / -.

[Back to contents](#contents)

## CDK / U7 connectivity issue

The CDK / U7 footprint currently shows no obvious blue ratsnest lines. The latest DRC report also does not list U7 among the unconnected pads. This strongly suggests that KiCad does not currently see U7 as having any electrical pads assigned to the relevant schematic nets.

Likely causes:

* U7 is only a mechanical or placeholder footprint.
* The U7 schematic symbol has no wires connected to its pins.
* The schematic symbol pin numbers do not match the PCB footprint pad numbers.
* The CDK footprint pads exist, but are assigned `<no net>`.
* The intended pins may be labelled descriptively rather than numbered in a way KiCad can match.

The important diagnostic check is to click a CDK header pad in PCB Editor and inspect its net field. If expected pads report `<no net>`, the issue is not visual ratsnest display. It is a schematic-to-footprint connectivity problem.

For the current Thin-Pod concept, the CDK should probably expose at least these relevant connections, subject to the final chosen pin map:

```text
CDK ADC input pin -> /ADC\_NODE
CDK GPIO/control pin -> /PFET\_GATE
CDK GND -> /GND
CDK power/reference pin -> /3V3+ if required by the chosen architecture
```

KiCad maps schematic pins to PCB pads by pin/pad number, not by human-readable labels. Therefore, the U7 symbol and U7 footprint must use matching pin/pad identifiers. For example, if the schematic uses pin `15`, the footprint pad must also be `15`, unless the custom symbol and footprint consistently use another matching convention such as `J10\_15`.

[Back to contents](#contents)

## Routing strategy established

Routing should not begin until U1 and U7 have been corrected. Once those issues are resolved, the recommended routing order is:

1. RAW\_IN from the power connector to F1 input side and TP1.
2. F1 output side to D1 anode and TP2.
3. D1 cathode to protected rail / regulator input and TP3.
4. Regulator 3.3 V output to TP4 and the main 3V3+ loads.
5. ACC\_3V3\_SW from the PFET-switched rail to the accelerometer supply and decoupling capacitors.
6. PFET\_GATE between the pull network and PFET gate.
7. ACC\_VOUT from ADXL1005 output to R1 input side and TP6.
8. ADC\_NODE from R1 output side to C1, R4 / ADC input, TP7, and the intended CDK ADC pin.
9. GND using a copper zone where possible, then short direct connections where required.

Suggested track widths:

```text
RAW\_IN, 3V3+, ACC\_3V3\_SW: 0.5 mm to 1.0 mm where space allows
PFET\_GATE, ACC\_VOUT, ADC\_NODE: 0.25 mm to 0.3 mm
```

For the sensor signal path, the intended topology is:

```text
ADXL1005 VOUT -> R1 -> ADC\_NODE -> ADC input
                    |
                    C1 to GND
```

TP6 should connect on the raw sensor-output side of R1. TP7 should connect on the filtered ADC-node side of R1.

[Back to contents](#contents)

## Open issues

* U1 power connector footprint still appears to have only one electrical pad.
* U1 schematic symbol may still be a one-pin connector or may not have pin 2 wired to GND.
* U7 / CDK currently has no visible ratsnest connections.
* U7 may be a mechanical placeholder, may have `<no net>` pads, or may have pin/pad numbering that does not match the schematic symbol.
* Routing has not yet started, so 27 unconnected pads remain.
* GND zone strategy still needs implementation or confirmation.
* F1 has paired same-net pads that may require short copper joins between equivalent terminal holes.
* U1 and U7 should be resolved before committing to power-entry or signal routing.

[Back to contents](#contents)

## Next session checklist

1. Inspect U1 in PCB Editor and confirm the actual footprint name, not just the 3D model path.
2. Inspect U1 in Schematic Editor and confirm that the symbol has two electrical pins.
3. Ensure:

```text
U1 pin 1 -> RAW\_IN / +
U1 pin 2 -> GND / -
```

4. Assign or reassign the correct two-pin JST-PH footprint.
5. Update PCB from schematic, allowing footprint changes.
6. Confirm that U1 now shows two electrical pads, pad 1 and pad 2, plus any mechanical pads.
7. Inspect a representative U7 / CDK pad in PCB Editor and check its net field.
8. Confirm that the U7 schematic symbol has the intended pins wired to ADC\_NODE, PFET\_GATE, GND, and any required power/reference nets.
9. Confirm that the U7 schematic pin numbers match the U7 footprint pad numbers exactly.
10. Update PCB from schematic after correcting U7.
11. Confirm that U7 now shows ratsnest lines to the relevant Thin-Pod nets.
12. Begin routing with the corrected power-entry chain:

```text
U1 -> F1 -> D1 -> regulator input
```

13. Route TP1, TP2, and TP3 into the power-entry path.
14. Route 3V3+, ACC\_3V3\_SW, PFET\_GATE, ACC\_VOUT, and ADC\_NODE.
15. Add or refill the GND copper zone.
16. Run DRC again and log the new error count.

[Back to contents](#contents)

## Notes

The board has moved past the earlier issue of missing or unimported test points. The present state is a clean pre-routing layout with test points placed, no DRC violations, no footprint errors, and 27 expected unconnected pads. The primary next work is not routing itself, but correcting U1 and verifying U7 connectivity.

The most important lesson from the evening is that KiCad ratsnest absence is diagnostic. If a footprint has no blue lines and does not appear in the unconnected-pad list, it may not be electrically participating in the netlist at all. This applies directly to the current CDK / U7 placement and should be checked before any tracks are committed.

[Back to contents](#contents)

