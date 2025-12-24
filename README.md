# Thin‑Pod — Vibration Fault‑Detection Pod (Hackster 'Best of 2025' Competition) 

A small, field‑deployable **vibration sensing pod** for rotating machinery that streams raw vibration windows to a nearby gateway for **DSP + TinyML anomaly/fault detection**.

---

## What this project is 

Thin‑Pod is a compact vibration node designed for motors, pumps, gearboxes, fans, and other rotating assets. The pod captures high‑bandwidth vibration from an analogue accelerometer, packetises short windows, and transmits them over a short‑range radio link to a gateway. The gateway runs classic condition‑monitoring DSP (RMS, crest factor, envelope, spectral bands) and optional edge‑AI (TinyML / anomaly detection) to produce **actionable health signals** instead of constant raw streaming.

---

## 1) Why it matters

Unplanned downtime is expensive — and avoidable. A low‑cost vibration node that can be deployed quickly helps:

- catch imbalance / looseness / misalignment early
- trend bearing wear before it becomes a 'stop the line' event
- reduce waste by extending machine life and preventing catastrophic failures
- cut energy use from poorly aligned or degrading machinery (often a hidden cost)

---

## 2) System overview

### Architecture (pod → gateway)

- **Thin‑Pod (sensor node):** samples vibration → builds fixed‑length windows → transmits.
- **Gateway (MCU dev board):** receives windows → runs DSP/ML → publishes results to LAN/Wi‑Fi dashboards.

Typical deployment: one gateway can listen to multiple pods and produce per‑asset health metrics.

### Data flow (high level)

1. **Analogue sensor** produces a mid‑rail vibration signal
2. **Front‑end** provides biasing + anti‑alias / envelope shaping (simple RC to start)
3. **ADC + DMA** captures a window (e.g., N samples at Fs)
4. **Radio link** sends the window (and metadata: timestamp, pod id, temperature, etc.)
5. **Gateway DSP/AI** extracts features + flags anomalies / faults
6. **Outputs**: health packet, trend charts, and (optionally) alerting

---

## 3) Key features

- **Open hardware + open firmware** (see Licensing)
- **High‑bandwidth vibration capture** using an analogue accelerometer (bearing harmonics live up here)
- **Window‑based streaming**: send short bursts of raw data, not 24/7 firehose
- **Gateway‑side DSP + TinyML**: compute features and infer faults where power and RAM are plentiful
- **Modular comms**: UWB for robust short‑range links and optional presence/ranging; optional Wi‑Fi backhaul module

---


## 3.1) Repository structure 
```
thin-pod/

├─ docs/                  # Design notes, app notes, datasheet list
├─ images/                # Renders, photos, diagrams
├─ firmware/              # Pod + gateway firmware (C/C++)
├─ tools/                 # Python analysis / feature extraction
├─ LICENCE-HARDWARE.md
├─ LICENCE-FIRMWARE.md
├─ LICENCE-DOCS.md
└─ LICENSE
```

## 4) Hardware used

This build is organised around **MCU dev boards** 

### Required (minimum viable build)

- **Gateway MCU dev board:** STM32 NUCLEO‑N657X0‑Q (receiver + DSP/ML), 
- **Pod and Gateway radio dev boards:** Qorvo DWM3001‑CDK (UWB link; pod‑side MCU handles sampling + packets)
- **Vibration sensor:** Analog Devices ADXL1005 (analogue MEMS accelerometer)

### Optional (nice‑to‑have)

- **Wi‑Fi backhaul:** ESP32‑C6 module/board (publish health metrics to LAN/cloud)
- **Mechanical:** enclosure, mounting hardware, strain relief, conformal coat (if needed)

---

## 5) Bill of Materials (BOM)


| Item | Example part | Qty | Notes |
|---|---:|---:|---|
| Gateway MCU dev board | STM32 NUCLEO‑N657X0‑Q | 1 | DSP/ML + system controller |
| Pod radio dev board | Qorvo DWM3001‑CDK | 1+ | One per pod |
| Accelerometer | ADXL1005 | 1 per pod | Analogue, high bandwidth |
| Passives (front‑end) | R/C network | 1 set per pod | Bias + simple filtering |
| Interconnect | ribbon / JST / headers | as needed | Keep leads short on analogue path |
| Power | battery pack / regulated supply | 1 per node | Choose for enclosure and duty cycle |
| (Optional) Wi‑Fi | ESP32‑C6 board/module | 1 | Backhaul from gateway |
| (Optional) Enclosure | 3D print / off‑the‑shelf | 1 | Environment dependent |

---

## 6) Bring‑up checklist (hardware):

1. Verify all rails and reference voltages
2. Confirm sensor bias is mid‑rail at rest
3. Inject a known sine into the ADC node and confirm the expected ADC counts
4. Only then connect the accelerometer output and test with gentle taps / a small shaker

---

## 7) Firmware & code

Repository locations (as they become available):

- `firmware/` — embedded code (pod sampling + radio packets; gateway receive + DSP/ML)
- `tools/` — Python scripts/notebooks for feature extraction, plotting, model training

**Gateway responsibilities (typical):**

- receive windows
- compute features (RMS, kurtosis, crest factor, band energy, envelope)
- run TinyML / anomaly detector (optional)
- publish a compact “health packet” upstream (MQTT/HTTP/etc.)

---

## 9) Results (what 'working' looks like)

A working end‑to‑end demo produces:

- a repeating stream of windows arriving at the gateway
- derived metrics that change when the machine state changes (idle vs load, imbalance added, etc.)
- a compact packet suitable for dashboards/alerts (feature vector + status)

---

## 10) Challenges & lessons learned

- Analogue front‑ends can be unintentionally 'too heavy' for MEMS outputs: input capacitance and RC choices matter.
- It’s worth validating the ADC node with a function generator before blaming firmware or radios.
- Windowing + burst transmission keeps the data model clean and makes downstream DSP reproducible.

---

## 11) Licensing

| Scope | License | File |
|---|---|---|
| Hardware design (schematics/PCB/mechanical) | **CERN–OHL–W v2.0** | `LICENCE-HARDWARE.md` |
| Firmware / software (C/C++, Python tools) | **MIT** | `LICENCE-FIRMWARE.md` and `LICENSE` |
| Documentation, images, diagrams | **CC‑BY‑4.0** | `LICENCE-DOCS.md` |

---

## Safety

This is experimental hardware. Use appropriate PPE and isolation when testing on live machinery. Regulatory compliance (EMC/RED/LVD) is not implied.

---

## Repo topics (suggested)

`vibration-analysis`, `condition-monitoring`, `predictive-maintenance`, `stm32`, `uwb`, `esp32-c6`, `low-power`, `kicad`, `accelerometer`, `edge-computing`, `iot`
