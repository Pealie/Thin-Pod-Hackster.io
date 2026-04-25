# 62 Engineering Log

**Project:** Thin-Pod  
**Date:** 25 April 2026  
**Session focus:** KiCad PCB Editor bring-up, test point placement, DRC interpretation, and power connector footprint diagnosis.

[Back to contents](#contents)

## Contents

1. [Overview](#overview)
2. [Work completed](#work-completed)
3. [Test point implementation](#test-point-implementation)
4. [DRC status](#drc-status)
5. [Power connector footprint issue](#power-connector-footprint-issue)
6. [Routing strategy established](#routing-strategy-established)
7. [Open issues](#open-issues)
8. [Next session checklist](#next-session-checklist)
9. [Notes](#notes)

[Back to contents](#contents)

## Overview

This session moved the Thin-Pod PCB layout from schematic synchronisation and missing test point investigation into practical PCB placement and routing preparation. The main technical outcome was confirmation that the newly added test points are now present in PCB Editor, with no footprint errors reported by DRC. The remaining board state is primarily unrouted rather than structurally broken.

The session also identified a significant power-entry footprint issue. The currently visible input connector footprint shows only one electrical pad and mechanical pads, meaning it cannot represent a true two-wire power input. This explains why only one ratsnest line appears from the connector. The issue is not a ratsnest display problem, but an electrical footprint or schematic-symbol mismatch.

[Back to contents](#contents)

## Work completed

* Imported test points into PCB Editor from the schematic.
* Confirmed that the test point footprints are recognised by KiCad.
* Deleted dangling track fragments previously reported by DRC.
* Re-ran DRC after removing dangling tracks.
* Placed the test point footprints on the board area.
* Moved several components slightly to improve layout readiness.
* Reviewed KiCad Appearance settings and confirmed that ratsnest display is enabled.
* Diagnosed the input power connector footprint after observing only one ratsnest line.
* Established a first-pass manual routing order for power, switched power, gate control, sensor output, ADC node, and ground.

[Back to contents](#contents)

## Test point implementation

The test point set is now physically present in PCB Editor. The useful diagnostic mapping is:

|Test point|Net|Purpose|
|-|-|-|
|TP1|RAW\_IN / input positive|Raw input voltage before fuse and protection path|
|TP2|D1 anode side|Fuse output / diode input side|
|TP3|D1 cathode side|Protected input rail after reverse-polarity diode|
|TP4|3V3+|Regulator output / main 3.3 V rail|
|TP5|GND|Ground reference test point|
|TP6|ACC\_VOUT|Raw ADXL1005 sensor output before the RC filter|
|TP7|ADC\_NODE|Filtered accelerometer signal at the ADC input node|

This is a useful measurement arrangement because it separates power-entry debugging from signal-chain debugging. TP6 and TP7 are particularly valuable because they allow direct comparison between the raw ADXL1005 output and the conditioned ADC input node.

[Back to contents](#contents)

## DRC status

The latest DRC state after component movement shows:

```text
Found 5 DRC violations
Found 27 unconnected pads
Found 0 Footprint errors
```

The important improvement is that there are now no footprint errors. The earlier dangling-track warnings have also been cleared. The remaining unconnected pads are expected at this stage because routing has not yet been performed.

The current DRC warnings are mainly secondary layout clean-up items:

* U3 footprint mismatch with the library copy.
* U1 silkscreen clearance overlap.
* U2 reference field clipped by the board edge.

These are not immediate blockers for first-pass routing. They should be corrected after the electrical routing is stable.

[Back to contents](#contents)

## Power connector footprint issue

The input connector remains the most important unresolved item.

The visible PCB footprint shows:

```text
Pad 1 = electrical pad, labelled '+'
MP pads = mechanical pads only
No electrical pad 2 visible for GND / negative
```

This means the currently placed footprint is still effectively a one-electrical-pin connector in the PCB view. The two large pads marked 'MP' are mechanical pads and do not replace the missing negative power pin. As a result, KiCad correctly shows only one ratsnest connection from the connector.

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

The next session should confirm whether only the 3D model was changed, rather than the actual footprint, or whether the schematic symbol is still a one-pin symbol. The schematic symbol should be a two-pin connector, for example:

```text
Connector\_Generic:Conn\_01x02
```

with pin 1 connected to RAW\_IN / + and pin 2 connected to GND / -.

[Back to contents](#contents)

## Routing strategy established

The recommended routing order is:

1. RAW\_IN from the power connector to F1 input side and TP1.
2. F1 output side to D1 anode and TP2.
3. D1 cathode to protected rail / regulator input and TP3.
4. Regulator 3.3 V output to TP4 and the main 3V3+ loads.
5. ACC\_3V3\_SW from the PFET-switched rail to the accelerometer supply and decoupling capacitors.
6. PFET\_GATE between the pull network and PFET gate.
7. ACC\_VOUT from ADXL1005 output to R1 input side and TP6.
8. ADC\_NODE from R1 output side to C1, R4 / ADC input, and TP7.
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
* Routing has not yet started, so 27 unconnected pads remain.
* GND zone strategy still needs implementation or confirmation.
* U3 local footprint mismatch remains as a warning.
* U1 and U2 silkscreen warnings remain and will need a later cosmetic pass.
* F1 has paired same-net pads that may require short copper joins between equivalent terminal holes.

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
7. Begin routing with the power-entry chain:

```text
U1 -> F1 -> D1 -> regulator input
```

8. Route TP1, TP2, and TP3 into the power-entry path.
9. Route 3V3+, ACC\_3V3\_SW, PFET\_GATE, ACC\_VOUT, and ADC\_NODE.
10. Add or refill the GND copper zone.
11. Run DRC again and log the new error count.

[Back to contents](#contents)

## Notes

The board has moved past the earlier issue of missing or unimported test points. The primary task now is to correct the power connector so that the board has a valid two-wire input, then proceed with first-pass routing. The present DRC state is therefore acceptable as an intermediate layout stage, but the U1 connector must be resolved before committing to power-entry routing.

[Back to contents](#contents)

