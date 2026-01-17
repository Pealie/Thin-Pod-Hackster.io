# Thin-Pod Gateway DSP study map (Arm Cortex-M)

Date: 2026-01-17  
Target platform: STM32 NUCLEO-N657X0-Q (Thin-Pod Gateway)  
Primary reference: *Digital Signal Processing using Arm Cortex-M based Microcontrollers: Theory and Practice* (Arm University Program, 2018)

Tags used in this checklist: `[buffering]` `[filters]` `[fixed-point]` `[adaptive]` `[real-time]` `[verification]` `[features]` `[telemetry]` `[budgets]` `[arch]`

## Translation note (board and toolchain)
The reference labs target STM32F4 Discovery and Keil µVision. The study sequence remains valid. Project creation, peripheral setup, and build system details require translation to the Gateway toolchain (typically STM32CubeIDE plus ST-LINK). CMSIS and CMSIS-DSP concepts remain applicable.

---

## Preparation and instrumentation

- [ ] **Appendix A + Lab 0 (p. 300): 'Getting Started with KEIL and CMSIS'**
  - Output artefacts:
    - [ ] Minimal 'blinky + printf' project template for the Gateway
    - [ ] DWT cycle counter helper (StartTiming, StopTiming, CalculateTime style)
    - [ ] RAM and ROM reporting method (map file or build output)
  - Thin-Pod relevance: baseline timing and memory discipline for all DSP kernels
  - Tags: `[verification]` `[budgets]`

---

## Chapter sequence and matching lab artefacts

- [ ] **Chapter 1 (p. 3): 'Digital Signal Processing Basics'**
  - [ ] **Lab 1 (p. 8): MCU environment and measurement basics**
  - Output artefacts:
    - [ ] Gateway DSP project skeleton (HAL init, clocks, IRQ layout)
    - [ ] 'timing_dwt' helper integrated into the build
  - Tags: `[real-time]` `[verification]`

- [ ] **Chapter 2 (p. 31): 'Discrete-Time Signal Processing Basics'**
  - [ ] **Lab 2 (p. 57): digital signals, operations, periodicity, implementing digital systems, LTI implementation**
  - Output artefacts:
    - [ ] Reference signal generators (sine, chirp, impulse, step)
    - [ ] Convolution and FIR reference implementation with test vectors
  - Tags: `[filters]` `[verification]`

- [ ] **Chapter 3 (p. 75): 'The Z-Transform'**
  - No lab in the reference.
  - Output artefacts:
    - [ ] Difference equation template for IIR sections (state, coefficients, update)
    - [ ] Notes: stability and pole placement constraints for embedded
  - Tags: `[filters]`

- [ ] **Chapter 4 (p. 93): 'Frequency Analysis of Discrete-Time Systems'**
  - [ ] **Lab 4 (p. 112): DFT, FFT, DTFS, STFT, filtering in the frequency domain, linear phase**
  - Output artefacts:
    - [ ] FFT-based spectral analysis of a captured vibration record
    - [ ] STFT spectrogram pipeline (buffer, windowing, magnitude, export)
  - Tags: `[filters]` `[features]`

- [ ] **Chapter 5 (p. 129): 'Conversion between Continuous-Time and Discrete-Time Signals'**
  - [ ] **Lab 5 (p. 148): ADC, DAC, aliasing, reconstruction, downsampling, interpolation**
  - Output artefacts:
    - [ ] Sampling-rate plan (target fs, anti-alias assumptions, decimation plan)
    - [ ] Sample-rate conversion utility (downsample/interpolate) for recorded data
  - Tags: `[real-time]` `[budgets]`

- [ ] **Chapter 6 (p. 163): 'Digital Processing of Continuous-Time Signals'**
  - [ ] **Lab 6 (p. 172): frequency mapping, continuous-time systems, digital representation of a continuous-time filter**
  - Output artefacts:
    - [ ] Notes: analogue front-end assumptions and corresponding digital model
    - [ ] Digital equivalent of a simple CT filter (proof via frequency response)
  - Tags: `[filters]`

- [ ] **Chapter 7 (p. 183): 'Structures for Discrete-Time LTI Systems'**
  - No lab in the reference.
  - Output artefacts:
    - [ ] Structure selection note for the Gateway (FIR, biquad cascade, lattice)
    - [ ] CMSIS-DSP structure mapping table for chosen filters
  - Tags: `[filters]` `[arch]`

- [ ] **Chapter 8 (p. 197): 'Digital Filter Design'**
  - [ ] **Lab 8 (p. 213): CMSIS filter structures, implementing a filter using different structures, three-band equaliser design**
  - Output artefacts:
    - [ ] Three-band filter bank design relevant to rotating machinery bands
    - [ ] Benchmark: same response implemented via at least two structures
  - Tags: `[filters]` `[features]` `[budgets]`

- [ ] **Chapter 9 (p. 225): 'Adaptive Signal Processing'**
  - [ ] **Lab 9 (p. 243): CMSIS LMS and NLMS, adaptive applications, performance analysis**
  - Output artefacts:
    - [ ] LMS or NLMS demo using recorded vibration data as 'input' and a target
    - [ ] Tracking and convergence metrics logged (error, step size, stability)
  - Tags: `[adaptive]` `[verification]`

