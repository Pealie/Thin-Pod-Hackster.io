# 64 Engineering Log

**Project:** Thin-Pod  
**Date:** 26 April 2026  
**Focus:** KiCad schematic/PCB synchronisation, net naming, CDK footprint mapping, and first routing preparation

## 1\. Session overview

This session brought the Thin-Pod PCB from a fragile pre-routing state into a much cleaner, better documented, and more robust schematic-to-PCB state. The main work was not copper routing itself, but the correction of several foundational issues that would otherwise have made routing unreliable: connector footprint assignment, CDK symbol-to-footprint mapping, persistent net naming, component reference designators, and PCB DRC readability.

By the end of the session, the schematic was reporting no DRC/ERC errors, the PCB Editor DRC showed no DRC violations or footprint errors, and the remaining PCB issues were ordinary unrouted ratsnest connections. This is a significant stabilisation point before completing the first copper routing pass.

## 2\. Board outline and manufacturing context

The board outline is now treated as an **80 mm × 80 mm** square prototype PCB. This was assessed as a straightforward, easy-to-manufacture size for a small prototype run. It is not a special standard size, but it sits comfortably within normal low-cost prototype PCB manufacturing expectations and provides useful debug space for this first spin.

The extra board area is currently advantageous because the design includes a CDK/module footprint, an accelerometer/sensor section, a regulator section, a fuse and protection path, and several test points. Compactness can be improved in a later revision once the circuit and mechanical arrangement are proven.

## 3\. Reference designator clean-up

The reference designators were rationalised so the schematic and PCB are easier to interpret.

The JST power connector was corrected from a generic `U` style reference to a connector-style reference:

```text
J1 = JST-PH 2-pin RAW\_IN power connector
```

The module and active-device references were cleaned up so the main functional blocks now read more coherently:

```text
U2 = ADXL1005 / accelerometer sensor section
U3 = PFET / switched accelerometer rail control device
U4 = S7V8F3 regulator module
U5 = DWM3001C-CDK
```

This removes misleading gaps and makes the schematic, PCB silkscreen, DRC output, and future BOM more readable.

## 4\. JST power connector correction

The earlier power connector issue was resolved. The previous footprint appeared to contain only one electrical pad plus mechanical pads, which explained why only one ratsnest line was visible for the supposed two-wire power input.

The connector has now been corrected to a proper two-pin JST-PH style input. PCB DRC now recognises both electrical pads:

```text
J1 pad 1 = /RAW\_IN
J1 pad 2 = /GND
```

This confirms that the board now has both the positive input and return path represented electrically. This correction was important before routing, since an incomplete power connector would have invalidated the input power chain.

## 5\. CDK/U5 net persistence issue resolved

A recurring issue occurred where the DWM3001C-CDK blue ratsnest lines would appear, then disappear after updating the PCB from the schematic. The underlying cause was traced to schematic-symbol/library/footprint mapping rather than PCB display settings.

The resolution was to ensure that the CDK schematic symbol and PCB footprint use matching pin/pad numbers. The important pin-number mapping is now:

```text
Symbol pin number     Footprint pad number
J1\_1                  J1\_1
J1\_2                  J1\_2
J10\_15                J10\_15
J10\_24                J10\_24
```

The corresponding nets are now anchored from the schematic and survive PCB updates:

```text
U5 J1\_1    = /3V3+
U5 J1\_2    = /GND
U5 J10\_15  = /ADC\_NODE
U5 J10\_24  = /PFET\_CTRL
```

The library symbol was also updated so the placed schematic symbol no longer diverges from the library copy. Pin display names were tidied for readability, including `GND` and `3V3+`, while preserving the exact pin numbers needed for footprint mapping.

## 6\. PFET control net clarified

The CDK GPIO/control pin was separated from the actual PFET gate node by naming the CDK-side control net:

```text
/PFET\_CTRL
```

This leaves the gate-side node as:

```text
/PFET\_GATE
```

The resulting control path is now clearer:

```text
U5 J10\_24 → /PFET\_CTRL → R3 → /PFET\_GATE → U3 gate
```

This is more accurate than using the same net name on both sides of the series resistor, since R3 intentionally separates the CDK output side from the PFET gate node.

## 7\. Power-path net naming improved

The power entry and protection chain was renamed into a readable sequence. The current named chain is:

```text
/RAW\_IN → F1 → /RAW\_FUSED → D1 → /RAW\_PROT → U4 Vin
```

The meaning is:

```text
RAW\_IN     = input voltage from J1
RAW\_FUSED  = input voltage after fuse F1
RAW\_PROT   = input voltage after reverse-polarity protection diode D1
```

This replaces autogenerated KiCad names such as `Net-(D1-A)` and `Net-(D1-K)` with intentional project-level net names. The DRC report is now much more useful as a routing checklist.

## 8\. Test-point structure confirmed

