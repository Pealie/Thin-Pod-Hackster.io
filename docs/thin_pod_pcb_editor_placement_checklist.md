## Thin-Pod PCB Editor Placement Checklist

### 1. Board Definition and Mechanical Constraints

Define the board outline as a rectangular carrier matching the perf-board form factor, with rounded corners and a minimum edge clearance of 1.0 mm for copper and silkscreen. Define four plated or non-plated mounting holes aligned to the DWM3001-CDK standoff pattern. Define a copper and component keep-out region under and in front of the DWM3001-CDK antenna section, extending at least 15 mm from the antenna edge, with no copper pours, tracks, vias, or components in this region.

### 2. Power Entry and Protection Placement

Place the JST-PH 2-pin power connector on the board edge at the top-left quadrant to minimise cable strain and enable enclosure routing. Place the optional 2-pin bench header adjacent to the JST connector, sharing the RAW_IN and GND nets. Place the Schottky diode (1N5817) directly downstream of RAW_IN with minimal trace length to define RAW_PROT. Orient the diode to allow visual polarity inspection during bring-up.

### 3. Pololu S7V8F3 Regulator Module Dock

Place the Pololu S7V8F3 module footprint near the power entry and diode to minimise RAW_PROT trace length. Use a 1×4, 2.54 mm through-hole header footprint with Pin 1 designated as SHDN, Pin 2 as VIN, Pin 3 as GND, and Pin 4 as VOUT. Mark Pin 1 clearly with a square pad and silkscreen indicator. Route VIN from RAW_PROT and route VOUT to the 3V3+ distribution network using a wide trace or polygon pour. Place bulk and high-frequency decoupling capacitors (10 µF and 100 nF) within 5 mm of the VOUT pin to GND.

### 4. DWM3001-CDK Docking Interface

Place the mating J10 Raspberry Pi-style header footprint in the right-hand region of the PCB, matching the CDK mechanical drawing for pin pitch, row spacing, and offset. Place the J1 2-pin power header footprint aligned with the CDK J1 connector. Place four mounting holes aligned with the CDK standoffs to allow mechanical fixation. Ensure the antenna keep-out polygon intersects the CDK antenna region and extends beyond the module edge. Route 3V3+ and GND from the regulator directly to J1 with low impedance paths.

### 5. ADXL1005 Breakout Dock

Place the ADXL1005 breakout header footprint in the left-hand region of the PCB, matching the perf-board orientation to preserve cable and mechanical assumptions. Provide a 1×6, 2.54 mm header footprint for Vdd, Vout, GND, ORo, ST, and STB. Add four mounting holes matching the breakout board pattern. Route ACC_3V3_SW to Vdd and GND to the ground plane. Route Vout as ACC_VOUT to the analogue conditioning network with minimal length and minimal coupling to switching or high-current traces.

### 6. PFET High-Side Switch Subsystem

Place the ZVP2106A PFET in SOT-23 near the ADXL1005 breakout to minimise the ACC_3V3_SW distribution path. Connect the PFET source to 3V3+, drain to ACC_3V3_SW, and gate to the PFET_GATE net. Place R3 (1 k) in series between J10.24 (P0.30) and PFET_GATE. Place R4 (1 M) from PFET_GATE to 3V3+ to define the default gate state. Place R2 (100 k) from PFET_GATE to ACC_3V3_SW as per the schematic to stabilise gate behaviour during transitions.

### 7. Analogue Signal Conditioning Network

Place R1 (8 k) in series between ACC_VOUT and ADC_NODE. Place C1 (680 pF) at ADC_NODE with the shortest possible return path to the intended reference node (as per schematic). Locate the conditioning network physically close to the CDK ADC entry pin (J10.15, P0.28) to minimise trace inductance and noise pickup. Route ADC_NODE as a single, continuous trace with no stubs or via transitions where feasible.

### 8. Ground Plane and Stitching Strategy

Implement a continuous ground pour on both PCB layers. Stitch ground vias around the regulator, ADXL1005 breakout, PFET, and ADC conditioning network. Ensure low-impedance ground return paths for the regulator and analogue subsystem. Avoid ground copper in the antenna keep-out region.

### 9. Power Distribution Geometry

Route 3V3+ as a wide trace or polygon pour from the regulator to the CDK J1 header and PFET source. Use ≥ 1 mm trace width or equivalent copper pour for 3V3+ where space permits. Avoid routing 3V3+ through narrow necks that could introduce voltage drop or thermal stress. Place test pads on RAW_IN, RAW_PROT, 3V3+, ACC_3V3_SW, ACC_VOUT, ADC_NODE, PFET_GATE, and GND.

### 10. Silkscreen and Assembly Markings

Label all connectors, test pads, and module docks with reference designators and net names where space permits. Mark Pin 1 on the Pololu S7V8F3 footprint and the DWM3001-CDK headers. Add polarity indicators for the JST power connector. Mark the antenna keep-out zone on silkscreen for assembly and review clarity.

### 11. DRC and ERC Preparation for PCB Editor

Define net classes for power, analogue, and digital signals. Set minimum trace widths and clearances according to the chosen PCB manufacturer. Define differential or controlled impedance rules if later RF routing is added, but maintain simple rules for the current carrier PCB. Run DRC with all pours enabled and antenna keep-out enforced.