- [ ] **Chapter 10 (p. 251): 'Fixed-Point Implementation'**
  - [ ] **Lab 10 (p. 261): fixed/float conversions; fixed-point convolution, FIR; fixed-point FFT; downsampling/interpolation; fixed-point LTI structures; fixed-point adaptive; fixed-point equaliser**
  - Output artefacts:
    - [ ] Fixed-point format decision record (q15 vs q31) for the Gateway DSP path
    - [ ] Fixed-point FIR and FFT benchmarks with error and overflow checks
  - Tags: `[fixed-point]` `[filters]` `[adaptive]` `[verification]`

- [ ] **Chapter 11 (p. 283): 'Real-Time Digital Signal Processing'**
  - [ ] **Lab 11 (p. 292): real-time setup; audio effects; triple and ping-pong buffers; overlap-add and overlap-save; equaliser in real time**
  - Output artefacts:
    - [ ] Buffering reference implementations: linear, circular, triple buffer, ping-pong
    - [ ] Frame-based FIR using overlap-add and overlap-save
    - [ ] Memory and latency comparison between triple buffer and ping-pong
  - Tags: `[buffering]` `[real-time]` `[filters]` `[budgets]` `[arch]`

---

## Thin-Pod additions: vibration feature pack and acceptance plots

Purpose: convert 'DSP study' into 'predictive maintenance capability'. Each item below should include an offline reference calculation (host-side) and an on-device implementation (Gateway), with outputs compared.

### Data products
- [ ] **Raw capture packet**: short bursts of raw ADC samples with metadata (fs, gain, timestamp, units)
- [ ] **Feature packet**: compact numerical features per window/frame, with schema version
- [ ] **Diagnostics packet**: internal health (saturation counts, dropped buffers, CPU margin)

Tags: `[telemetry]` `[verification]`

### Vibration feature pack (minimum viable)
Time-domain (per window):
- [ ] RMS
- [ ] Peak and peak-to-peak
- [ ] Crest factor
- [ ] Kurtosis
- [ ] Skewness
- [ ] Zero-crossing rate (optional)

Frequency-domain (per window, windowed FFT):
- [ ] Dominant peak frequency and amplitude
- [ ] Band powers for chosen machine-relevant bands
- [ ] Spectral centroid and spectral flatness (optional)

Envelope features (bearing-fault oriented):
- [ ] Simple envelope via rectification or Hilbert magnitude (choose one)
- [ ] Envelope spectrum peak(s) and band power

Tags: `[features]` `[filters]`

### Rotating machinery relevance mapping (to record in the log)
- [ ] Chosen sampling rate and why (Nyquist versus expected fault bands)
- [ ] Window length and overlap and why (frequency resolution versus latency)
- [ ] Band definitions and justification (link to machine or gearbox speeds if available)

Tags: `[budgets]` `[features]`

### Acceptance plots to commit for each milestone
For each new feature/kernel, commit at least the following plots (host-generated is acceptable; on-device values must be exported):

- [ ] **Time trace**: raw samples and pre-processed samples (DC removed, scaled)
- [ ] **FFT magnitude**: with identified peaks labelled
- [ ] **Band-power bar plot**: the defined bands for one representative capture
- [ ] **Feature trend plot**: feature values over successive windows (a short run)
- [ ] **Error plot**: on-device versus offline reference (absolute and relative)
- [ ] **CPU and RAM report**: cycles per window, worst-case margin, buffer RAM use

Tags: `[verification]` `[budgets]`

---

## Thin-Pod additions: architecture templates (reusable patterns)

- [ ] **Acquisition template**: Timer-triggered ADC + DMA circular buffer + half/full callbacks
- [ ] **Processing template**: Ping-pong frame buffer + deterministic processing function
- [ ] **Transport template**: lock-free queue from processing to comms/logging
- [ ] **Benchmark harness**: one function pointer per kernel, timed with DWT, reporting cycles

Tags: `[arch]` `[real-time]` `[buffering]`

---

## Thin-Pod additions: budgeting and decision records (ADR style)

### Budget sheet (one page, kept current)
- [ ] Target sample rate (fs)
- [ ] Frame size (N) and overlap (%)
- [ ] Frames per second (effective update rate)
- [ ] Cycles per frame (measured) and CPU margin (%)
- [ ] RAM: DMA buffers + DSP scratch + feature packet buffers
- [ ] Data rate: raw and feature packets (bytes/s)

Tags: `[budgets]`

### Decision records to add when choices are made
- [ ] Filter structure choice (FIR vs IIR, cascade vs lattice) and why
- [ ] Numeric choice (float32 vs q15 vs q31) and why
- [ ] Overflow and scaling policy for fixed-point stages
- [ ] Telemetry schema versioning and units policy

Tags: `[verification]` `[fixed-point]` `[telemetry]`

---

## Exit criteria: DSP study map complete for Thin-Pod Gateway

- [ ] End-to-end pipeline runs: ADC capture → buffering → DSP kernel(s) → feature packet output
- [ ] CPU margin documented at target fs, worst-case
- [ ] Latency documented (pipeline + per-window)
- [ ] At least one filter bank and one FFT feature path validated against offline reference
- [ ] At least one adaptive method demonstrated on recorded vibration data
- [ ] One field-like dataset captured, replayed, and analysed reproducibly (vectors committed)

