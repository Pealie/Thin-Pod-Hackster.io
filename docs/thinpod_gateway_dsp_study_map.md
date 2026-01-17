# Thin-Pod Gateway DSP study map (Arm Cortex-M)

Date: 2026-01-17  
Target platform: STM32 NUCLEO-N657X0-Q (Thin-Pod Gateway)  
Primary reference: *Digital Signal Processing using Arm Cortex-M based Microcontrollers: Theory and Practice* (Arm University Program, 2018)

Tags used in this checklist: `[buffering]` `[filters]` `[fixed-point]` `[adaptive]` `[real-time]`

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

---

## Chapter sequence and matching lab artefacts

- [ ] **Chapter 1 (p. 3): 'Digital Signal Processing Basics'**
  - [ ] **Lab 1 (p. 8): MCU environment and measurement basics**
  - Output artefacts:
    - [ ] Gateway DSP project skeleton (HAL init, clocks, IRQ layout)
    - [ ] 'timing_dwt' helper integrated into the build
  - Tags: `[real-time]` (prerequisite)

- [ ] **Chapter 2 (p. 31): 'Discrete-Time Signal Processing Basics'**
  - [ ] **Lab 2 (p. 57): digital signals, operations, periodicity, implementing digital systems, LTI implementation**
  - Output artefacts:
    - [ ] Reference signal generators (sine, chirp, impulse, step)
    - [ ] Convolution and FIR reference implementation with test vectors
  - Tags: `[filters]` (foundation)

- [ ] **Chapter 3 (p. 75): 'The Z-Transform'**
  - No lab in the reference.
  - Output artefacts:
    - [ ] Difference equation template for IIR sections (state, coefficients, update)
    - [ ] Notes: stability and pole placement constraints for embedded
  - Tags: `[filters]` (foundation)

- [ ] **Chapter 4 (p. 93): 'Frequency Analysis of Discrete-Time Systems'**
  - [ ] **Lab 4 (p. 112): DFT, FFT, DTFS, STFT, filtering in the frequency domain, linear phase**
  - Output artefacts:
    - [ ] FFT-based spectral analysis of a captured vibration record
    - [ ] STFT spectrogram pipeline (buffer, windowing, magnitude, export)
  - Tags: `[filters]`

- [ ] **Chapter 5 (p. 129): 'Conversion between Continuous-Time and Discrete-Time Signals'**
  - [ ] **Lab 5 (p. 148): ADC, DAC, aliasing, reconstruction, downsampling, interpolation**
  - Output artefacts:
    - [ ] Sampling-rate plan (target fs, anti-alias assumptions, decimation plan)
    - [ ] Sample-rate conversion utility (downsample/interpolate) for recorded data
  - Tags: `[real-time]` (prerequisite)

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
  - Tags: `[filters]`

- [ ] **Chapter 8 (p. 197): 'Digital Filter Design'**
  - [ ] **Lab 8 (p. 213): CMSIS filter structures, implementing a filter using different structures, three-band equaliser design**
  - Output artefacts:
    - [ ] Three-band filter bank design relevant to rotating machinery bands
    - [ ] Benchmark: same response implemented via at least two structures
  - Tags: `[filters]`

- [ ] **Chapter 9 (p. 225): 'Adaptive Signal Processing'**
  - [ ] **Lab 9 (p. 243): CMSIS LMS and NLMS, adaptive applications, performance analysis**
  - Output artefacts:
    - [ ] LMS or NLMS demo using recorded vibration data as 'input' and a target
    - [ ] Tracking and convergence metrics logged (error, step size, stability)
  - Tags: `[adaptive]`

- [ ] **Chapter 10 (p. 251): 'Fixed-Point Implementation'**
  - [ ] **Lab 10 (p. 261): fixed/float conversions; fixed-point convolution, FIR; fixed-point FFT; downsampling/interpolation; fixed-point LTI structures; fixed-point adaptive; fixed-point equaliser**
  - Output artefacts:
    - [ ] Fixed-point format decision record (q15 vs q31) for the Gateway DSP path
    - [ ] Fixed-point FIR and FFT benchmarks with error and overflow checks
  - Tags: `[fixed-point]` `[filters]` `[adaptive]`

- [ ] **Chapter 11 (p. 283): 'Real-Time Digital Signal Processing'**
  - [ ] **Lab 11 (p. 292): real-time setup; audio effects; triple and ping-pong buffers; overlap-add and overlap-save; equaliser in real time**
  - Output artefacts:
    - [ ] Buffering reference implementations: linear, circular, triple buffer, ping-pong
    - [ ] Frame-based FIR using overlap-add and overlap-save
    - [ ] Memory and latency comparison between triple buffer and ping-pong
  - Tags: `[buffering]` `[real-time]` `[filters]`

---

## Acceptance checks (apply to each Thin-Pod DSP kernel)

- [ ] Cycle budget recorded (cycles per sample or per frame)
- [ ] Latency recorded (frame size and pipeline latency)
- [ ] RAM and ROM usage recorded
- [ ] Numeric behaviour recorded (float vs fixed-point error, saturation, overflow)
- [ ] Test vectors committed (synthetic + at least one recorded vibration dataset)

