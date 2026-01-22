# Qorvo DWM3001C-DK (CDK) — dimensions from caliper photos

## Raw caliper readouts observed in the photos

### Board / antenna dimensions (first photo set)

| Photo (filename) | What the caliper appears to be measuring | Reading (mm) |
|---|---:|---:|
| ED7EE7E2-066B-4EAE-812F-5D1568E23299.jpeg | Overall dimension **including the antenna tab** (outside-to-outside) | **62.63** |
| 11AE8079-058E-4E1A-804D-3E07E82857AF.jpeg | PCB **overall length/width excluding antenna tab** (outside-to-outside of PCB) | **54.23** |
| 7756AE82-784A-424D-9349-239CE12C242D.jpeg | PCB **other side dimension** (outside-to-outside) | **43.17** |
| 5B5E8B64-99E7-44C7-9115-6938B0538E7E.jpeg | Antenna-tab **projection** beyond the PCB edge (interpreted) | **19.49** |

### J1 and J10 pin-location readouts (second photo set, user-labelled)

> These four photos show only the caliper readout (the jaws are not visibly on the features in-frame), so the **numerical values can be recorded**, but the **exact geometric interpretation** (which edge/hole is the reference) cannot be independently verified from the images alone.  
> Negative sign is treated as “direction from the zero reference”, so **use magnitude unless the sign convention is required**.

| Photo (filename) | Label from message | Reading (mm) |
|---|---|---:|
| FE50EC06-6562-435A-817A-BEF2CD3856CB.jpeg | J10 **pin 15** (as measured) | **-21.27** |
| AC7167E7-58A7-478D-A18F-46167481698D.jpeg | J10 **pin 16** (as measured) | **-12.65** |
| 27F3F5FE-308C-428E-94CC-13E68D36FDCD.jpeg | J1 **(+ or −)** (as measured) | **23.48** |
| EBD726FF-507C-4447-A2D1-C021A69C6688.jpeg | J1 **(+ or −)** (as measured) | **8.90** |

## Board outline dimensions (derived)

### PCB only (no antenna tab)
- **PCB width/length:** **54.23 mm**
- **PCB height/other side:** **43.17 mm**

### Antenna tab
- **Antenna tab projection beyond PCB edge (as photographed):** **19.49 mm**

### Overall board footprint (including antenna tab)
- **Overall dimension in antenna direction:** **62.63 mm**  
  (Consistent with **43.17 + 19.49 = 62.66 mm**, within expected photo/alignment tolerance.)
- **Overall dimension orthogonal to antenna direction:** **54.23 mm**

Practical “keep-out” rectangle covering PCB + antenna tab:
- **≈ 62.6 mm × 54.2 mm**

## J1 (+/−) header spacing (mechanical)

Independently of the offset measurements above, if J1 is implemented as a standard 2-pin header footprint:
- **Pitch (centre-to-centre) between adjacent header pins:** **2.54 mm**

The two user-labelled J1 caliper readouts recorded in the second photo set are:
- **23.48 mm** and **8.90 mm** (reference geometry not visible in-frame).

## J10 ‘Raspberry Pi’ header spacing (mechanical)

For the Pi-compatible 2×20 header footprint:
- **Pitch along a row (adjacent pins in the same row):** **2.54 mm**
- **Row-to-row spacing:** **2.54 mm**
- **Header length (pins 1→39 or 2→40):** **19 × 2.54 = 48.26 mm**

### Separation between Pi pins 15 and 16 (standard Pi numbering)
Pin **15** is on the *odd* row and pin **16** is on the *even* row at the same position, so:
- **Along-row offset:** **0 mm**
- **Across-row offset:** **2.54 mm**
- **Straight-line distance (centre-to-centre):** **2.54 mm**

The two user-labelled pin-location readouts recorded in the second photo set are:
- **Pin 15:** **-21.27 mm**
- **Pin 16:** **-12.65 mm**
(reference geometry not visible in-frame).

## Notes / limits

- The first photo set supports **overall PCB** and **overall including antenna tab** dimensions well.
- The second photo set provides numeric readouts for **J1** and **J10 pins 15/16**, but the photos do not show the caliper jaws actually touching the target features, so the numbers are recorded **as given**, without asserting the exact reference edge/hole they were taken from.
