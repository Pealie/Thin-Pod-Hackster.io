Today’s session produced a clean, Hackster-ready simplification of the Thin-Pod and Gateway wiring.

The Gateway interconnect was narrowed to a single shared SPI bus (SPI5 on NUCLEO-N657X0-Q CN14) feeding both peripherals, with distinct chip-select lines per device. A bench-friendly pin map was written down for the DWM3001C-CDK on J10 (CLK/MISO/MOSI/CS, plus IRQ and RESET) and for the Seeed ESP32-C6 (shared SPI lines plus its own CS, INT/host-wake, and EN/RST). A one-page ‘shared-bus wiring sketch’ was prepared and Markdown formatting was corrected by placing the sketch into fenced code blocks for reliable rendering.

Thin-Pod pinouts were simplified to the essentials for the Hackster scope, excluding energy harvesting. The signal chain was corrected and locked down as ADXL1005 Vout → RC (8 kΩ / 680 pF) → DWM3001C-CDK ADC input at J10 pin 15, with common ground and 3V3 power, and an ultra-minimal wiring block.

A quick analogue sanity check was added to the record: the 8 kΩ / 680 pF RC forms an approximately 29 kHz low-pass corner, relevant for expected vibration bandwidth and ADC settling considerations.

The firmware was examined with regard to the Thin-Pod system as a future architecture pattern: clear stage boundaries, minimised trusted code, signed-update mindset, and ‘observability engineering’ via deterministic boot markers, reason codes, and disciplined debug access, applied across the STM32N6 Gateway, the DWM3001 Thin-Pod node, and the ESP32-C6 network subsystem.



Additional progress:



The Thin-Pod analogue-to-ADC interface was pinned down more precisely: ADXL1005 Vout is routed through the RC network (8 kΩ series, 680 pF to ground) into the Thin-Pod DWM3001C-CDK ADC input at J10 pin 15, treated as the measurement boundary for subsequent ADC and UWB tests. 



A staged, end-to-end validation plan was written for the full data path: (1) analogue sanity and ADC electrical validity, (2) Thin-Pod DWM3001 ADC bring-up with known-input ladder tests and window timing checks, (3) UWB transport integrity using synthetic payloads followed by real ADC payload reassembly and statistics matching, and (4) final-hop verification over SPI from the Gateway DWM3001 to the STM32. The plan formalises pass criteria at each stage and separates ‘prove UWB reception’ from ‘prove SPI handoff’.



A decision point was resolved for the final hop: an oscilloscope can confirm signalling quality but cannot validate SPI data transfer without an active master. Therefore, STM32 firmware is not required to confirm ADC capture and UWB transport up to Gateway buffering, but a minimal STM32 SPI master reader (or an alternative external SPI master) is required to validate reliable extraction of received windows from the Gateway DWM3001 over SPI.



The recommended STM32 bench validation approach was specified: use STM32CubeIDE with a CubeMX-generated project (IOC + HAL + startup + linker script), and implement a minimal SPI5 master reader that deselects the ESP32-C6, pulses CDK reset, uses PB9 as an EXTI ‘data ready’ interrupt, clocks out a small fixed-length frame per interrupt, computes a CRC32, and emits short diagnostics via SWV/ITM or UART. The required CubeMX pin and peripheral configuration was enumerated against the bench pinouts (SPI5 on CN14, CDK CS on PA3, IRQ on PB9, reset on PD0, optional ESP32 CS held high).



ADC configuration on the Thin-Pod DWM3001 was defined in detail as an nRF52833 SAADC setup on P0.28 (AIN4), which is the signal routed from CDK J10 pin 15. The configuration choices were narrowed to a robust bring-up baseline: internal 0.6 V reference, gain 1/6 to cover 3.3 V full-scale, 12-bit resolution, conservative acquisition time (20 µs or longer) due to the 8 kΩ series impedance, offset calibration, and timer+PPI-triggered sampling into double-buffered 2048-sample windows using EasyDMA, to minimise sampling jitter under FreeRTOS and UWB activity.



