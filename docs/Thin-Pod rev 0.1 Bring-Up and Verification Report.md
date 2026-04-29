# Thin-Pod rev 0.1 Bring-Up and Verification Report

**Project:** Thin-Pod  
**Board revision:** rev 0.1  
**Document date:** 29 April 2026  
**Board status:** PCB ordered, awaiting delivery  
**Purpose:** Prepare a structured bring-up and verification process before the first Thin-Pod PCB arrives.

\---

## 1\. Aim

This document defines the bring-up and verification plan for the first Thin-Pod rev 0.1 PCB.

The aim is to move the project beyond successful PCB design and into measured engineering validation. The report is intended to capture expected behaviour, measured results, pass/fail evidence, faults found, corrective action and design lessons for a future rev 0.2 board.

The immediate validation goals are:

```text
1. Confirm the manufactured PCB matches the intended design.
2. Confirm there are no obvious fabrication faults before assembly.
3. Confirm the power-entry, fuse, reverse-protection and regulator path.
4. Confirm the 3.3 V regulated rail.
5. Confirm the PFET-switched accelerometer supply rail.
6. Confirm the ADXL1005 output path.
7. Confirm the filtered ADC node path to the DWM3001C-CDK.
8. Record evidence for design review and rev 0.2 changes.
```

\---

## 2\. Board summary

Thin-Pod rev 0.1 is a prototype open-hardware vibration-sensing node for rotating machinery. It is designed to capture accelerometer data at the edge and send it wirelessly to a gateway for fault detection and predictive maintenance.

The current board is an 80 mm × 80 mm two-layer PCB using through-hole and module-style footprints.

Key design elements:

```text
J1      Right-angle JST-PH RAW\_IN power connector
F1      Fuse holder
D1      1N5817 Schottky diode for reverse-polarity protection
U4      Pololu S7V8F3 3.3 V regulator module
U3      PFET high-side switch for accelerometer supply
U2      ADXL1005 sensor section / module footprint
U5      DWM3001C-CDK interface section
C2      100 nF local decoupling capacitor
C3      10 uF non-polar local bulk decoupling capacitor
TP1-TP7 Electrical test points
```

\---

## 3\. Known final design state before fabrication

The final pre-order status was:

```text
Schematic ERC: clean
PCB DRC: clean
Gerbers: generated and inspected
Drill files: generated and inspected
Manufacturer preview: checked
PCB order: placed with UK manufacturer
Quantity: 1
Lead time: approximately 1 week
```

Important corrections completed before order:

```text
U2 ADXL1005 pin numbering corrected.
U3 PFET footprint made project-local and verified.
C3 changed to non-polar 10 uF capacitor footprint.
J1 changed to right-angle JST-PH connector.
Mounting-hole keep-outs added.
H1 conflict removed.
F1 confirmed not bypassed by copper.
D1 polarity checked.
```

\---

## 4\. Test-point map

|Test point|Net|Purpose|
|-|-|-|
|TP1|RAW\_IN|Input voltage from J1 before fuse|
|TP2|RAW\_FUSED|Input voltage after F1|
|TP3|RAW\_PROT|Protected input after D1, before U4 VIN|
|TP4|3V3+|Regulated 3.3 V output from U4|
|TP5|GND|Ground reference|
|TP6|ACC\_VOUT|Raw accelerometer output|
|TP7|ADC\_NODE|Filtered ADC input node to U5|

Expected power path:

```text
J1 + → /RAW\_IN → F1 → /RAW\_FUSED → D1 → /RAW\_PROT → U4 VIN → /3V3+
```

Expected switched accelerometer rail:

```text
/3V3+ → U3 source
U3 drain → /ACC\_3V3\_SW
/ACC\_3V3\_SW → U2 Vdd\_3V3, C2, C3
```

Expected analogue signal path:

```text
U2 Vout → /ACC\_VOUT → TP6 → R1 → /ADC\_NODE → TP7 → U5 J10\_15
```

Expected control path:

