# Helium (MVE) enablement and audit checklist for Thin-Pod

A compact, audit-friendly checklist to confirm Helium (MVE) is correctly enabled and actually used in the STM32N6 (Cortex-M55) build.

## 1) Build configuration
- Maintain **two build profiles**:
  - **Baseline**: no MVE, portable scalar.
  - **MVE**: Cortex-M55 with MVE enabled.
- Ensure **every compilation unit that matters** uses consistent target flags:
  - application
  - DSP or ML libraries
  - any performance-critical modules

## 2) Compiler target flags
- Confirm the project targets a Helium-capable core (STM32N6 uses Cortex-M55).
- Ensure flags permit MVE instruction selection.
- Generate a build artefact that makes the flags visible:
  - `compile_commands.json`
  - verbose build log
  - map file notes

Example verification approach:
- In CI, grep the verbose compile line for the selected CPU/arch flags and store in logs as an artefact.

## 3) Compile-time feature macros
Add a tiny compile-time guard in a common header (or one test module):

```c
#if defined(__ARM_FEATURE_MVE)
  #define THINPOD_HAS_MVE 1
#else
  #define THINPOD_HAS_MVE 0
#endif

#if defined(__ARM_FEATURE_MVE_FP)
  #define THINPOD_HAS_MVE_FP 1
#else
  #define THINPOD_HAS_MVE_FP 0
#endif
```

Then:
- For the MVE build, require `THINPOD_HAS_MVE == 1`.
- For the baseline build, require `THINPOD_HAS_MVE == 0`.

This catches accidental ‘wrong target’ builds immediately.

## 4) Confirm real instruction emission
Relying on macros alone is not enough. Confirm the binary contains MVE instructions.

- Produce disassembly (`.elf` → `.s`):
  - `objdump -d` or IDE disassembly view.
- Check for **MVE instruction mnemonics** in hot functions (vector loads, vector MACs, predicated ops).
- Alternatively, use compiler vectorisation reporting if available, and archive the report.

Pass criteria:
- At least one known-hot kernel shows clear vectorised code in the MVE build and scalar code in baseline.

## 5) Library selection (CMSIS-DSP / CMSIS-NN)
If CMSIS is used, confirm the build selects Helium variants consistently.

- Build defines typically used by CMSIS DSP variants include:
  - `ARM_MATH_MVEI` (integer Helium path)
  - `ARM_MATH_MVEF` (float Helium path)
- Confirm the library objects were compiled with the same MVE-capable target flags as the application.

Pass criteria:
- Link map shows the expected Helium-optimised objects being pulled in for MVE build.
- Baseline build links scalar implementations.

## 6) RTOS and interrupt context handling
Because Helium shares the FP register file, context handling becomes a real configuration item.

- Confirm toolchain and runtime save and restore FP or MVE state when needed.
- If an RTOS is in use:
  - confirm FP context save policy matches actual use
  - confirm ISR usage of FP or MVE is either avoided or correctly handled
- If no RTOS, confirm interrupt entry does not corrupt FP state when Helium code runs in thread context.

Pass criteria:
- Stress test with interrupts firing during a known Helium kernel produces deterministic output.

## 7) Benchmark kernels for ‘does it help’ and ‘is it used’
Add two micro-benchmarks with fixed data to avoid measurement noise:
- Dot product or MAC loop (DSP-like)
- FIR or small convolution

Measurement:
- Use a cycle counter (where available) or a high-resolution timer.
- Compare baseline vs MVE builds on identical input buffers.

Pass criteria:
- MVE build shows a clear cycle reduction for at least one kernel.
- Numeric output remains within expected tolerance for fixed-point or floating-point mode.

## 8) Data layout and alignment checks
Helium performance is often memory-limited.

- Confirm hot buffers are contiguous and aligned.
- Avoid aliasing ambiguity in hot loops (use `restrict` where valid).
- Prefer SoA or packed formats that match vector loads rather than scattered access.

Pass criteria:
- Benchmarks show stable scaling with buffer sizes and do not collapse due to bandwidth or misalignment.

## 9) CI guardrails
Add simple checks that prevent regressions:
- Baseline build job: asserts `THINPOD_HAS_MVE == 0`
- MVE build job: asserts `THINPOD_HAS_MVE == 1`
- Disassembly grep job: requires at least one MVE mnemonic in a named kernel for MVE build
- Performance smoke test: flags large slowdowns beyond a threshold
