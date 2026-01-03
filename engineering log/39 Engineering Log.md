## Engineering Log entry (2026-01-03)

* Qorvo DWM3001C-CDK bring-up progressed to a stable ‘known-good’ baseline: FreeRTOS images successfully flashed via SEGGER J-Link Commander, RTT output verified (‘Hello World’ plus device IDs), and the USB serial CLI confirmed operational with stable COM enumeration (local echo issue resolved).
* Stage 2 UWB link validated end-to-end using the Qorvo SDK apps:

  * Thin-Pod in `LISTENER` continuously reported received frames (`JS0058{... rsl/fsl ...}`), confirming RF reception and ongoing traffic.
  * Gateway side reported repeated `SESSION_INFO_NTF` entries with `status="SUCCESS"` and varying `distance[cm]`, confirming ranging completion and a live exchange loop.
* Ranging versus 2D position clarified: ranging confirmed sufficient for current validation; 2D solve deferred to a future anchor-based coordinate setup.
* Stage 3 analogue sensor path diagnosis completed to the key failure point: the ADC node loading issue traced to the 680 pF capacitor to ground. With that capacitor removed, ADXL1005 `VOUT` returned to the expected mid-rail resting point (about 1.6–1.7 V). J10 pin 15 confirmed as the ADXL input into the DWM3001C-CDK SAADC channel (nRF52833 P0.28, SAADC AIN4).
* Initial instrumentation checks captured: DVM readings showed ADXL `VOUT` around 1.74 V at rest; handheld scope configuration appeared to be limiting visible transient capture during tapping, so further scope setup refinement remains.
* Gateway interconnect labelling corrected: the Gateway STM32 NUCLEO link to the Gateway DWM3001C-CDK is treated as an SPI data-plane (SPI5 plus CS, IRQ, optional reset). UART is appropriate for console paths, not the main data-plane link.
* Power architecture revision recorded in the text schematic: AEM harvester block removed; AA pack via JST added into Pololu S7V8F3 to create `3V3_MAIN`, with optional PTC and reverse-polarity protection. VBATT sense divider added for logging, brownout policy notes added, and optional regulator enable power-gating noted.
* Pin mapping confidence improved for the Wi-Fi option: CN3 pin 17 = PC0 confirmed as the intended STM32 mapping for ESP32-C6 `EN/RST`; host-wake INT mapping noted; chip select identification work advanced on the header diagram.

**Current stage status**

* Stage 1 (flash + console): complete
* Stage 2 (UWB ranging): complete
* Stage 3 (sensor path): partially complete (DC bias fixed; AA/anti-alias and ADC window sampler still pending)
* Stage 4 (Thin-Pod stream → Gateway ingest → STM32 DSP/TinyML): not started
