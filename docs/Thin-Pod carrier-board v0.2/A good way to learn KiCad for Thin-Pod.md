A good way to learn KiCad for Thin-Pod is to build the first PCB as a direct translation of the bench-validated wiring, then iterate. That keeps KiCad learning tightly coupled to hardware progress, and avoids the trap of trying to learn KiCad in the abstract.

For carrier-board v0.2, that first translation has a clear scope:

- Power entry and protection:
  - `JST_BATT_IN (RAW_IN) -> D1 (1N5817) -> RAW_PROT -> S7V8F3 VIN -> 3V3_SYS -> { 3V3_CDK, 3V3_A }`
  - plus an alternative bench header: `J_BENCH_IN -> RAW_IN`
- CDK interfaces:
  - power injection via CDK `J1` (`3V3_CDK` and `GND`)
  - ADC entry via `J10.15` (`ADC_IN0`)
- Sensor front-end and power control:
  - analogue chain: `ADXL1005_VOUT -> 8.0 k -> ADC_NODE -> J10.15`, with `680 pF` from `ADC_NODE` to `GND`
  - deep-off option: `3V3_A -> P-MOSFET high-side -> ACC_3V3_SW -> ADXL1005 VDD`
  - gate control: `CDK_GPIO_ACC_PWR -> R_GATE -> ACC_PFET_G`, with `100 k` pull-up to `3V3_A` for default OFF
  - STANDBY remains separate: `ACC_STANDBY` from CDK GPIO to ADXL1005 STANDBY

The KiCad-ready pin and net list for the new Schottky and P-MOSFET blocks is stored in:
- `thin-pod-carrier-board-v0.2/thin-pod_kicad_blocks_raw_schottky_pfet.md`

## A practical KiCad 9 learning path that maps to Thin-Pod

### 1) Start a KiCad project that matches the carrier-board v0.2 intention

A project such as `thin-pod-carrier-v0.2` can be created and committed early, even before a PCB exists. KiCad’s workflow is built around keeping schematic and PCB in sync via ‘Update PCB from Schematic’. ([KiCad Documentation][3])

Suggested file organisation (project-local libraries, reproducible builds):

- `thin-pod-carrier-v0.2.kicad_pro`
- `thin-pod-carrier-v0.2.kicad_sch`
- `thin-pod-carrier-v0.2.kicad_pcb`
- `thinpod.kicad_sym` (project symbol lib)
- `thinpod.pretty/` (project footprint lib)

KiCad’s ‘Getting Started’ guide encourages project-specific libraries for new symbols and footprints, which fits a carrier board with a few custom modules and board-to-board connectors. ([KiCad Documentation][4])

### 2) Capture the schematic as three small sheets, plus a tiny ‘Power Control’ sub-block

This is the fastest route to a clean schematic and an easy PCB:

- Sheet: Power
  - `JST_BATT_IN` on `RAW_IN`
  - `J_BENCH_IN` on `RAW_IN`
  - D1 series Schottky: `RAW_IN -> RAW_PROT`
  - S7V8F3 module: `RAW_PROT -> 3V3_SYS`
  - rail split: `3V3_SYS -> 3V3_CDK` and `3V3_SYS -> 3V3_A`
  - optional analogue isolation element at the branch to `3V3_A`
  - `VBATT_SENSE` divider and sense capacitor (tap point defined explicitly as either `RAW_IN` or `RAW_PROT`)

- Sheet: Sensor front-end
  - ADXL1005 breakout interface: `ACC_VOUT`, `ACC_STANDBY`, `ACC_3V3_SW`, `GND`
  - RC front-end values: `8.0 k`, `680 pF` with the shunt cap at `ADC_NODE`
  - local decoupling on `ACC_3V3_SW` near the sensor (100 nF plus bulk)

- Sheet: CDK interface
  - CDK `J1` mapping to `3V3_CDK` and `GND`
  - CDK `J10.15` mapping to `ADC_IN0`
  - CDK GPIO mapping to `CDK_GPIO_ACC_PWR` (PFET gate control)
  - CDK GPIO mapping to `ACC_STANDBY` (sensor standby)

This structure mirrors the carrier notes, keeps ERC debugging local, and makes changes easy to review.

### 3) Treat the CDK and breakouts as connectors first, not as full devices

For the first PCB spin, the simplest modelling is usually best:

- Represent DWM3001C-CDK attachment points as connectors:
  - `J1` as a 1x2 2.54 mm header footprint (carrier side)
  - `J10` as the relevant header footprint (pin 15 used for `ADC_IN0`, plus any other required pins)

- Represent the ADXL1005 breakout as a connector footprint with the pins actually used:
  - `VDD` (to `ACC_3V3_SW`)
  - `GND`
  - `VOUT` (to `ACC_VOUT`)
  - `STANDBY` (to `ACC_STANDBY`)

This keeps the first layout about placement, grounding, and routing, not library perfection.

### 4) Make a short list of custom footprints, then build them once