### 12. Bring-Up and Evidence-Grade Verification Hooks

Include footprints for optional 0 Ω links or jumper resistors on SHDN and PFET_GATE for manual override during bring-up. Provide test pads sized for oscilloscope probe ground clips near ADC_NODE and ACC_VOUT. Ensure mounting holes are electrically isolated or tied to chassis ground as a deliberate design choice. Document the final placement with annotated PCB screenshots for the Engineering Log.


### 13. KiCad Footprint and Symbol Library References

Define all symbols in the schematic using the official KiCad libraries where possible, and project-local libraries for custom modules.

**Power Input and Protection**
- U1 (JST-PH 2-pin): `Connector_JST:JST_PH_S2B-PH-K_1x02_P2.00mm_Horizontal` or vertical variant as required.
- U4 (2-pin bench header): `Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical`.
- D1 (1N5817 Schottky): `Diode_SMD:D_SOD-123` or `Diode_THT:D_DO-41_SOD81_P7.62mm_Horizontal` depending on assembly preference.

**Regulator Module (Pololu S7V8F3)**
- U5 footprint: `Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical`.
- Schematic symbol: custom symbol recommended, named `Pololu_S7V8F3`, with pin order SHDN, VIN, GND, VOUT.

**DWM3001-CDK Docking**
- J10 mating header: `Connector_PinHeader_2.54mm:PinHeader_2x20_P2.54mm_Vertical` (verify exact pin count and row length against CDK datasheet).
- J1 power header: `Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical`.
- Mounting holes: `MountingHole:MountingHole_2.5mm` or size matched to CDK standoff screws.

**ADXL1005 Breakout Dock**
- U2 header: `Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical`.
- Mounting holes: `MountingHole:MountingHole_2.5mm` (verify breakout board hardware).

**PFET and Passives**
- U3 (ZVP2106A): `Package_TO_SOT_SMD:SOT-23`.
- R1–R4: `Resistor_SMD:R_0603_1608Metric` or `R_0805_2012Metric`.
- C1–C3: `Capacitor_SMD:C_0603_1608Metric` or `C_0805_2012Metric`.
- Test pads: `TestPoint:TestPoint_Pad_D1.0mm`.

### 14. Explicit Coordinate-Based Placement Constraints

Define the board origin at the lower-left corner of the PCB outline. Use millimetre units for all placement constraints.

**Board Outline Reference Dimensions**
- Target board size: 70 mm (X) × 40 mm (Y) nominal, adjustable to enclosure constraints.
- Edge clearance: ≥ 1.0 mm for copper, ≥ 0.5 mm for silkscreen.

**DWM3001-CDK Dock Region**
- J10 header centreline: X = 45–55 mm, Y = 10–30 mm (align to CDK mechanical drawing).
- J1 header: positioned relative to J10 per CDK datasheet offset.
- Mounting holes: positioned per CDK standoff pattern with tolerance ±0.1 mm.
- Antenna keep-out: polygon from X = 55 mm to board edge, Y = 0–40 mm, no copper or components.

**Power Entry and Regulator Zone**
- JST-PH connector (U1): X = 0–10 mm, Y = 30–40 mm, board edge aligned.
- Bench header (U4): adjacent to U1, offset 5 mm in Y.
- D1 and U5: X = 10–25 mm, Y = 25–40 mm, with D1 directly inline between U1 and U5 VIN.
- Decoupling capacitors: within 5 mm of U5 VOUT pad.

**ADXL1005 Breakout Zone**
- Header footprint centre: X = 5–20 mm, Y = 5–20 mm.
- Mounting holes aligned to breakout geometry, tolerance ±0.1 mm.
- PFET and gate resistors: within 10 mm of ADXL1005 Vdd pin.

**Analogue Conditioning Zone**
- R1 and C1: within 5 mm of J10.15 ADC entry pin.
- ADC_NODE trace length target: < 25 mm, single-layer if possible.

### 15. PCB Layer Stack-Up Definition

Define a two-layer FR-4 stack-up suitable for low-frequency mixed-signal operation.

**Layer 1 (Top Copper)**
- Signal and power routing.
- Component placement.
- Local copper pours for 3V3+ where beneficial.

**Layer 2 (Bottom Copper)**
- Continuous ground plane with minimal segmentation.
- Ground stitching vias near all power and analogue components.

**Dielectric**
- Core material: FR-4, Tg ≥ 150 °C.
- Thickness: 1.6 mm nominal (0.8 mm optional for ultra-thin Thin-Pod variants).
- Copper weight: 1 oz/ft² (35 µm) standard; 2 oz optional for thermal robustness.

**Surface Finish**
- ENIG recommended for fine-pitch headers and long-term reliability.
- HASL acceptable for prototypes.

**Solder Mask and Silkscreen**
- Solder mask both sides, green or black.
- Silkscreen top side with Pin 1 indicators, connector labels, and antenna keep-out marking.

**Controlled Impedance**
- Not required for current low-frequency signals.
- Reserve clearance for future RF routing if the CDK antenna feed is later integrated on-board.

