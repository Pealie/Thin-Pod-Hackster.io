# Thin-Pod TinyML: Feature Vector and Packet Format Spec (TPFV1 + TP-LINK v1)

This document defines a stable, field-friendly vibration feature vector produced on Thin-Pod and a version-resilient packet format for transport to a PoE-powered gateway, where DSP + TinyML inference is performed.

## Summary

- **Preferred split:** features on Pod, model on Gateway
- **Feature schema:** **TPFV1** (Thin-Pod Feature Vector v1), 16 float32 features (maskable)
- **Link framing:** **TP-LINK v1** (binary header + TLV payload + optional CRC32)
- **Versioning:** framing evolves via `proto_ver`; feature definitions evolve via `schema_id`; optionality via `feature_mask`; extensibility via TLVs

---

## 1. Feature vector definition: TPFV1 (schema_id = 1)

### 1.1 Windowing assumptions

Typical starter configuration for rotating machinery:

- Sample rate `fs_hz`: **10_000**
- Window length `N`: **512** samples
- Hop `H`: **256** samples (50% overlap)
- Sample type: signed **int16 ADC counts** (or scaled units, such as 'g')

Per window:

- `x[n]` is the raw signal
- `dc_mean = mean(x)`
- `x0[n] = x[n] - dc_mean`

Optional (recommended for FFT-derived features):

- Apply Hann window before FFT: `xw[n] = hann(n) * x0[n]`

### 1.2 Output representation

- Features are transmitted as **IEEE-754 float32**, little-endian.
- Features are ordered by **feature index** (0..15), with omissions controlled by a bitmask.

### 1.3 Feature list (16 features)

The list below defines the canonical feature indices for **TPFV1**.

#### Time-domain (features 0..7)

0. `dc_mean`  
   Mean of the raw window before DC removal (ADC counts or scaled units)

1. `rms`  
   RMS of DC-removed signal: `sqrt(mean(x0^2))`

2. `peak_abs`  
   Maximum absolute value of DC-removed signal: `max(|x0|)`

3. `p2p`  
   Peak-to-peak of DC-removed signal: `max(x0) - min(x0)`

4. `crest_factor`  
   `peak_abs / rms` (0 if `rms` is 0)

5. `kurtosis_excess`  
   Excess kurtosis of DC-removed signal (use epsilon guards)

6. `zcr`  
   Zero-crossing rate (crossings per sample, 0..1) computed on `x0`

7. `env_rms`  
   RMS of an envelope signal. Two valid implementations:
   - Digital: `env = LPF(|x0|)` then `rms(env)`
   - Analogue envelope channel (if present): RMS of that channel window

If `env_rms` is not available, it may be omitted using the `feature_mask`.

#### Frequency-domain (features 8..13)

Let `P[k] = |FFT(xw)[k]|^2` be the power spectrum.

8.  `bandpower_0_200`  
9.  `bandpower_200_800`  
10. `bandpower_800_2000`  
11. `bandpower_2000_5000`  
Bandpower is the sum (or mean) of `P[k]` over FFT bins whose centre frequencies lie within the band.

12. `spec_centroid_hz`  
    Spectral centroid in Hz: `sum(f[k]*P[k]) / sum(P[k])`

13. `spec_flatness`  
    Spectral flatness: `geomean(P) / mean(P)` (use epsilon guards)

#### Dominant peak (features 14..15)

Define a search band such as **20 Hz to 1000 Hz**, excluding DC.

14. `peak1_freq_hz`  
    Frequency of the strongest spectral peak in the search band

15. `peak1_power`  
    Power value `P[k]` at the peak bin

### 1.4 Normalisation conventions

To keep the feature schema stable, normalisation is treated as **model-side** (Gateway) by default.

Two supported approaches:

- **Raw features:** Pod transmits unnormalised features. Gateway applies per-feature z-score using stored training stats.
- **Light scaling:** Pod may scale features into a known numeric range, but the meaning of each feature remains unchanged.

If on-Pod normalisation is later required, it should be introduced as:
- a new `schema_id` (preferred), or
- a new TLV describing normalisation parameters applied (acceptable if unambiguous)

---

## 2. Packet format: TP-LINK v1

### 2.1 Endianness

- Little-endian for all multi-byte fields.

### 2.2 Fixed header (32 bytes)

```
0  : u16  magic        = 0x5450        // 'TP'
2  : u8   proto_ver     = 1             // TP-LINK version
3  : u8   msg_type      = 0x01          // 0x01=FEATURES, 0x02=RAW_WINDOW, 0x03=HEARTBEAT
4  : u8   header_len    = 32            // bytes, allows extension
5  : u8   flags         // bit0: crc32_present, bit1: timestamp_valid, bit2: ack_req, etc.
6  : u16  seq           // increments per packet
8  : u32  pod_id        // stable unique ID (hash/serial)
12 : u64  t_us          // microseconds since boot (or epoch if available)
20 : u32  fs_hz         // sample rate
24 : u16  win_n         // N
26 : u16  win_h         // hop/stride H
28 : u16  payload_len   // bytes following header, before CRC
30 : u16  hdr_crc16     // optional: CRC16 over header (0 if unused)
```

### 2.3 Payload: TLV list

Payload is a sequence of TLVs. Unknown TLV types are skipped using `length`.

```
TLV:
0 : u8   type
1 : u8   reserved      // set 0
2 : u16  length        // bytes of value[]
4 : u8   value[length]
```

### 2.4 TLV types (initial set)

#### 0x10 FEATURES_F32

Value:

```
u16 schema_id        // 1 for TPFV1
u16 feature_count    // number of float32 values included
u32 feature_mask     // bit i = feature i present (TPFV1 uses bits 0..15)
f32 features[]       // feature_count floats, ordered by increasing feature index
```