Likely custom items for this carrier:

- Pololu S7V8F3 module footprint (hole pattern and outline)
- CDK mechanical mating headers, if exact geometry is required
- mounting holes, if the carrier is structural

KiCad 9’s Footprint Editor is integrated with PCBNew, so these can live in `thinpod.pretty` and evolve with the design. ([KiCad Documentation][5])

### 5) Set net classes early so routing feels rule-driven

In Board Setup:

- power net class for `RAW_IN`, `RAW_PROT`, `3V3_*`, `ACC_3V3_SW` with wider tracks
- ‘quiet analogue’ net class for `ACC_VOUT`, `ADC_NODE`, `ADC_IN0` with conservative clearances and short-route intent
- `GND` as a filled zone early, then route signals against a stable return plane

KiCad’s PCB Editor DRC supports this workflow, and it is worth leaning on DRC continuously during placement and routing. ([KiCad Documentation][5])

### 6) Place with the electrical story in mind

A placement pattern that maps directly onto v0.2:

- Cluster `JST_BATT_IN`, `J_BENCH_IN`, D1 (Schottky), and S7V8F3 tightly to keep the raw path short and obvious
- Keep `RAW_IN` and `RAW_PROT` test points accessible for bring-up (`TP_RAW_IN`, `TP_RAW_PROT`)
- Place the `3V3_SYS -> 3V3_A` branch element at the split point, not near the sensor
- Place Q1 (PFET), R1/R2, and `ACC_3V3_SW` decoupling close to the ADXL1005 VDD pin
- Place the ADC RC parts close to the CDK ADC entry so `ADC_NODE` is physically short
- Keep switching-regulator ‘noisy’ loops away from `ADC_NODE` and `ACC_VOUT`

### 7) Use KiCad 9 jobsets for repeatable fabrication outputs

KiCad 9 jobsets can run ERC, DRC, and generate fabrication outputs in a repeatable sequence. This suits a disciplined Thin-Pod release flow with versioned outputs. ([KiCad Documentation][6])

## Thin-Pod specific first spin checklist (carrier-board v0.2)

- Nets named exactly as defined in the carrier notes and the v0.2 blocks:
  - `RAW_IN`, `RAW_PROT`, `3V3_SYS`, `3V3_CDK`, `3V3_A`, `ACC_3V3_SW`
  - `ADC_IN0`, `ACC_VOUT`, `ADC_NODE`
  - `ACC_PFET_G`, `CDK_GPIO_ACC_PWR`, `ACC_STANDBY`
- D1 orientation and labelling match the intended protection: `RAW_IN -> RAW_PROT`
- PFET gate default state is OFF via 100 k pull-up to `3V3_A`
- Test points placed for: `RAW_IN`, `RAW_PROT`, `ACC_3V3_SW`, and optionally `ACC_PFET_G`
- RC front-end matches bench topology, with the shunt cap at `ADC_NODE`
- A schematic note exists about backfeeding risk when powering the CDK externally (USB and any ‘off’ sensor rail edge cases)

## High-leverage learning resources aligned to this workflow

- ‘Getting Started in KiCad 9.0’ (project setup, project libraries, end-to-end flow). ([KiCad Documentation][4])
- ‘Schematic Editor 9.0’ section on ‘Update PCB from Schematic’ (the core loop). ([KiCad Documentation][3])
- ‘PCB Editor 9.0’ section on DRC and rule-driven layout. ([KiCad Documentation][5])
- KiCad’s curated learning resources page for longer tutorials. ([kicad.org][7])

A fast tangible milestone is: a clean KiCad schematic for carrier-board v0.2 with footprints assigned for everything except any CDK mechanical mating uncertainty, followed by a first board outline and rough placement. That yields a first PCB candidate with minimal friction, while staying faithful to bench-proven behaviour.

[1]: https://chatgpt.com/c/6949b504-e1bc-8332-9c86-5ed4a680e2f6 'Bridging the 10% gap'
[2]: https://chatgpt.com/c/695ee160-8754-8327-949b-001225e4357a 'C Compiler Reasons'
[3]: https://docs.kicad.org/9.0/en/eeschema/eeschema.html?utm_source=chatgpt.com 'Schematic Editor | 9.0 | English | Documentation - KiCad Docs'
[4]: https://docs.kicad.org/9.0/en/getting_started_in_kicad/getting_started_in_kicad.html?utm_source=chatgpt.com 'Getting Started in KiCad | 9.0 | English | Documentation'
[5]: https://docs.kicad.org/9.0/en/pcbnew/pcbnew.html?utm_source=chatgpt.com 'PCB Editor | 9.0 | English | Documentation - KiCad Docs'
[6]: https://docs.kicad.org/9.0/en/introduction/introduction.html?utm_source=chatgpt.com 'Introduction | 9.0 | English | Documentation - KiCad Docs'
[7]: https://www.kicad.org/help/learning-resources/?utm_source=chatgpt.com 'Learning Resources - KiCad'
