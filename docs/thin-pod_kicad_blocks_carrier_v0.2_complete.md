# Thin-Pod KiCad-ready schematic blocks (carrier-board v0.2)
RAW input Schottky protection, optional bench input header, ADXL1005 high-side PFET gating, and ADC node conditioning.

This file supersedes earlier split notes by consolidating:
- Block A: RAW input entry with optional bench header and series 1N5817.
- Block B: ADXL1005 VDD high-side P-MOSFET gating (deep-off capable).
- Block C: ADC node conditioning, including an ADC node pull-down resistor (populated).

Net names and connector pin mappings are intended to be pasted directly into KiCad as net labels and connector pin nets.

---

## Block A: Bench header + series 1N5817 on RAW input

### Nets
- `RAW_IN` (raw source side, battery or bench)
- `RAW_PROT` (post-diode, feeds regulator VIN)
- `GND`

### Connectors and pins

**JST_BATT_IN (JST-PH2)**
- Pin 1: `RAW_IN`
- Pin 2: `GND`

**J_BENCH_IN (2-pin 2.54 mm header, alternative supply)**
- Pin 1: `RAW_IN`
- Pin 2: `GND`

### Protection component

**D1 (1N5817, series Schottky)**
- Anode: `RAW_IN`
- Cathode: `RAW_PROT`

### Regulator interface

**U1 (Pololu S7V8F3)**
- VIN: `RAW_PROT`
- GND: `GND`
- VOUT: `3V3_SYS`

### Recommended test points
- `TP_RAW_IN` on `RAW_IN`
- `TP_RAW_PROT` on `RAW_PROT`

---

## Block B: ADXL1005 VDD high-side P-MOSFET gating

### Nets
- `3V3_A` (analogue branch source rail)
- `ACC_3V3_SW` (switched sensor rail, to ADXL1005 VDD)
- `ACC_PFET_G` (PFET gate node)
- `CDK_GPIO_ACC_PWR` (CDK GPIO net controlling the PFET gate)
- `GND`

### Switch components and connections

**Q1 (P-MOSFET, high-side)**
- Source (S): `3V3_A`
- Drain (D): `ACC_3V3_SW`
- Gate (G): `ACC_PFET_G`

**R1 (gate pull-up, default OFF)**
- 100 kΩ between `ACC_PFET_G` and `3V3_A`

**R2 (gate series resistor)**
- 100 Ω to 1 kΩ between `CDK_GPIO_ACC_PWR` and `ACC_PFET_G`

**C1 (local decoupling, close to ADXL VDD)**
- 100 nF between `ACC_3V3_SW` and `GND`

**C2 (local bulk, close to ADXL VDD)**
- 1 µF to 10 µF between `ACC_3V3_SW` and `GND`

### Sensor connector pin mapping

**J_ADXL1005 (breakout header or pads)**
- VDD: `ACC_3V3_SW`
- GND: `GND`
- VOUT: `ACC_VOUT`
- STANDBY: `ACC_STANDBY` (separate from PFET gating)

### Logic note
- `CDK_GPIO_ACC_PWR = HIGH` (≈3V3) -> `ACC_PFET_G` pulled up to `3V3_A` -> Q1 OFF -> `ACC_3V3_SW` unpowered.
- `CDK_GPIO_ACC_PWR = LOW` (0V) -> Vgs negative -> Q1 ON -> `ACC_3V3_SW` powered.

### Recommended test points
- `TP_ACC_3V3_SW` on `ACC_3V3_SW`
- `TP_ACC_PFET_G` on `ACC_PFET_G`

---

## Block C: ADC node conditioning (includes populated ADC node pull-down)

### Purpose
- Preserve the bench-validated anti-alias topology at the ADC input node.
- Ensure a defined ADC node state when the sensor rail is switched off via PFET gating.

### Nets
- `ACC_VOUT` (sensor output)
- `ADC_NODE` (post-series resistor, pre-ADC pin)
- `ADC_IN0` (CDK ADC pin net)
- `GND`

### Components and connections

**R_SER1 (series resistor)**
- Value: 8.0 kΩ
- Pin 1: `ACC_VOUT`
- Pin 2: `ADC_NODE`

**C_SHUNT1 (ADC node shunt capacitor)**
- Value: 680 pF
- Pin 1: `ADC_NODE`
- Pin 2: `GND`

**R_PD1 (ADC node pull-down resistor, populated)**
- Value: 1 MΩ (acceptable range: 470 kΩ to 2.2 MΩ)
- Pin 1: `ADC_NODE`
- Pin 2: `GND`

**ADC connection**
- `ADC_NODE` connects to `ADC_IN0` (CDK J10.15)

### Optional experiment footprint (DNP)

**C_PAR1 (sensor output shunt capacitor, DNP)**
- Value: DNP (test range: 100 pF to 1 nF)
- Pin 1: `ACC_VOUT`
- Pin 2: `GND`

### Placement guidance
- Place `R_SER1`, `C_SHUNT1`, and `R_PD1` close to the CDK ADC entry so `ADC_NODE` is physically short.
- Place `C_PAR1` close to the ADXL1005 breakout VOUT pin if fitted during output loading experiments.

---

## Copy-paste net labels
- `RAW_IN`
- `RAW_PROT`
- `GND`
- `3V3_SYS`
- `3V3_A`
- `ACC_3V3_SW`
- `ACC_PFET_G`
- `CDK_GPIO_ACC_PWR`
- `ACC_VOUT`
- `ACC_STANDBY`
- `ADC_NODE`
- `ADC_IN0`

## Reference filenames
- Earlier split block note: `thin-pod_kicad_blocks_raw_schottky_pfet.md`
- Earlier addendum note: `thin-pod_kicad_blocks_addendum_adc_pulldown_optional_vout_cap.md`
