# Engineering Log 62

## Summary

Weekend work focused on stabilising the KiCad PCB layout for Thin-Pod after a full PCB reset and re-import from schematic. Board geometry, copper pours, and DRC hygiene were brought back under control, with the remaining work reduced to a small set of explicit unrouted nets.

## Key outcomes

### PCB recovery and board geometry

* The PCB was cleared and rebuilt via ‘Update PCB from schematic’ to re-establish a clean baseline of footprints and net assignments.
* Board outline workflow was re-established using Edge.Cuts graphics, including handling of malformed outline behaviours and outline persistence.

### Ground copper pour restored and validated

* A functioning ground pour was reintroduced using the filled zone workflow (Z), including correct zone net assignment to **/GND** rather than ‘GND’.
* Zone fill behaviour was verified via zone listing, visual confirmation of copper, and DRC reduction of /GND-related unconnected items.
* The zone workflow was documented in practice: create zone, set net and layer, then refill using B.

### DRC triage and failure mode isolation

* DRC reports were used iteratively to distinguish genuine shorts, solder mask aperture bridges, and simple unrouted connections.
* A recurring issue was identified: tracks ending a fraction short of pads, producing ‘missing connection between items’ errors that required snapping the route into the pad rather than merely approaching it.
* Net highlighting and temporarily hiding zones were used to reveal obscured track ends on sensitive nets such as /ADC\_NODE.

### Thermal connection issue on C3

* A ‘starved thermal’ condition on C3 pad 1 (/GND) was investigated.
* Component movement experiments confirmed that local copper congestion could both relieve and worsen the condition, including a case where a move introduced an actual short between C3 (/GND) and /ACC\_3V3\_SW.
* Pad-level mitigation was applied by setting C3 pad 1 zone connection to **Solid**, separating thermal-spoke constraints from genuine clearance and shorting issues.

### Remaining work scoped cleanly

* Following the pour restore and DRC cleanup, remaining items were narrowed to a small set of explicit unrouted connections called out by DRC (notably involving /ADC\_NODE, /RAW\_IN, and /RAW\_PROT), ready for direct resolution by zooming to each marker and completing the route into the named pad.

## Notes

* The distinction between ‘GND’ and ‘/GND’ is operationally significant in KiCad. Zone net selection must match the pad net name exactly, otherwise the pour will appear present but will not electrically connect.
* ‘Update PCB from schematic’ restores footprints and nets, not tracks, vias, or zones. Post-update routing and pours are expected tasks rather than faults.
