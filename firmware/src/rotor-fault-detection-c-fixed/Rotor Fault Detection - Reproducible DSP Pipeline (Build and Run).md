### `firmware/src/rotor-fault-detection-c-fixed` (code section summary)

A self-contained **C11 reference implementation** of the Thin-Pod rotor and bearing fault-detection pipeline, built as a small CLI tool (`rotor_fd`) via the included Makefile.

Key points:

* Implements the full DSP chain: **windowed-sinc FIR band-pass → Hilbert envelope (FFT analytic signal) → Welch PSD**.
* Predicts characteristic bearing fault lines from geometry, then **peak-matches BPFO, BPFI, BSF, FTF** with tolerance and SNR gating.
* Includes optional **tach-based order tracking** (equal-angle resampling) for speed-robust diagnostics.
* Provides two execution paths:

  * **Float pipeline** for algorithm development and inspection.
  * **Strict fixed-point Q15 pipeline** (`--fixed`) plus optional Q15 headroom tracking (`--q15simulate`) to keep the algorithm portable to embedded targets.
* Emits a clean, machine-readable **`diagnostic.json`** containing run metadata, geometry, predicted frequencies, detected peaks, and the final decision rationale.
* Adds a lightweight **SMT2 (Z3) check** (`fixedpoint_checks.smt2`) to sanity-check Q15 saturation and butterfly arithmetic used by the fixed-point path.

Here is how the files in `rotor-fault-detection-c-fixed` get used in practice.

## What each file is for

* `main.c`
  The whole program. Builds a single executable called `rotor_fd` that runs the DSP pipeline and writes a diagnostic JSON report.

* `Makefile`
  One-command build (`make`) and cleanup (`make clean`) using `gcc` and `libm`.

* `README.md`
  The quick-start: build, run modes, CLI flags, and the expected CSV input format.

* `fixedpoint_checks.smt2`
  Optional formal sanity checks for the Q1.15 fixed-point arithmetic using Z3.

* `diagnostic.json`
  Example output produced by a run. By default the program overwrites `diagnostic.json` unless `--out` is used.

## Build

From the directory containing the files:

```bash
make
```

This compiles `main.c` into:

```bash
./rotor_fd
```

Cleanup:

```bash
make clean
```

## Run (synthetic demo)

Runs an internal synthetic signal generator and writes `diagnostic.json`:

```bash
./rotor_fd
```

## Run (real data from CSV)

Input CSV formats supported:

* **1 column**: `accel`
* **2 columns**: `accel,tach` where `tach` is a pulse signal (0..1) and assumes **1 pulse per revolution**.

Example:

```bash
./rotor_fd --input my_signal.csv --fs 51200 --nperseg 65536 --band 4000 8000 \
  --geom n=8,d=0.010,D=0.050,beta_deg=0,rpm=1800
```

If the CSV includes a tach column and order tracking is desired:

```bash
./rotor_fd --input my_signal.csv --fs 51200 --geom n=8,d=0.010,D=0.050,beta_deg=0,rpm=1800 --order
```

## Fixed-point path (Q15)

Runs the strict Q15 pipeline:

```bash
./rotor_fd --fixed
```

Add Q15 headroom and overflow monitoring (diagnostics only):

```bash
./rotor_fd --fixed --q15simulate
```

## Output location

Default output is `diagnostic.json`. A different filename can be set:

```bash
./rotor_fd --out results_run_001.json --name 'bench_run_001'
```

## Run the Z3 fixed-point checks (optional)

If Z3 is installed:

```bash
z3 fixedpoint_checks.smt2
```

Expected output is typically `sat`, meaning the stated constraints are internally consistent. This is a lightweight guardrail, not a full correctness proof.

## Q15

‘Q15’ is a **fixed-point number format** commonly used on microcontrollers and DSPs.

* Stored in a **signed 16-bit integer**.
* Interpreted as **1 sign bit + 15 fractional bits** (often described as *Q1.15*).
* Numeric range is approximately **[-1.0, 1.0)**, more precisely **-1.0 to (1 − 2⁻¹⁵)**.
* Conversion idea:

  * float → Q15: `q = round(x * 32768)` (with saturation to [-32768, 32767])
  * Q15 → float: `x = q / 32768`
* Arithmetic note:

  * Multiplying two Q15 values produces a wider intermediate (conceptually Q30) and then shifts back down (with rounding and saturation) to return to Q15.
* Value proposition: predictable performance and no floating-point dependency, at the cost of careful scaling and overflow management.

## Z3

‘Z3’ is a **Satisfiability Modulo Theories (SMT) solver** from Microsoft Research.

* Takes logical constraints over theories such as bit-vectors, integers, reals, arrays, and more.
* Answers whether constraints are **satisfiable** (‘sat’) or **unsatisfiable** (‘unsat’).
* If satisfiable, can produce a **model** (a concrete counterexample or witness assignment).
* In embedded and fixed-point DSP work, Z3 is often used to check properties like:

  * ‘No overflow occurs in this computation’
  * ‘This shift is safe under all input ranges’
  * ‘Saturation logic matches intended bounds’
* The `.smt2` file is written in the standard SMT-LIB v2 syntax that Z3 can consume directly.


