# Qorvo DWM3001C-DK (CDK) — dimensions from caliper photos

## Raw caliper readouts observed in the photos

|Photo (filename)|What the caliper appears to be measuring|Reading (mm)|
|-|-:|-:|
|ED7EE7E2-066B-4EAE-812F-5D1568E23299.jpeg|Overall dimension **including the antenna tab** (outside-to-outside)|**62.63**|
|11AE8079-058E-4E1A-804D-3E07E82857AF.jpeg|PCB **overall length/width excluding antenna tab** (outside-to-outside of PCB)|**54.23**|
|7756AE82-784A-424D-9349-239CE12C242D.jpeg|PCB **other side dimension** (outside-to-outside)|**43.17**|
|5B5E8B64-99E7-44C7-9115-6938B0538E7E.jpeg|Antenna-tab **projection** beyond the PCB edge (interpreted)|**19.49**|

## Board outline dimensions (derived)

### PCB only (no antenna tab)

* **PCB width/length:** **54.23 mm**
* **PCB height/other side:** **43.17 mm**

### Antenna tab

* **Antenna tab projection beyond PCB edge (as photographed):** **19.49 mm**

### Overall board footprint (including antenna tab)

* **Overall dimension in antenna direction:** **62.63 mm**  
  (This is consistent with **43.17 + 19.49 = 62.66 mm**, within expected photo/alignment tolerance.)
* **Overall dimension orthogonal to antenna direction:** **54.23 mm**

So, a practical “keep-out” rectangle that covers everything visible (PCB + antenna tab) is approximately:

* **62.6 mm × 54.2 mm**

## J1 (+/−) header spacing (mechanical)

The caliper photos provided do not show J1 being directly measured, but on the CDK the J1 (+/−) header is a standard **0.1 inch (2.54 mm) pitch** header footprint.

* **J1 (+) to J1 (−) centre-to-centre spacing:** **2.54 mm**

## J10 ‘Raspberry Pi’ header spacing (mechanical)

Again, the photos do not show a caliper measurement placed on the J10 pins themselves, but a Raspberry Pi compatible header uses **2.54 mm pitch**.

Assuming the standard **2×20** (40-pin) Pi header geometry:

* **Pitch along a row (adjacent pins in the same row):** **2.54 mm**
* **Row-to-row spacing:** **2.54 mm**
* **Header length (pins 1→39 or 2→40):** **19 × 2.54 = 48.26 mm**

### Separation between Pi pins 16 and 25 (assuming standard Pi numbering)

* Pin **16** is on the *even* row, position 8
* Pin **25** is on the *odd* row, position 13
* **Along-row offset:** **5 × 2.54 = 12.70 mm**
* **Across-row offset:** **2.54 mm**
* **Straight-line distance:** **√(12.70² + 2.54²) ≈ 12.95 mm**

## Notes and limits of what can be extracted from these photos

* The four photos provide strong, readable **overall PCB and antenna-tab** dimensions via the caliper display.
* They do **not** show the caliper jaws placed on **J1** or on **specific J10 pins** (e.g., pin 16 and pin 25), so **positional offsets** (distance from PCB edges/mounting holes to those pins) cannot be stated confidently from these images alone.
* If positional offsets are needed for mating-PCB alignment, the most reliable next set of photos would be:

  1. Calipers measuring from a **known PCB edge** to the **centre of J1+** and **J1−**
  2. Calipers measuring from a **known PCB edge** (or a mounting-hole centre) to the **centre of J10 pin 16** and **pin 25**