```text
U5 J10\_24 → /PFET\_CTRL → R3 → /PFET\_GATE → U3 gate
```

\---

## 5\. Equipment required

|Equipment|Purpose|
|-|-|
|Digital multimeter|Continuity, resistance and DC voltage checks|
|Current-limited bench supply|Controlled first power-up|
|Oscilloscope|Sensor output, switching behaviour and ADC node checks|
|Fine probes or grabbers|TP1-TP7 measurements|
|Magnification or inspection camera|Solder joint and fabrication inspection|
|Soldering iron and solder|Assembly|
|Flux and solder wick|Rework|
|Small side cutters|Lead trimming|
|Known-good fuse|F1 installation|
|1N5817 diode|D1 installation|
|JST-PH cable or mating connector|Power input|
|Spacers/screws|U5/CDK support and board mounting checks|

Optional:

|Equipment|Purpose|
|-|-|
|Thermal camera or contact thermometer|Regulator, diode and PFET heating checks|
|Logic analyser|PFET\_CTRL verification|
|Signal generator or vibration source|Later accelerometer validation|
|Small motor or fan|Controlled vibration source|

\---

## 6\. Safety and handling notes

Initial power-up must be current-limited. The board should not be powered from an unrestricted supply before basic continuity checks are complete.

Suggested first-power current limit:

```text
Initial current limit: 50 mA to 100 mA
Increase only after rails behave as expected.
```

Basic cautions:

```text
Do not fit all modules before power-entry checks.
Do not connect U5/CDK until the 3.3 V rail has been measured.
Do not connect U2 until the switched rail has been verified.
Check D1 orientation before applying power.
Check RAW\_IN to GND resistance before applying power.
Check that F1 is not bypassed by copper or solder.
```

\---

## 7\. Stage 1: Bare PCB inspection

### 7.1 Visual inspection

|Check|Expected result|Result|Notes|
|-|-|-|-|
|Board outline|80 mm × 80 mm, rounded corners as intended|||
|Surface finish|No obvious fabrication damage|||
|Silkscreen|Labels readable and not clipped|||
|J1 footprint|Right-angle connector footprint present and correctly oriented|||
|F1 footprint|Fuse-holder pads and holes clear|||
|D1 marking|Cathode/K side visible|||
|TP1-TP7|Test points visible and accessible|||
|H holes|Mounting holes clean and correctly drilled|||
|SH holes|U5 spacer holes correctly drilled|||
|Solder mask|Openings align with pads|||
|Copper|No obvious shorts, cuts or under-etched areas|||

### 7.2 Drill and hole inspection

|Area|Expected result|Result|Notes|
|-|-|-|-|
|J1|Holes aligned with pads|||
|F1|Fuse-holder holes aligned|||
|D1|Holes aligned with diode footprint|||
|U2|Pads/holes align with intended module/pins|||
|U4|Pads/holes align with regulator module|||
|U5|Pads/holes align with CDK interface|||
|Mounting holes|Clean and mechanically usable|||
|Vias|Present and clean|||

\---

## 8\. Stage 2: Bare-board continuity checks

Carry out these checks before fitting components.

|Check|Method|Expected result|Measured result|Pass/fail|Notes|
|-|-|-|-|-|-|
|RAW\_IN to GND|Resistance mode|No short||||
|3V3+ to GND|Resistance mode|No short||||
|RAW\_PROT to GND|Resistance mode|No short||||
|ACC\_3V3\_SW to GND|Resistance mode|No short||||
|ACC\_VOUT to GND|Resistance mode|No short||||
|ADC\_NODE to GND|Resistance mode|High resistance path only, according to R4||||
|F1 pad 1 pair|Continuity|Connected together||||
|F1 pad 2 pair|Continuity|Connected together||||
|F1 pad 1 to pad 2|Continuity|Open unless fuse fitted||||
|D1 anode pad to TP2|Continuity|Connected||||
|D1 cathode pad to TP3|Continuity|Connected||||
|U4 VIN to TP3|Continuity|Connected||||
|U4 VOUT to TP4|Continuity|Connected||||
|U5 J10\_15 to TP7|Continuity|Connected through ADC\_NODE||||
|U5 J10\_24 to R3/PFET\_CTRL|Continuity|Connected||||

