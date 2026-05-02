# Thin-Pod Gateway rev 0.1: CN3/CN15 consolidated NUCLEO pinout

**Date:** 2 May 2026  
**Scope:** NUCLEO-N657X0-Q gateway connections only  
**Constraint:** Use **CN3** and **CN15** only on the NUCLEO side. Move all NUCLEO connections away from **CN5**, **CN13** and **CN14**.  
**Out of scope:** The Thin-Pod node section is intentionally ignored for now.

## Purpose

This note consolidates the Gateway wiring so that the NUCLEO-N657X0-Q uses only the **ST morpho CN3 and CN15 headers** for the DWM3001C-CDK and Seeed Studio XIAO ESP32-C6 connections.

The original bench-ready note used the Arduino/Zio header presentation for several pins, especially **CN14** for SPI5 and control, and **CN5** for power. The same STM32 pins are available on the ST morpho headers, so the revised wiring keeps the same electrical intent while moving the physical connection points to **CN3** and **CN15**.

## Revised NUCLEO header strategy

Use **CN15** for almost all logic signals.

Use **CN3** for power, ground, and the ESP32-C6 enable/reset line.

Do not use the following NUCLEO headers in this revised wiring:

* CN5
* CN13
* CN14

## Gateway STM32N6 to DWM3001C-CDK J10

### Shared SPI5 bus and control signals

|Function|Previous NUCLEO source|Revised NUCLEO source|STM32 pin|DWM3001C-CDK destination|
|-|-|-:|-|-|
|SPI SCK|CN14 D13|**CN15 pin 11**|PE15 / SPI5\_SCK|J10 pin 23, SPI1\_CLK|
|SPI MISO|CN14 D12|**CN15 pin 13**|PG1 / SPI5\_MISO|J10 pin 21, SPI1\_MISO|
|SPI MOSI|CN14 D11|**CN15 pin 15**|PG2 / SPI5\_MOSI|J10 pin 19, SPI1\_MOSI|
|DWM CS|CN14 D10|**CN15 pin 17**|PA3|J10 pin 24, CS\_RPI|
|DWM IRQ|CN15 pin 16|**CN15 pin 16**|PB9|J10 pin 15, GPIO\_RPI|
|DWM RESET|CN13 D2|**CN15 pin 33**|PD0|J10 pin 12, RESET|

### Power and ground

|Function|Previous NUCLEO source|Revised NUCLEO source|DWM3001C-CDK destination|
|-|-|-:|-|
|+5 V|CN5 pin 5|**CN3 pin 6** or **CN3 pin 18**|J10 pin 2 or J10 pin 4, 5V0|
|GND|Any NUCLEO GND|**CN3 pin 8**, **CN3 pin 20**, or **CN15 pin 20**|J10 pin 6, or another CDK GND|

Recommended physical pairing for first routing:

|Net|Preferred NUCLEO pin|Destination|
|-|-:|-|
|5V\_DWM|**CN3 pin 6**|DWM3001C-CDK J10 pin 2 or J10 pin 4|
|GND\_DWM|**CN3 pin 8**|DWM3001C-CDK J10 pin 6|

## Gateway STM32N6 to Seeed Studio XIAO ESP32-C6 DIP/header

### Shared SPI5 bus and control signals

|Function|Previous NUCLEO source|Revised NUCLEO source|STM32 pin|XIAO ESP32-C6 destination|
|-|-|-:|-|-|
|SPI CLK|CN14 D13|**CN15 pin 11**|PE15 / SPI5\_SCK|XIAO SCK|
|SPI MISO|CN14 D12|**CN15 pin 13**|PG1 / SPI5\_MISO|XIAO MISO|
|SPI MOSI|CN14 D11|**CN15 pin 15**|PG2 / SPI5\_MOSI|XIAO MOSI|
|C6 CS|CN14 D9|**CN15 pin 19**|PD7|XIAO CS|
|C6 INT / host-wake|CN14 D14|**CN15 pin 5**|PC1|XIAO INT|
|C6 EN / RST|CN3 pin 17|**CN3 pin 17**|PC0|XIAO EN/RST|

### Power and ground

|Function|Previous NUCLEO source|Revised NUCLEO source|XIAO ESP32-C6 destination|
|-|-|-:|-|
|3V3|CN5 pin 4|**CN3 pin 16**|XIAO 3V3|
|GND|Any NUCLEO GND|**CN3 pin 20** or **CN15 pin 20**|XIAO GND|

Recommended physical pairing for first routing:

|Net|Preferred NUCLEO pin|Destination|
|-|-:|-|
|3V3\_C6|**CN3 pin 16**|XIAO 3V3|
|GND\_C6|**CN3 pin 20**|XIAO GND|
|C6\_EN\_RST|**CN3 pin 17**|XIAO EN/RST|

For first bring-up, choose one XIAO power path. Do not power the XIAO simultaneously from its USB-C connector and the NUCLEO 3V3 rail unless that power interaction has been deliberately checked.

## Consolidated net list