`feature_mask` permits stable omission of optional features (for example `env_rms`).

#### 0x11 RAW_WINDOW_I16 (lab / debug)

```
i16 samples[win_n]
```

#### 0x12 POD_STATUS (optional)

```
u16 vbat_mv
i16 temp_c_x100
u16 flags
```

#### 0x13 MODEL_HINT (optional)

```
u32 model_id         // assists gateway model selection
```

### 2.5 Footer

If `flags.bit0` is set, append:

- `u32 crc32` computed over `header || payload`

### 2.6 Typical FEATURES packet size

For TPFV1 with all 16 features present:

- Header: 32 bytes
- TLV header: 4 bytes
- FEATURES metadata: 8 bytes
- Feature payload: 16 * 4 = 64 bytes
- CRC32: 4 bytes

Total: **112 bytes**

---

## 3. Gateway model interface and mapping

This section defines a concrete, low-friction mapping from decoded TPFV1 features into gateway inference, including model IDs, normalisation, and smoothing.

### 3.1 Canonical model input vector

The gateway constructs an input vector `v` of length **16** for TPFV1:

- Initialise all elements as NaN or 0
- For each feature index `i` present in `feature_mask`, copy the next float from the TLV into `v[i]`
- For any missing feature index, apply a deterministic policy:

Recommended policy for TPFV1:
- Missing `env_rms` (index 7): fill with `0.0`
- Any other missing index: reject packet for ML inference and log as schema violation

This preserves robustness while keeping the 'optional feature' scope narrow.

### 3.2 Model selection

Model selection should be stable and explicit. Two keys are used:

1. `schema_id` (must match model training schema)
2. optional `model_id` (TLV 0x13)

If `model_id` is absent, a gateway-side default model is used for the `schema_id`.

Recommended initial model IDs:

- `model_id = 0x00010001` : **LR_Binary_v1** (logistic regression, normal vs fault)
- `model_id = 0x00010002` : **MLP_3Class_v1** (tiny MLP, normal/imbalance/looseness)
- `model_id = 0x00010003` : **OC_Anomaly_v1** (one-class anomaly score, eg. Gaussian / Mahalanobis on features)

The high 16 bits (`0x0001`) are reserved for `schema_id = 1` family to reduce collisions.

### 3.3 Normalisation (gateway-side z-score)

For each model, store:

- `mu[16]` (mean per feature)
- `sigma[16]` (std dev per feature, epsilon-limited)

Normalised vector:

`z[i] = (v[i] - mu[i]) / max(sigma[i], eps)`

This is applied before LR or MLP inference.

Rationale:
- Training stats remain gateway-resident and model-specific.
- Pod feature meaning remains stable across deployments.

### 3.4 Inference outputs

#### Logistic regression (binary)

- Output: `p_fault` in [0, 1]
- Classes:
  - `p_fault < threshold_low` => normal
  - `p_fault > threshold_high` => fault
  - else => uncertain (optional)

Hysteresis is recommended (`threshold_low` < `threshold_high`) to avoid chatter.

#### Tiny MLP (3-class)

- Output: probability vector `p = [p_normal, p_imbalance, p_looseness]`
- Prediction: `argmax(p)`
- Confidence: `max(p)`

### 3.5 Temporal smoothing (recommended)

Window-by-window predictions may flicker; smoothing produces instrument-like stability.

Two simple options:

**A) Majority vote (labels)**
- Maintain a FIFO of last `K` predicted labels (K = 7 or 9)
- Output label = mode of FIFO

**B) EMA (probabilities)**
- Maintain `p_ema = alpha * p + (1-alpha) * p_ema`
- alpha in [0.1, 0.3] often works well
- Output label = argmax(p_ema)

EMA is recommended for multi-class models.

### 3.6 Suggested gateway API (minimal)

A minimal gateway module interface:

- `decode_packet(bytes) -> (header, tlvs)`
- `extract_features(tlvs) -> (schema_id, feature_mask, v[16], model_id_optional)`
- `infer(schema_id, model_id, v[16]) -> (label, confidence, scores[])`
- `log_result(header, label, confidence, key_features_subset)`

### 3.7 Pseudocode

```c
bool handle_features_packet(const uint8_t* pkt, size_t len) {
  Header h;
  TLVList tlvs;
  if (!decode_tp_link(pkt, len, &h, &tlvs)) return false;

  FeaturesF32 f;
  if (!find_features_f32(tlvs, &f)) return false;
  if (f.schema_id != 1) return false; // TPFV1 only

  float v[16];
  bool ok = assemble_tpfv1_vector(f.feature_mask, f.features, f.feature_count, v);
  if (!ok) return false;

  uint32_t model_id = default_model_for_schema(1);
  ModelHint mh;
  if (find_model_hint(tlvs, &mh)) model_id = mh.model_id;

  float z[16];
  zscore_normalise(model_id, v, z);

  Inference out = infer_model(model_id, z);
  Inference smoothed = smooth(out);

  log_inference(h, smoothed);
  return true;
}
```

---

## 4. Notes for field deployment

- **Bandwidth control:** feature packets are ~112 bytes, making continuous streaming feasible without raw-window saturation.
- **Schema stability:** avoid changing TPFV1 semantics once deployed; evolve via `schema_id`.
- **Debug path:** RAW_WINDOW_I16 TLV (0x11) remains available for lab capture and for investigating misclassifications.
- **Calibration:** absolute amplitude calibration is optional for anomaly detection; consistent mounting and consistent gain are more important.

---

## 5. Change log

- v1.0: Initial definition of TPFV1 (16 features) and TP-LINK v1 (header + TLV + optional CRC32), plus gateway model mapping (model IDs, normalisation, smoothing).