\---

## 9\. Stage 3: Power-entry assembly and test

Fit only the power-entry and regulator path initially.

Suggested fitted parts:

```text
J1
F1 holder and fuse
D1
U4 regulator module
TP1-TP5 if not already fitted
Relevant power-path passives as required
```

Do not yet fit or connect:

```text
U2 ADXL1005 section
U5 DWM3001C-CDK
Sensitive signal wiring
```

### 9.1 Power-up conditions

|Parameter|Planned value|Actual value|Notes|
|-|-:|-:|-|
|Supply input voltage|3.0 V to 5.0 V initially|||
|Current limit|50 mA to 100 mA initially|||
|Fuse fitted|Yes|||
|D1 fitted|Yes, cathode toward RAW\_PROT/U4 VIN|||

### 9.2 Power rail measurements

Measure using TP5 as GND reference.

|Test point|Net|Expected result|Measured result|Pass/fail|Notes|
|-|-|-|-|-|-|
|TP1|RAW\_IN|Input supply voltage||||
|TP2|RAW\_FUSED|Approximately RAW\_IN||||
|TP3|RAW\_PROT|RAW\_FUSED minus Schottky drop||||
|TP4|3V3+|Approximately 3.3 V||||
|TP5|GND|0 V reference||||

### 9.3 Power-entry observations

|Observation|Expected result|Result|Notes|
|-|-|-|-|
|D1 temperature|No abnormal heating|||
|U4 temperature|No abnormal heating|||
|Input current|Within expected unloaded range|||
|Fuse behaviour|No nuisance opening|||
|3V3+ stability|Stable under light load|||

\---

## 10\. Stage 4: PFET-switched rail verification

Fit or connect the U3 PFET section and relevant gate network.

Expected mapping:

```text
U3 Source → /3V3+
U3 Gate   → /PFET\_GATE
U3 Drain  → /ACC\_3V3\_SW
```

Expected control path:

```text
/PFET\_CTRL → R3 → /PFET\_GATE
R2 provides gate reference according to the design
```

### 10.1 Static checks before power

|Check|Expected result|Measured result|Pass/fail|Notes|
|-|-|-|-|-|
|U3 source to TP4|Continuity to /3V3+||||
|U3 drain to C2/C3 supply side|Continuity to /ACC\_3V3\_SW||||
|U3 gate to R3/R2|Continuity to /PFET\_GATE||||
|ACC\_3V3\_SW to GND|No short||||

### 10.2 Powered checks

|Test|Expected result|Measured result|Pass/fail|Notes|
|-|-|-|-|-|
|3V3+ present at U3 source|Approx. 3.3 V||||
|PFET\_GATE state when disabled|Defined safe level||||
|ACC\_3V3\_SW when disabled|Off or near 0 V, according to control scheme||||
|ACC\_3V3\_SW when enabled|Approx. 3.3 V||||
|Gate transition|Clean enough for switching||||
|Current draw change|Within expected range||||

\---

## 11\. Stage 5: U2 ADXL1005 section verification

Fit or connect U2 and local decoupling.

Corrected U2 pin mapping:

```text
Pin 1  STB       unconnected
Pin 2  ORo       unconnected
Pin 3  Vout      /ACC\_VOUT
Pin 4  ST        unconnected
Pin 5  Gnd       /GND
Pin 6  Vdd\_3V3   /ACC\_3V3\_SW
```

### 11.1 Static checks

|Check|Expected result|Measured result|Pass/fail|Notes|
|-|-|-|-|-|
|U2 pin 6 to ACC\_3V3\_SW|Connected||||
|U2 pin 5 to GND|Connected||||
|U2 pin 3 to TP6|Connected||||
|U2 pin 3 to R1 input side|Connected||||
|U2 pins 1, 2, 4|Deliberately unconnected||||
|C2 between ACC\_3V3\_SW and GND|Correct||||
|C3 between ACC\_3V3\_SW and GND|Correct, non-polar||||

