# Thin-Pod carrier PCB v0.1
## Schematic page draft and minimal PCB placement plan

### Scope
A minimal carrier board that:
- Accepts `RAW_IN` via a JST-PH2 battery connector.
- Regulates to `3V3_SYS` using a Pololu S7V8F3 module.
- Splits power into:
  - `3V3_CDK` feeding the DWM3001C-CDK power entry via `J1` (+ / - holes or header).
  - `3V3_A` feeding the ADXL1005 VDD.
- Routes ADXL1005 `VOUT` through a simple RC low-pass to the DWM3001C-CDK ADC input at `J10.15`.

This page is intended to be KiCad-transcribable with minimal ambiguity, while keeping flexibility around connector gender and board-to-board mating.

---

## 1) Schematic page draft

### 1.1 Net names
Power nets
- `RAW_IN`      (battery +, pre-protection)
- `RAW_IN_PROT` (post-fuse/diode, regulator VIN)
- `GND`         (0 V reference)
- `3V3_SYS`     (S7V8F3 output)
- `3V3_CDK`     (branch to CDK)
- `3V3_A`       (branch to ADXL1005)

Analogue nets
- `ACC_VOUT`    (ADXL1005 analogue output)
- `ADC_IN0`     (filtered analogue node presented to CDK ADC)

Optional control/bench nets
- `REG_SHDN`    (S7V8F3 shutdown control, optional)

---

### 1.2 Symbols and connectivity (netlist style)

#### Battery input and protection
**J_BATT_IN** (JST-PH2, 2-pin)
- Pin 1: `RAW_IN`
- Pin 2: `GND`

**F1** (optional resettable PTC, or 0 Ω link)
- `RAW_IN` → `RAW_IN_FUSED`

**D1** (reverse polarity protection)
- Preferred v0.1: Schottky diode in series
- `RAW_IN_FUSED` → `RAW_IN_PROT`

Notes:
- If minimising loss is important, replace D1 with an ideal-diode controller or a P-channel MOSFET reverse-protect stage in a later revision. v0.1 keeps D1 for simplicity.

Recommended passives:
- **CIN_BULK** 10 µF (X5R/X7R) from `RAW_IN_PROT` to `GND`
- **CIN_HF**   100 nF (X7R) from `RAW_IN_PROT` to `GND`

---

#### 3.3 V regulation
**U1** Pololu S7V8F3 module (represented as 4 pins)
- VIN:  `RAW_IN_PROT`
- GND:  `GND`
- VOUT: `3V3_SYS`
- SHDN: `REG_SHDN` (optional)

**REG_SHDN default**
- If always-on operation is desired: tie `REG_SHDN` to `RAW_IN_PROT` via a 0 Ω link.
- If on-off control is desired: route `REG_SHDN` to a 2-pin jumper to `RAW_IN_PROT`, or to a GPIO pad later.

Recommended output passives:
- **COUT_BULK** 22 µF from `3V3_SYS` to `GND`
- **COUT_HF**   100 nF from `3V3_SYS` to `GND`

---

#### Power split
**JP_CDK_PWR** (solder jumper or 0 Ω link)
- `3V3_SYS` → `3V3_CDK`

**JP_ADXL_PWR** (solder jumper or 0 Ω link)
- `3V3_SYS` → `3V3_A`

Rationale:
- Both jumpers enable safe isolation during bench work, especially when the CDK is also attached to USB power.

---

#### CDK power output
**J_CDK_J1** (2-pin header or 2 plated holes matching CDK J1)
- Pin 1: `3V3_CDK` (label as '+')
- Pin 2: `GND`     (label as '-')

Optional testpoints:
- **TP_3V3_CDK** on `3V3_CDK`
- **TP_GND** on `GND`

---

#### ADXL1005 connector
**J_ADXL** (3-pin header, minimal)
- Pin 1: `3V3_A`     (ADXL VDD)
- Pin 2: `GND`
- Pin 3: `ACC_VOUT`  (ADXL VOUT)

Optional (4th pin) if the ADXL carrier exposes self-test:
- Pin 4: `ADXL_ST` (no connection in v0.1 unless required)

Local decoupling (at J_ADXL, close to the sensor):
- **C_ADXL_BULK** 1 µF to 4.7 µF from `3V3_A` to `GND`
- **C_ADXL_HF**   100 nF from `3V3_A` to `GND`

---

#### Analogue front-end (RC low-pass)
Topology (bench-matching and corrected):
- `ACC_VOUT` → **R_SER1** → `ADC_IN0`
- `ADC_IN0` → **C_SHUNT1** → `GND`
- `ADC_IN0` → CDK ADC pin (`J10.15`)

**R_SER1** (0603)
- Populate options: 1.0 kΩ, 2.2 kΩ, 3.3 kΩ, 8.0 kΩ (bench value)
- Default suggestion for first pass: 3.3 kΩ

**C_SHUNT1** (0603)
- Populate options: 680 pF (bench value), 1 nF, 2.2 nF, 4.7 nF
- Default suggestion for first pass: 4.7 nF

Notes:
- The earlier failure mode (cap directly at ADXL VOUT) is avoided by keeping the capacitor on the ADC side of the series resistor.
- `R_SER1` also helps the ADC sample-and-hold transient and provides current limiting for any clamp structure.

Optional protection (DNP by default):
- **D_CLAMP1** dual Schottky or dual diode clamp from `ADC_IN0` to `3V3_A` and `GND`.

Optional testpoints:
- **TP_ACC_VOUT** on `ACC_VOUT`
- **TP_ADC_IN0** on `ADC_IN0`