The diagnostic test points are now correctly present in the PCB and assigned to meaningful nets. The current functional grouping is:

```text
TP1 = /RAW\_IN
TP2 = /RAW\_FUSED
TP3 = /RAW\_PROT
TP4 = /3V3+
TP5 = /GND
TP6 = /ACC\_VOUT
TP7 = /ADC\_NODE
```

The sensor-side test points are particularly useful:

```text
TP6 = raw accelerometer output before the RC filter
TP7 = filtered ADC node going to the CDK ADC input
```

This should support bench testing with a scope by allowing comparison between the raw ADXL1005 output and the filtered ADC input.

## 9\. Current schematic state

The schematic now reports no DRC/ERC errors. The main named nets present in the design are:

```text
RAW\_IN
RAW\_FUSED
RAW\_PROT
3V3+
GND
ACC\_3V3\_SW
ACC\_VOUT
ADC\_NODE
PFET\_CTRL
PFET\_GATE
```

No further net labels appear necessary before routing. The schematic now has clear labels for power entry, protection, regulated output, switched accelerometer supply, sensor output, filtered ADC input, and CDK control.

## 10\. Current PCB DRC state

After updating the PCB from the corrected schematic, the PCB DRC reached a clean pre-routing state. Following the first two short fuse-holder pad-pair tracks, the latest DRC report showed:

```text
Found 0 DRC violations
Found 27 unconnected pads
Found 0 Footprint errors
```

The remaining unconnected items are expected at this stage. They represent ordinary unrouted nets rather than schematic, footprint, or clearance faults.

## 11\. Routing started: fuse-holder pad-pair bridges

Routing has begun with the fuse holder. The Littelfuse-style fuse footprint has two same-net pad pairs:

```text
F1 pad 1 pair = /RAW\_IN
F1 pad 2 pair = /RAW\_FUSED
```

The two same-number pad pairs were connected with short tracks. This reduced the unconnected-pad count from 29 to 27, confirming that the first routing step was successful.

A key routing rule was established:

```text
Do not route directly between F1 pad 1 and F1 pad 2.
```

Those two sides must remain separated by the physical fuse. A copper track across them would bypass the fuse and defeat its purpose.

## 12\. Track-width guidance established

A routing convention was established for the first pass:

```text
0.75 mm to 1.0 mm:
RAW\_IN
RAW\_FUSED
RAW\_PROT
3V3+
ACC\_3V3\_SW, where space allows

0.25 mm to 0.30 mm:
PFET\_CTRL
PFET\_GATE
ACC\_VOUT
ADC\_NODE
```

The wider tracks are intended for power paths. Narrower tracks are sufficient for control and analogue signal paths. Where 1.0 mm is too wide for a gap, 0.75 mm or 0.5 mm can be used locally.

## 13\. Recommended next routing sequence

The next routing pass should proceed in this order:

```text
1. /RAW\_IN
   J1 pad 1 → TP1 → F1 pad 1 pair

2. /RAW\_FUSED
   F1 pad 2 pair → TP2 → D1 anode

3. /RAW\_PROT
   D1 cathode → TP3 → U4 Vin

4. /3V3+
   U4 Vout → TP4 → U5 J1\_1

5. /GND
   J1 pad 2 → TP5 → U5 J1\_2 → U4 GND → sensor/filter grounds
   A GND copper zone is preferred.

6. /ACC\_3V3\_SW
   U3 switched rail → C2/C3 → U2 supply

7. /PFET\_CTRL
   U5 J10\_24 → R3 pad 2

8. /PFET\_GATE
   R3 pad 1 → R2 pad 1 → U3 gate

9. /ACC\_VOUT
   U2 VOUT → TP6 → R1 pad 2

10. /ADC\_NODE
    U5 J10\_15 → R1 pad 1 → C1/R4 → TP7
```

## 14\. High-level outcome

The day’s main achievement was not yet a routed board, but a reliable foundation for routing. The schematic and PCB now agree on connector pins, CDK pins, named nets, test points, and reference designators. The most error-prone parts of the design, especially the CDK symbol-to-footprint mapping and the JST power connector, have been corrected.

The design is now ready for a systematic first routing pass. The remaining DRC output should be treated as a routing checklist rather than a fault list.

## 15\. Next session checklist

```text
1. Continue routing from the power entry chain.
2. Use wider tracks on RAW\_IN, RAW\_FUSED, RAW\_PROT, 3V3+, and ACC\_3V3\_SW.
3. Keep the fuse sides electrically separated except through the fuse itself.
4. Add or refill the GND copper zone after key routing is complete.
5. Route PFET\_CTRL and PFET\_GATE as distinct nets.
6. Route ACC\_VOUT and ADC\_NODE cleanly and keep the analogue path short.
7. Run PCB DRC after each major routing group.
8. Update this log once the unconnected-pad count falls significantly.
```