|Net name|NUCLEO header pin|STM32 pin|Destination 1|Destination 2|
|-|-:|-|-|-|
|SPI5\_SCK|**CN15.11**|PE15|DWM J10.23 SPI1\_CLK|XIAO SCK|
|SPI5\_MISO|**CN15.13**|PG1|DWM J10.21 SPI1\_MISO|XIAO MISO|
|SPI5\_MOSI|**CN15.15**|PG2|DWM J10.19 SPI1\_MOSI|XIAO MOSI|
|DWM\_CS|**CN15.17**|PA3|DWM J10.24 CS\_RPI|-|
|DWM\_IRQ|**CN15.16**|PB9|DWM J10.15 GPIO\_RPI|-|
|DWM\_RESET|**CN15.33**|PD0|DWM J10.12 RESET|-|
|C6\_CS|**CN15.19**|PD7|-|XIAO CS|
|C6\_INT|**CN15.5**|PC1|-|XIAO INT|
|C6\_EN\_RST|**CN3.17**|PC0|-|XIAO EN/RST|
|5V\_DWM|**CN3.6**|5V rail|DWM J10.2 or J10.4|-|
|3V3\_C6|**CN3.16**|3V3 rail|-|XIAO 3V3|
|GND|**CN3.8 / CN3.20 / CN15.20**|GND|DWM GND|XIAO GND|

## Old-to-new migration table

|Remove this old connection|Replace with this CN3/CN15 connection|
|-|-|
|CN14 D13 / PE15 / SPI5\_SCK|**CN15 pin 11 / PE15 / SPI5\_SCK**|
|CN14 D12 / PG1 / SPI5\_MISO|**CN15 pin 13 / PG1 / SPI5\_MISO**|
|CN14 D11 / PG2 / SPI5\_MOSI|**CN15 pin 15 / PG2 / SPI5\_MOSI**|
|CN14 D10 / PA3 / DWM CS|**CN15 pin 17 / PA3 / DWM CS**|
|CN14 D9 / PD7 / ESP32-C6 CS|**CN15 pin 19 / PD7 / ESP32-C6 CS**|
|CN14 D14 / PC1 / ESP32-C6 INT|**CN15 pin 5 / PC1 / ESP32-C6 INT**|
|CN13 D2 / PD0 / DWM RESET|**CN15 pin 33 / PD0 / DWM RESET**|
|CN5 pin 5 / +5V|**CN3 pin 6 or CN3 pin 18 / 5V**|
|CN5 pin 4 / 3V3|**CN3 pin 16 / 3V3**|
|CN5/CN13/CN14 ground references|**CN3 pin 8, CN3 pin 20, or CN15 pin 20**|

## Revised one-page schematic view

```text
NUCLEO-N657X0-Q, using CN3 and CN15 only

CN15 signal header
------------------------------------------------------------

CN15.11  PE15 / SPI5\_SCK   ----+--> DWM3001C-CDK J10.23 SPI1\_CLK
                               +--> XIAO ESP32-C6 SCK

CN15.13  PG1 / SPI5\_MISO   <---+--  DWM3001C-CDK J10.21 SPI1\_MISO
                               +--  XIAO ESP32-C6 MISO

CN15.15  PG2 / SPI5\_MOSI   ----+--> DWM3001C-CDK J10.19 SPI1\_MOSI
                               +--> XIAO ESP32-C6 MOSI

CN15.17  PA3 / DWM\_CS      -------> DWM3001C-CDK J10.24 CS\_RPI
CN15.16  PB9 / DWM\_IRQ     <------- DWM3001C-CDK J10.15 GPIO\_RPI
CN15.33  PD0 / DWM\_RESET   -------> DWM3001C-CDK J10.12 RESET

CN15.19  PD7 / C6\_CS       -------> XIAO ESP32-C6 CS
CN15.5   PC1 / C6\_INT      <------- XIAO ESP32-C6 INT


CN3 power / auxiliary header
------------------------------------------------------------

CN3.6    5V                -------> DWM3001C-CDK J10.2 or J10.4
CN3.16   3V3               -------> XIAO ESP32-C6 3V3
CN3.17   PC0 / C6\_EN\_RST   -------> XIAO ESP32-C6 EN/RST
CN3.8    GND               -------> DWM3001C-CDK GND
CN3.20   GND               -------> XIAO ESP32-C6 GND
```

## Practical checks before routing

The shared SPI bus is valid if the **NUCLEO is the only SPI master** and both the DWM3001C-CDK and the XIAO ESP32-C6 behave as SPI peripherals.

Each peripheral must have its own chip-select line:

|Peripheral|Chip-select net|NUCLEO source|
|-|-|-:|
|DWM3001C-CDK|DWM\_CS|CN15 pin 17 / PA3|
|XIAO ESP32-C6|C6\_CS|CN15 pin 19 / PD7|

Consider weak pull-ups, around **10 kΩ to 3V3**, on both CS lines so neither peripheral is accidentally selected while the NUCLEO is resetting or before firmware configures the pins.

Keep **DWM\_RESET on PD0 via CN15 pin 33**, not on **CN3 pin 14 / NRST**. CN3 pin 14 is the NUCLEO reset net. Using it would couple the DWM reset to the NUCLEO's own reset behaviour rather than giving the STM32 firmware a clean independent reset output.

For the XIAO ESP32-C6, avoid ambiguous power states during bring-up. If programming the XIAO over USB-C while it is also connected to the NUCLEO carrier, verify whether the XIAO's 3V3 rail is being back-fed. If in doubt, isolate the carrier 3V3 connection during USB-only programming.

## Final routing intent

The routing plan is:

* **CN15** carries the shared SPI5 bus and all device control signals except XIAO enable/reset.
* **CN3** carries 5 V, 3.3 V, ground, and XIAO enable/reset.
* **CN5, CN13 and CN14** are no longer used for this Gateway wiring.