---

#### CDK ADC input connection
Two practical footprint patterns exist. v0.1 can carry both.

**Pattern A: full J10 footprint (preferred if mating directly to the CDK)**
- **J_CDK_J10**: 2×N header footprint matching the CDK `J10`.
- Route only:
  - `J10.15` → `ADC_IN0`
  - One nearby `GND` pin → `GND` (optional but strongly recommended)

**Pattern B: minimal ‘flying lead’ footprint (preferred for breadboard-style wiring)**
- **J_ADC_OUT**: 1×2 header
  - Pin 1: `ADC_IN0`
  - Pin 2: `GND`
- A short 2-wire lead then lands on the CDK `J10.15` and a `GND` pin.

Schematic symbol recommendation:
- Keep `J_CDK_J10` as a connector symbol with explicit pin numbers.
- Keep `J_ADC_OUT` present and DNP when `J_CDK_J10` is used.

---

### 1.3 Minimum BOM (v0.1)
Power
- J_BATT_IN: JST-PH2 right-angle or vertical
- U1: Pololu S7V8F3 module
- D1: Schottky diode (SOD-123 or similar)
- CIN_BULK 10 µF, CIN_HF 100 nF
- COUT_BULK 22 µF, COUT_HF 100 nF
- JP_CDK_PWR, JP_ADXL_PWR: solder jumpers or 0 Ω links

Analogue
- J_ADXL: 1×3 header
- R_SER1 (0603)
- C_SHUNT1 (0603)
- Optional clamp diode footprint (DNP)
- J_ADC_OUT 1×2 header (optional)

CDK interface
- J_CDK_J1: 1×2 header or plated-hole pair matching CDK J1
- Optional J_CDK_J10 footprint (populate only if direct mating is planned)

Test and debug
- TP_RAW_IN, TP_RAW_IN_PROT, TP_3V3_SYS, TP_3V3_CDK, TP_3V3_A, TP_ACC_VOUT, TP_ADC_IN0, TP_GND

---

## 2) Minimal PCB placement plan (v0.1)

### 2.1 Placement intent
- Power entry and protection clustered at one PCB edge.
- Regulator module placed near the power entry, with short, wide traces for VIN, VOUT, GND.
- Analogue RC placed near the CDK ADC connection point, with a short `ADC_IN0` trace.
- Sensor connector placed away from the regulator switching node where possible.

---

### 2.2 Recommended top-side placement (not to scale)
```
+------------------------------------------------------------------+
| [J_BATT_IN]  [F1] [D1]     [U1: S7V8F3 module]                   |
|    edge       opt  opt        VIN/GND/VOUT/SHDN                  |
|                                                                  |
|             TP_RAW  TP_RAWP  TP_3V3_SYS  JP_CDK  JP_ADXL          |
|                                                                  |
| [J_CDK_J1] (+/-)                            [J_ADXL 1x3]         |
|                                                                  |
|                                  R_SER1   TP_ADC  J_ADC_OUT      |
|                                  |----|---o-----> (or J10.15)    |
|                                          |                        |
|                                        C_SHUNT1                  |
|                                          |                        |
|                                         GND                       |
|                                                                  |
|                                     [J_CDK_J10 footprint]         |
|                                     (optional, direct mate)       |
+------------------------------------------------------------------+
```

### 2.3 Footprint notes
- **JST-PH2**: place on an edge, mechanical keepout respected.
- **S7V8F3 module**: use a 1×4, 2.54 mm header footprint or a dedicated module footprint. Keep copper under the module conservative unless the Pololu layout guidance is already characterised.
- **Power traces**: use a wide pour/trace for `RAW_IN_PROT` and `3V3_SYS`.
- **Ground**: use a solid ground plane; return of `C_SHUNT1` should land into the local ground plane close to the CDK ground reference.
- **Analogue node**: keep `ADC_IN0` short and away from regulator switching loops.

### 2.4 Connector gender and mating plan
A decision is required for each CDK interface:
- If the CDK presents a **male** header at J10, place a **female** socket on the carrier.
- If the CDK presents a **female** header at J10, place a **male** header on the carrier.
- If direct mating is not required, omit J10 mating and use `J_ADC_OUT` with a short lead.

A similar decision applies for `J_CDK_J1`:
- For flying leads: a 1×2 header on the carrier is sufficient.
- For direct mating: match the CDK J1 hole spacing and use plated holes plus mechanical support.

---

## 3) Bring-up checklist for the carrier itself (v0.1)
1. Populate only: J_BATT_IN, D1, U1, CIN/COUT, TP_3V3_SYS, TP_GND.
2. Apply battery input, confirm `3V3_SYS` at TP_3V3_SYS.
3. Populate JP_ADXL_PWR and J_ADXL, confirm `3V3_A` at the sensor header with no sensor fitted.
4. Populate JP_CDK_PWR and J_CDK_J1, confirm `3V3_CDK` at the CDK feed point before connecting the CDK.
5. Populate R_SER1 and C_SHUNT1, confirm continuity:
   - `ACC_VOUT` to `ADC_IN0` only through R_SER1.
   - `ADC_IN0` to `GND` only through C_SHUNT1.
6. Connect ADXL and CDK, confirm ADXL `VOUT` sits at mid-rail and that `ADC_IN0` follows it.

---

## 4) Open decisions (parked for v0.2)
- Ideal-diode reverse protection vs Schottky loss.
- Buffering op-amp for the ADC node (if SAADC dynamic input impedance creates distortion).
- Battery sense divider and brownout threshold policy.
- Direct J10 mating strategy and mechanical standoff pattern.
