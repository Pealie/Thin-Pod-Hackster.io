# 64b Engineering Log

**Project:** Thin-Pod  
**Date:** 2026-04-26  
**Session focus:** PCB routing completion, ground-zone strategy, diode polarity confirmation, and remaining footprint-library clean-up.

## 1\. High-level summary

This session moved the Thin-Pod PCB from a partially routed, congested state to a substantially complete routed board. The central achievement was resolving the apparent routing deadlock by accepting a normal two-layer PCB workflow: front-layer routing for local component connections, back-layer copper for selected escapes and crossovers, and a B.Cu ground zone to simplify the return network.

The PCB now has all required electrical connections recognised by KiCad. The latest routing check reported no unconnected pads and no footprint errors. One remaining DRC item persists: U3 does not match the library copy from `Connector\_Wire`, because the PFET footprint was locally edited to make the gate, source, and drain pad mapping electrically meaningful. This is now the principal clean-up item before treating the layout as fully release-ready.

## 2\. Power-entry and diode orientation

The D1 diode orientation was reviewed directly in PCB Editor. The cathode side was identified from the `K` marking and the diode body stripe/bar. The left pad of D1 is the anode and is connected to `/RAW\_FUSED`; the right pad is the cathode and is connected to `/RAW\_PROT`.

The intended power path remains:

```text
J1 + → /RAW\_IN → TP1 → F1 → /RAW\_FUSED → D1 anode
D1 cathode → /RAW\_PROT → TP3 → U4 VIN
```

This confirms that the 1N5817 Schottky diode is oriented as the series reverse-polarity protection element, with the striped/cathode end facing the protected regulator input rail.

## 3\. Ground-zone routing strategy

A B.Cu `/GND` zone was added to relieve routing congestion and reduce the number of explicit ground tracks required. This materially improved the routing problem: the unconnected count dropped after the zone was added and refilled.

The ground zone also gives the analogue signal path a better local return reference than a set of long, narrow, hand-routed ground traces. This is especially useful around the accelerometer output filter and ADC input node.

The B.Cu zone should remain subject to final inspection around:

```text
U5 CDK area
U2 sensor area
R1/C1/R4 ADC filter area
U4 regulator area
J1/F1/D1 power-entry path
```

## 4\. Completion of remaining electrical routes

The difficult remaining nets were routed after the B.Cu zone was introduced. These included:

```text
/ACC\_3V3\_SW
/PFET\_CTRL
/PFET\_GATE
/ACC\_VOUT
/ADC\_NODE
/3V3+
```

The switched accelerometer rail was clarified as:

```text
/3V3+ → U3 source
U3 drain → /ACC\_3V3\_SW
/ACC\_3V3\_SW → U2 Vdd and C2/C3 supply-side pads
```

This confirms that the accelerometer supply is downstream of the PFET switch rather than permanently tied to the regulator output.

The raw and filtered sensor paths remain:

```text
U2 Vout → /ACC\_VOUT → TP6 → R1 pad 2
R1 pad 1 → /ADC\_NODE → C1/R4/TP7 → U5 J10\_15
```

This provides both raw sensor-output probing and filtered ADC-node probing.

## 5\. Use of back copper and vias

Blue track portions visible in PCB Editor were identified as B.Cu copper tracks, not unresolved ratsnest lines. These were expected and acceptable because the board is now using a two-layer routing strategy.

The practical routing model is now:

```text
Red = F.Cu routed copper
Blue = B.Cu routed copper
Cyan/blue straight airwires = ratsnest, only if still unconnected
```

The latest DRC showed no unconnected pads, so the visible blue segments are routed copper rather than missing connections.

## 6\. DRC status

The latest PCB DRC status reached the important connectivity milestone:

```text
Found 0 unconnected pads
Found 0 Footprint errors
```

The remaining DRC item is:

```text
Footprint U3 does not match copy in library 'Connector\_Wire'
```

This is a library-consistency issue rather than an incomplete-routing issue. It arises because U3 was originally based on a generic three-pad wire/solder footprint, then locally edited in PCB Editor to suit the PFET gate/source/drain mapping.

## 7\. Outstanding clean-up: U3 PFET footprint

The remaining engineering clean-up is to convert the locally edited U3 footprint into a proper project-local Thin-Pod footprint.

Recommended action:

```text
Create project-local footprint:
ThinPod:PFET\_ZVP2106A\_1x03\_GSD
```

The intended pad mapping should be checked and preserved:

```text
U3 S → /3V3+
U3 G → /PFET\_GATE
U3 D → /ACC\_3V3\_SW
```

Once saved into the project library, the U3 symbol in Schematic Editor should be assigned to the new footprint. Then the PCB should be updated from the schematic and DRC rerun. This should clear the footprint-library mismatch warning without suppressing the error or relying on a local board-only footprint modification.

## 8\. Current assessment

The board has reached a meaningful first-route milestone. The schematic is coherent, the main nets are named clearly, the CDK connections persist through schematic-to-PCB updates, the power-entry chain is correctly labelled, and the PCB now routes all known electrical nets.

The remaining work is no longer conceptual connectivity. It is layout hygiene and manufacturing preparation:

```text
1. Replace the locally edited U3 footprint with a project-local PFET footprint.
2. Re-run ERC and PCB DRC.
3. Confirm U3 pad nets one final time.
4. Inspect power-track widths and widen where practical.
5. Inspect the U5 CDK keep-out and antenna area.
6. Review B.Cu GND zone clearance and fragmentation.
7. Generate manufacturing outputs only after a fully clean DRC.
```

## 9\. Notes for next session

The next session should begin with U3 footprint clean-up, not further routing. The PCB now appears electrically connected, so the most valuable next step is to make the footprint libraries reflect the actual design intent. Once U3 is represented by a project-local PFET footprint, the layout can proceed toward final DRC, Gerber generation, drill files, pick/place or position files if required, and a short manufacturing-readiness review.

