# Thin-Pod KiCad-ready addendum: ADC node pull-down and optional VOUT capacitor footprints

This addendum extends the existing v0.2 KiCad-ready blocks by defining an optional pull-down on the ADC node
and an optional experimental capacitor footprint at the sensor output. These are intended to support
deterministic behaviour when the sensor rail is power-gated and to allow controlled experiments with output
loading.

File to accompany:
- `thin-pod_kicad_blocks_raw_schottky_pfet.md`

---

## Block C: Optional ADC node pull-down resistor (recommended with PFET power-gating)

### Purpose
- Provide a defined DC state for the ADC input node when the ADXL1005 rail is switched off (or the output is floating).
- Reduce floating/noise pickup at `ADC_NODE` during deep-off and reset transitions.

### Nets
- `ADC_NODE`
- `GND`

### Component and connections
**R_PD1 (ADC node pull-down, DNP option)**
- Value: 1 MΩ (acceptable range: 470 kΩ to 2.2 MΩ)
- Pin 1: `ADC_NODE`
- Pin 2: `GND`

### Placement guidance
- Place physically close to the ADC input conditioning network (near `R_SER1` and `C_SHUNT1`) and close to the CDK ADC entry.
- Default population state can be DNP; populate if floating-node behaviour is observed during testing.

---

## Block D: Optional experimental capacitor at sensor output (use with caution)

### Purpose
- Provide a controlled, documented footprint to experiment with additional output loading at `ACC_VOUT`.
- Default state is DNP; only populate during deliberate drive/loading characterisation.

### Nets
- `ACC_VOUT`
- `GND`

### Component and connections
**C_PAR1 (sensor output shunt capacitor, DNP option)**
- Value: DNP (test range: 100 pF to 1 nF)
- Pin 1: `ACC_VOUT`
- Pin 2: `GND`

### Placement guidance
- Place near the ADXL1005 breakout `VOUT` pin if the intention is to load the sensor output directly.
- Do not populate by default; the primary anti-alias capacitor remains `C_SHUNT1 = 680 pF` at `ADC_NODE` to avoid excessive sensor output loading.

---

## Copy-paste net labels (addendum)
- `R_PD1`
- `C_PAR1`
- `ADC_NODE`
- `ACC_VOUT`
- `GND`
