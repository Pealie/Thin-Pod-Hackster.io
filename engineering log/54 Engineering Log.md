# Engineering Log — DWM3001C-DK mechanical measurements (calipers)

## Summary
Mechanical measurements for the Qorvo DWM3001C-DK (CDK) were captured from caliper photos, collated, and written up as a reusable dimension note for mating-PCB and fixture design. A pin-reference correction was applied for the Raspberry Pi header focus, updating the note to target J10 pins **15** and **16**.

## Work completed

### 1) Board outline and antenna footprint captured
Caliper readouts were extracted from the photos and converted into a practical board-envelope for mechanical keep-out planning:

- **PCB-only outline:** **54.23 mm × 43.17 mm**
- **Antenna tab projection (photo-derived):** **19.49 mm**
- **Overall envelope including antenna (photo-derived):** **62.63 mm × 54.23 mm**
  - Cross-check: **43.17 + 19.49 = 62.66 mm**, consistent with the **62.63 mm** readout within expected alignment tolerance.

### 2) Header geometry documentation tightened for alignment planning
- J1 (+/−) and J10 (Pi header) were documented as **2.54 mm pitch** interfaces for mechanical assumptions and early CAD layout work.
- Pin-reference correction applied: J10 focus is **pins 15 and 16** (not pins 25 and 16).

### 3) Additional caliper readings captured for header localisation
Additional caliper photos were captured to support header localisation work. The displays in the latest images show the following readings (units: mm):

- **-21.27**
- **-12.65**
- **23.48**
- **8.90**

These readings were retained in the dimension note as raw evidence for subsequent interpretation, alongside the earlier board-outline measurements.

## Artefacts
- `dwm3001cdk_dimensions_from_photos.md`  
  Consolidated dimension note containing raw photo readouts plus derived board-envelope dimensions, with the J10 reference corrected to **pins 15 and 16**.

## Notes
- The caliper photos provide robust board-envelope dimensions directly from the display.
- For high-confidence mating-board alignment, the next best photo set would place caliper jaws on unambiguous fiducials (PCB edge, mounting-hole centre, or specific pin centres), then record both:
  - edge-to-pin centre offsets, and
  - hole-centre-to-pin centre offsets.
