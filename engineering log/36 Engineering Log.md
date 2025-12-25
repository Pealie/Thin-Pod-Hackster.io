
## Engineering Log — High-level Summary Tues 23/12/2025

* **UWB firmware flashing and tooling stabilised.** J-Link Commander successfully erased and programmed the DWM3001CDK with the Qorvo SDK FreeRTOS binaries (HelloWorld and CLI). Initial `loadfile` issues were resolved; repeatable flash + reset + run flow established.

* **Serial/console path brought under control.** USB CDC devices enumerated reliably (COM ports confirmed). Terminal confusion (garbled output / “unknown command” noise) was traced largely to terminal settings/paste/echo behaviour rather than firmware faults; command entry became usable and deterministic.

* **Device identity and SDK build verified.** RTT and/or CLI output confirmed the Qorvo device/lot/part IDs and the DW3_QM33_SDK 1.1.0 build, including driver and UWB stack versions.

* **UWB application state and modes understood.** CLI command set explored (`STAT`, `STOP`, `SETAPP`, `SAVE`, etc.). “incompatible mode” behaviour observed and correctly interpreted as “top application running” rather than a crash.

* **Thin-Pod receiver path confirmed.** Thin-Pod in LISTENER produced continuous `JS0058{"LSTN":...}` receive events with timestamps and signal metrics (RSL/FSL ≈ −63 to −64 dB), demonstrating active frame reception.

* **Gateway ranging exchanges confirmed.** Gateway INITF output showed repeated `SESSION_INFO_NTF` measurements with `status="SUCCESS"` and consistent `distance[cm]` values, proving two-way ranging transactions and closing “Stage 2: UWB link (no sensor)” as complete.

* **Environmental sensitivity noted.** Measured/derived position/range behaviour was observed to change with physical cable routing/length, reinforcing that the setup is susceptible to near-field coupling/multipath and should be treated as a controlled-variable during future tests.

* **Stage 3 entry conditions established.** With UWB bring-up complete, the next work item is the sensor pipeline: reintroducing a non-loading anti-alias front-end at the ADC node and implementing an ADC window sampler for raw vibration windows suitable for packetisation.

**The 4 stages:**

**Stage 1** Flash + console + 'known-good firmware' (bring-up / CLI / flash / serial): ✅ done (COM port stable, CLI responding)

**Stage 2** UWB link bring-up (ranging, no sensor): ✅ done

* Thin-Pod shows JS0058{"LSTN":... rsl/fsl ...} → it’s hearing frames (listener is actively receiving)

* Gateway shows SESSION_INFO_NTF ... status="SUCCESS" distance[cm]=... → ranging is completing successfully

**Stage 3** Sensor path (ADXL1005 → ADC node → sampled windows): partially done (DC bias fixed by removing capacitor)

**Stage 4** End-to-end pipeline (Thin-Pod stream → Gateway ingest → NUCLEO DSP/TinyML → backhaul): not yet



