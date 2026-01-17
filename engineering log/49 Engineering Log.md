# Engineering Log Summary (2026-01-17)

## Key outcomes
- A KiCad-ready 'carrier-board v0.2' set of schematic blocks was consolidated, covering RAW power entry, reverse-polarity protection, sensor power-gating, and ADC conditioning for the ADXL1005 to CDK ADC path.
- A practical drill and through-hole footprint spec was drafted for hand-assembly of the carrier, including CDK headers, the S7V8F3 regulator module, ADXL1005 board mounting, and the added diode and PFET gating parts.
- A high-level DSP learning and implementation roadmap for the Thin-Pod Gateway (STM32 NUCLEO-N657X0-Q) was produced, with a chapter-by-chapter study map and concrete lab artefacts aligned to vibration work. 
- A practical integration note was produced describing how the DSP layer feeds a TinyML layer in a compiled C vibration-analysis pipeline, including architecture boundaries, scheduling patterns, and verification strategy.
- A compact checklist was produced to audit and prove Helium (MVE) enablement in builds, including compile-time guards, disassembly checks, and baseline vs MVE benchmarking.

## Hardware and KiCad artefacts created
- **Carrier v0.2 KiCad block set (copy-paste net labels and mappings):**
  - Block A: `RAW_IN` bench or battery entry, series **1N5817** to `RAW_PROT`, feed into S7V8F3 VIN, with recommended test points.
  - Block B: **ADXL1005 VDD high-side P-MOSFET gating** (`3V3_A` to `ACC_3V3_SW`) controlled by a CDK GPIO, including pull-up and gate series resistor, plus local decoupling.
  - Block C: **ADC node conditioning** (`ACC_VOUT` to `ADC_NODE` to ADC pin), using 8.0 kΩ series, 680 pF shunt to GND, and an ADC pull-down resistor to define state during sensor deep-off.
- **Addendum for optional footprints and experiments:**
  - Optional and DNP guidance for ADC pull-down population policy and an optional sensor-output shunt capacitor footprint for controlled loading experiments.
- **THT footprint and drill spec (draft):**
  - Proposed hole sizes, pad diameters, and candidate KiCad footprints for J10, J1, ADXL1005 header pattern, S7V8F3 pin row, DO-41 diode footprint, and PFET package options (TO-92 default, TO-220 option).

## Firmware and signal-processing artefacts created
- **DSP study map for Cortex-M (Gateway focused):**
  - Chapter sequence mapped to buildable artefacts: timing harness, reference signals, FIR, FFT, STFT pipelines, sampling-rate and decimation plan, filter structures, adaptive LMS, NLMS, fixed-point decision record, and real-time buffering patterns.
  - Thin-Pod-specific additions: feature packets, diagnostics packets, acceptance plots, and budget reporting (cycles, RAM, latency, data rate).
- **DSP plus TinyML integration note (compiled C pipeline):**
  - Clear split between DSP 'signal conditioning and features' and TinyML 'decision mapping', with suggested runtime conveyor: ADC DMA buffering to DSP frontend to feature normalisation and quantisation to inference to decision smoothing to telemetry.
  - Emphasis on train-deploy numeric consistency and parity testing (offline reference vs on-device features and inference).
- **Helium (MVE) enablement checklist:**
  - Dual build profiles (baseline vs MVE), macro guards, disassembly proof of MVE instruction emission, CMSIS variant selection checks, RTOS FP and MVE context considerations, and benchmark gates.

## Immediate next steps suggested by the outputs
- Start a KiCad carrier project using the v0.2 blocks as schematic drop-ins, then instantiate the draft THT footprints and validate placement with a 1:1 print against real modules.
- Set up a Gateway firmware skeleton that includes a timing harness and a first end-to-end vibration frame path (buffering to FFT features to feature packet).
- Decide the first DSP to TinyML contract (feature vector size and normalisation), then run feature parity tests before committing to a model deployment path (CMSIS-NN, TFLM, or STM32Cube.AI).
- Add the Helium audit checks early so performance work stays measurable and regression-proof as the DSP pipeline grows.
