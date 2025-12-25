# rotor-fault-detection-c

A compact C pipeline for **bearing fault detection** (outer/inner race, etc.):

- Band-pass (windowed-sinc FIR) → **Hilbert envelope** (FFT-based analytic signal)
- **Welch PSD** (Hann, 50% overlap)
- Predicts **BPFO/BPFI/BSF/FTF** from geometry
- Peak-matches with tolerance and SNR thresholds
- Optional **tach-based order tracking** (equal-angle resampling)
- Emits **`diagnostic.json`** (clean schema)

## Build
```bash
make                # builds ./rotor_fd
```

## Run (synthetic)
```bash
./rotor_fd
```

## Run (CSV input)
```bash
# CSV: either single column accel, or two columns accel,tach (0..1 pulses)
./rotor_fd --input my_signal.csv --fs 51200 --nperseg 65536 --band 4000 8000 \
  --geom n=8,d=0.010,D=0.050,beta_deg=0,rpm=1800 --order
```

### CLI
```
--input <path>           CSV with 1col(acc) or 2col(acc,tach)
--fs <Hz>                Sampling rate (default 51200)
--duration <s>           Synthetic duration (default 4)
--band <lo> <hi>         Band-pass in Hz (default 4000 8000)
--nperseg <N>            Welch segment length (pow2; auto-bounded)
--geom n=..,d=..,D=..,beta_deg=..,rpm=..   Bearing geometry
--order                  Enable tach-based order tracking (if tach present)
--out <path>             JSON output (default diagnostic.json)
--q15simulate            Track Q15 headroom/overflow (diagnostics only)
--name <string>          Run label in JSON
```

### Output: `diagnostic.json`
- `run`: timestamp, seed, run name
- `signal`: fs_hz, duration_s, window, nperseg, band_hz, order_tracking
- `geometry`: n, d, D, beta_deg, rpm
- `predictions_hz`: fr, BPFO, BPFI, BSF, FTF
- `detections_hz`: peak dicts (freq, snr_db, df, harmonic)
- `detections_order` (if order tracking): peak dicts in **orders**
- `decision`: {fault_class, confidence, rationale}

> Note: order tracking resamples the **envelope** to equal-angle samples using the tach pulses (1 pulse/rev assumed).