### 11.2 Powered checks

|Test point|Net|Expected result|Measured result|Pass/fail|Notes|
|-|-|-|-|-|-|
|U2 Vdd|ACC\_3V3\_SW|Approx. 3.3 V when enabled||||
|TP6|ACC\_VOUT|Valid sensor output bias/signal||||
|TP7|ADC\_NODE|Filtered version of ACC\_VOUT||||
|C2/C3 rail|ACC\_3V3\_SW|Stable local supply||||

\---

## 12\. Stage 6: ADC\_NODE and U5/CDK interface verification

U5 interface pins of interest:

```text
U5 J1\_1    → /3V3+
U5 J1\_2    → /GND
U5 J10\_15  → /ADC\_NODE
U5 J10\_24  → /PFET\_CTRL
```

### 12.1 Static checks

|Check|Expected result|Measured result|Pass/fail|Notes|
|-|-|-|-|-|
|U5 J1\_1 to TP4|Connected to /3V3+||||
|U5 J1\_2 to TP5|Connected to /GND||||
|U5 J10\_15 to TP7|Connected to /ADC\_NODE||||
|U5 J10\_24 to R3 pad 2|Connected to /PFET\_CTRL||||

### 12.2 Functional checks

|Test|Expected result|Measured result|Pass/fail|Notes|
|-|-|-|-|-|
|U5 powered correctly|3.3 V and GND present||||
|PFET\_CTRL can be toggled|J10\_24 changes state||||
|PFET\_GATE responds|Gate follows through R3 network||||
|ACC\_3V3\_SW responds|Rail switches as intended||||
|ADC\_NODE readable|ADC sees expected voltage/signal||||

\---

## 13\. Stage 7: Initial vibration/signal check

This stage is optional for first power-up but important for the full validation record.

Possible test sources:

```text
Bench vibration from a small motor or fan
Gentle tapping or controlled mechanical excitation
Known vibration source
Signal injection if sensor output path is tested separately
```

|Test|Expected result|Measured result|Pass/fail|Notes|
|-|-|-|-|-|
|TP6 responds to motion/vibration|Visible changing signal||||
|TP7 follows filtered TP6 behaviour|Filtered/smoothed response||||
|ADC captures plausible values|Values change with excitation||||
|Noise level acceptable for first prototype|No severe unexpected noise||||

\---

## 14\. Fault log

|Date|Stage|Fault observed|Suspected cause|Action taken|Result|
|-|-|-|-|-|-|
|||||||

\---

## 15\. Design-change log for rev 0.2

|Item|Evidence from rev 0.1|Proposed rev 0.2 change|Priority|
|-|-|-|-|
|||||

Possible areas for future revision:

```text
Connector placement
Test-point spacing
U5/CDK mechanical support
Regulator module placement
PFET footprint
Sensor signal routing
Grounding strategy
3D model completeness
Board size reduction
Move selected parts to SMD
Improve silkscreen assembly notes
```

\---

## 16\. Pass/fail summary

|Area|Status|Notes|
|-|-|-|
|Bare-board inspection|||
|Continuity checks|||
|Power-entry path|||
|3.3 V regulator output|||
|PFET-switched rail|||
|U2 sensor supply|||
|ACC\_VOUT signal path|||
|ADC\_NODE signal path|||
|U5/CDK interface|||
|Initial vibration/signal response|||

\---

## 17\. Overall conclusion

To be completed after assembly and testing.

The intended conclusion should state whether Thin-Pod rev 0.1 has successfully demonstrated:

```text
A manufacturable PCB design
A working protected power-entry path
A stable 3.3 V regulated rail
A controllable switched accelerometer supply
A valid accelerometer output test point
A valid filtered ADC input test point
A workable path toward UWB gateway integration
```

The completed measurements and findings will determine whether rev 0.1 is suitable for continued firmware and signal-chain validation, or whether a rev 0.2 PCB should be designed before further integration work.

