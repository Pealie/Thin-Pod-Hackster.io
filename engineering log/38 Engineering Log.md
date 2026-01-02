### Engineering Log summary

* **Stage 1 bring-up remains complete**

  * DWM3001CDK FreeRTOS images flashed successfully and console access proven stable (COM port enumerates cleanly, CLI responds reliably).
  * PuTTY local-echo and terminal behaviour corrected, removing spurious ‘unknown command’ artefacts caused by echoed banner text.

* **Stage 2 UWB link remains complete and repeatable**

  * Thin-Pod in **LISTENER** visibly receiving frames (continuous `JS0058{"LSTN":...}` reports with stable `rsl/fsl` around the mid `-60 dB` range).
  * Gateway running **INITF** producing continuous successful ranging notifications (`SESSION_INFO_NTF ... status="SUCCESS" distance[cm]=...`), demonstrating a working two-node exchange loop.
  * Noted sensitivity of apparent positioning to USB cable placement and length, consistent with bench RF and grounding effects influencing near-field conditions.

* **Stage 3 sensor path validation advanced from DC bias to ‘dynamic evidence’**

  * ADXL1005 Vout verified with DVM at **~1.74 V** at rest, consistent with a mid-rail biased analogue output (no RC loading in place).
  * A brief Vout change observed on tapping with DVM, indicating small transient response.
  * Scope configuration progressed from an uninformative view to a longer timebase view (`H=20 ms`) where **bursty activity appears during tapping**, contrasting with the earlier near-flat baseline capture. This supports the presence of impulse-like dynamics, while also highlighting remaining scope setup limitations (vertical scale and coupling choices still constraining visibility).

* **Pin and control-line mapping clarity improved for the Gateway stack**

  * Confirmed the NUCLEO mapping **‘CN3 pin 17 = PC0’** as documented, and treated as the candidate **RST/EN** control line for the ESP32-C6 side.
  * INT (host-wake) line mapping captured as **CN14 D14 (PC1)** toward the ESP32-C6 interrupt input.
  * Sensor-to-ADC routing recorded as **ADXL1005_VOUT → DWM3001CDK J10 pin 15 → nRF SAADC AIN4 (P0.28)**.

* **Stage status checkpoint**

  * Stage 1: ✅
  * Stage 2: ✅
  * Stage 3: ⚠️ progressing (analogue signal plausibly present, next step is SAADC sampling and transport)
  * Stage 4: not started

