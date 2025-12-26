### Executive Summary


## v2.2 Reconciliation Update (architecture lock-in)
- **AI location:** Pod streams raw vibration windows; **all DSP + TinyML/NanoEdge run on the gateway**.
- **Main MCU (gateway):** **STM32 NUCLEO-N657X0-Q**.
- **ADC path (pod):** **ADXL1005 → Vout→RC(8K/680pF)→DWM3001 (nRF52833) ADC (P0.28, SAADC AIN4, J10, pin15)**.  
- **Optional Power (pod):** **AEM30941** energy harvester with **5F super-cap**.

**Vibration-Based Fault Detection on STM32 – Version 2.0**

---

#### 1  | Project Aim

Deliver an **ultra-compact, open-source hardware node** that mounts on rotating equipment, such as a motor or gearbox, streams vibration data by ultra-wideband, and performs **real-time fault diagnosis** (mis-alignment, imbalance, early bearing defects) using both:

* **Supervised TinyML** (STM32Cube.AI-generated MLP)
* **Unsupervised NanoEdge AI** (self-learning anomaly library)

Results are transmitted locally over 100Mb/s RJ45 Ethernet or Wi-Fi and, with sub-nanosecond synchronisation, can be fused across multiple machines.

---

#### 2  | What’s New in Version 2.2

| Area         | V1.0 (proof of concept)   | **V2.0 (current build)**                                          | Benefit                                              |
| ------------ | ------------------------- | ----------------------------------------------------------------- | ---------------------------------------------------- |
| **MCU**      | STM32F411 (M4, v7E-M)     | **STM32 NUCLEO-N657X0-Q (M33, v8-M, TrustZone)**                  | 1.6× DSP speed, lower run-current, secure-boot ready |
| **Radio**    | ESP32-C6 (Wi-Fi)          | **Dual-radio**: ESP32-C6 (2.4 GHz) **+ UWB anchor (DWM3001-CDK)** | Adds <10 cm ranging and <50 ns time-stamp jitter     |
| **Sensor**   | ADXL345 (±16 g, 3 kHz BW) | **ADXL1005** (±100 g, 45 kHz BW, analogue)                        | Captures bearing harmonics >10 kHz                   |
| **Pod link** | 2m CAT-6 + RJ-45          | **Battery pod + UWB / BLE**                                       | Cable-less, installs in minutes                      |
| **Power**    | USB only                  | **5F super-cap coin-cell + 10 mF burst cap** (harvester option)   | Months of life; path to battery-free                 |
| **Open HW**  | Single carrier PCB        | **Carrier + Sensor-Pod PCBs** (CERN-OHL-S)                        | Clear separation; pod can evolve independently       |

---

#### 3  | Core Pipeline

1. **Sampling** – ADXL1005 → 192 kS/s ADC DMA on STM32 NUCLEO-N657X0-Q
2. **DSP** – RMS, envelope, 4 k FFT, spectral-kurtosis (option)
3. **Edge AI**

   * *TinyML classifier* (10–15 handcrafted features → 16-neuron MLP)
   * *NanoEdge anomaly detector* (self-tuning, on-device learning)
4. **UWB timestamp** aligns windows across pods; Wi-Fi/MQTT publish health, range, and spectrum snapshots.

Latency: **< 40 ms** from sensor to fault decision.

---

#### 4  | Open-Source Deliverables

* **KiCad v2.0 schematics & PCBs** (carrier + pod) under CERN-OHL-S.
* **FreeCAD/STL enclosures** (IP-54, gyroid acoustic insert).
* **MIT-licensed firmware**: STM32CubeIDE project, ESP-IDF anchor/tag apps.
* **Python notebooks** for model training, spectral-kurtosis, live plotting.
* **Demo data-set & Grafana dashboard** (MQTT → InfluxDB).

---

#### 5  | Roadmap Highlights

| Q3-25                                                                 | Q4-25                                                       | 2026                                                                      |
| --------------------------------------------------------------------- | ----------------------------------------------------------- | ------------------------------------------------------------------------- |
| Piezo harvester variant • Secure-boot + OTA • LC3 BLE audio streaming | Multimodal fusion (acoustic mic) • Battery-free field trial | Federated learning gateway • CE/FCC full compliance • OSHWA certification |

---

#### 6  | Value Proposition

* **Industrial-grade diagnostics** at **< approx £300/node** (<20 % of commercial analysers).
* **Modular** – swap sensor boards (high-g MEMS, IEPE piezo) without redesigning gateway.
* **Research-ready** – sub-nanosecond sync enables order-analysis and array beamforming; open data accelerates academic collaboration.
* **Business model** – sell kits, cloud analytics, or dual-licence commercial forks while the hardware/IP stays open.

---

Version 2.2 positions the project as a **low-cost, easily reproducible yet technically advanced** platform that blends classic DSP, TinyML, adaptive AI, and precision UWB timing—ready for early pilot deployments.
