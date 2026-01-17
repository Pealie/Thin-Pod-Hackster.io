# Thin-Pod: integrating DSP with TinyML in a compiled C vibration-analysis program

Date: 2026-01-17  
Target: Thin-Pod Gateway (STM32 NUCLEO-N657X0-Q)

## Where DSP ends and TinyML begins

For vibration analysis on an MCU, DSP and TinyML are best treated as two layers of the same pipeline:

1. **DSP layer**: makes the signal well-behaved and information-dense  
   Examples: anti-alias and decimation policy, DC removal, windowing, FFT, band powers, envelope spectrum, feature scaling.

2. **TinyML layer**: maps features to a decision  
   Examples: 'healthy vs fault class', 'fault severity score', 'anomaly score', 'which band looks wrong'.

This split keeps the learning problem stable and keeps inference cheap, while still allowing a move to more end-to-end models later.

On the Gateway’s STM32N657 (STM32N6 family), the Cortex-M55 with Helium MVE is explicitly positioned as bringing DSP capability into the CPU, and the N6x7 parts include an embedded NPU.

## Integration architecture for a compiled C program

### Dataflow (runtime)

A practical arrangement is a three-stage conveyor:

**A) Acquisition**
- Timer-triggered ADC sampling
- DMA into a circular buffer (half/full transfer callbacks)

**B) DSP stage**
- Frame assembly (ping-pong buffers)
- Pre-processing: DC removal, scale-to-units, optional band-limit
- Feature extraction: FFT and derived features (band energies, peaks, envelope spectrum features)
- Normalisation: fixed parameters baked into the build (mean, std, min/max, or quantisation scale)

**C) TinyML stage**
- Tensor fill from features
- Inference on CPU via CMSIS-NN or TFLM, or via ST tooling that targets the NPU where supported
- Post-processing: smoothing, hysteresis, 'event' detection and confidence gating
- Output: telemetry packet of features + decision + diagnostics

This architecture matches the buffering and real-time work already in the DSP study map, while keeping TinyML as a clean consumer of a feature vector.

### Controlflow (scheduling)

Two common patterns work well:

- **Always-on light DSP, conditional ML**  
  Light features run every frame; ML runs only when a gate trips (RMS jump, band power threshold, kurtosis spike). This saves power and cycles.

- **Always-on ML on a reduced cadence**  
  DSP runs at full rate; inference runs every Nth frame or on an aggregated window to reduce jitter sensitivity.

## Choosing the DSP-to-ML interface

### Option 1: 'Feature vector' classifier

The DSP stage emits roughly 10 to 50 numbers per frame, then a small model consumes them.

Typical feature groups:
- Time domain: RMS, crest factor, kurtosis
- Frequency domain: band powers, dominant peaks
- Envelope domain: envelope spectrum peaks or band powers

Model types that stay small:
- Logistic regression or small MLP
- 1D CNN over band-power sequences (if temporal context matters)

This is usually the fastest route to a robust first system, with strong interpretability.

### Option 2: 'Spectrogram patch' model

DSP emits a log-magnitude STFT patch (time-frequency tile), then a small 2D CNN consumes it.

Pros: fault signatures often live in patterns across time-frequency.  
Cons: larger buffers, more compute, more careful quantisation needed.

### Option 3: 'Raw waveform' model

DSP does minimal conditioning, then a 1D CNN consumes raw samples.

Pros: fewer handcrafted choices.  
Cons: usually the hardest to make data-efficient and compute-efficient on MCU, especially if sample rates are high.

## Tooling choices on STM32N657 for compiled C inference

### ST workflow: STM32Cube.AI and X-CUBE-AI

STM32Cube.AI is positioned as an optimiser and deploy tool that can export STM32-optimised inference libraries from common training frameworks, available via CubeMX and CLI.

X-CUBE-AI is described as compiling and generating optimised C code, and for Neural-ART NPU targets it can generate microcode and schedule operators between NPU and CPU as needed.

For STM32N657 specifically, the product documentation notes the Cortex-M55 core with Helium MVE and indicates the presence of an NPU on STM32N6x7 devices.

### Generic workflow: TFLM + CMSIS-NN

A second path is TensorFlow Lite for Microcontrollers plus CMSIS-NN kernels, with full control over memory arenas and quantisation. This can be attractive when the project wants minimal vendor coupling, or when the model is small and performance requirements are already met on CPU.

## The critical integration detail: numeric consistency

Most TinyML deployments succeed or fail on train-deploy skew.

A robust contract between DSP and ML needs:
- Exactly the same windowing, FFT scaling, and band definitions offline and on device
- Exactly the same feature normalisation and any clipping policy
- Deterministic quantisation if the model is int8

For an int8 quantised model, the feature-to-tensor mapping is usually:

- `q = round(x / scale) + zero_point`, then clamped to `[-128, 127]`

The 'scale' and 'zero_point' must be treated as model parameters and versioned alongside the weights.

## How this looks in a Thin-Pod codebase

A clean compiled-C structure tends to converge on modules like:

- `acq_adc_dma.c`: acquisition and buffers
- `dsp_frontend.c`: windowing, FFT, feature extraction (CMSIS-DSP)
- `feature_norm.c`: normalisation, quantisation helpers
- `ml_infer.c`: model entrypoint, invokes generated code or TFLM
- `decision.c`: smoothing, thresholds, event logic
- `telemetry.c`: packet schema and versioning

A simple interface boundary helps:

- DSP exports: `features[N]` plus diagnostics (saturation count, dropped frames, cycle count)
- ML consumes: `tensor_in[]` and emits `scores[]` or `class_id + confidence`

## Verification loop for 'DSP + TinyML' on vibration

A high-value workflow for Thin-Pod is:

1. **Offline reference pipeline**  
   A host script runs the exact same DSP steps and produces features.

2. **On-device feature parity test**  
   The same input window is injected, and feature deltas are computed. Tight tolerances for fixed-point and quantised paths.

3. **Inference parity test**  
   The same features are passed into the offline model and the on-device model. Output deltas are compared.

4. **Budget regression**  
   Cycles per frame, RAM, and latency are logged as part of the engineering log.

This makes it possible to change filters or feature sets without silently invalidating the model.

## A Thin-Pod-specific first sensible milestone

For rotating machinery, a pragmatic first integrated system is:

- DSP: windowed FFT + band powers + RMS + kurtosis + simple envelope feature
- TinyML: small MLP or logistic regression classifier on a 20 to 40 element feature vector
- Output: 'normal' vs 'anomaly' plus a few top contributing bands for interpretability

It tends to deliver early value, collects labelled data for later richer models, and fits comfortably inside MCU real-time budgets.
