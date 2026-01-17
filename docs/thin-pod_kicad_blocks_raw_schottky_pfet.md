# Thin-Pod KiCad-ready schematic blocks: RAW input Schottky + ADXL1005 high-side gating

## Block A: Bench header + series IN5817 on RAW input

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
- VOUT: `3V3_SYS` (existing net)

### Recommended test points
- `TP_RAW_IN` on `RAW_IN`
- `TP_RAW_PROT` on `RAW_PROT`


## Block B: ADXL1005 VDD high-side P-MOSFET gating

### Nets
- `3V3_A` (existing analogue branch source rail)
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
- VOUT: `ACC_VOUT` (existing signal chain continues via `R_SER1` to `ADC_NODE`)
- STANDBY: `ACC_STANDBY` (existing control net, separate from PFET gating)

### Logic note
- `CDK_GPIO_ACC_PWR = HIGH` (≈3V3) -> `ACC_PFET_G` pulled up to `3V3_A` -> Q1 OFF -> `ACC_3V3_SW` unpowered.
- `CDK_GPIO_ACC_PWR = LOW` (0V) -> Vgs negative -> Q1 ON -> `ACC_3V3_SW` powered.

### Recommended test points
- `TP_ACC_3V3_SW` on `ACC_3V3_SW`
- `TP_ACC_PFET_G` on `ACC_PFET_G`


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
