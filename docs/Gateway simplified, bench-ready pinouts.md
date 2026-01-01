Below is a simplified, bench-ready pinout for the Gateway (NUCLEO-N657X0-Q) to (1) the DWM3001C-CDK and (2) the Seeed ESP32-C6, with both peripherals sharing the same SPI bus and using separate chip-select lines.

The earlier pinout note about header naming still stands: D13/D12/D11/D10 live on **CN14** (not CN13). Reset for the CDK stays on **CN13 D2**.

Gateway STM32N6 ↔ DWM3001C-CDK (J10)

SPI bus: **SPI5 on CN14**

* SCK: CN14 **D13** (PE15 / SPI5\_SCK) → CDK J10 **23** (SPI1\_CLK)
* MISO: CN14 **D12** (PG1 / SPI5\_MISO) → CDK J10 **21** (SPI1\_MISO)
* MOSI: CN14 **D11** (PG2 / SPI5\_MOSI) → CDK J10 **19** (SPI1\_MOSI)

Control

* CS: CN14 **D10** (PA3) → CDK J10 **24** (CS\_RPI)
* IRQ: **CN15 pin 16 = PB9** → CDK J10 **15** (GPIO\_RPI)
* RESET: CN13 **D2 = PD0** → CDK J10 **12** (RESET)

Power

* +5 V: **CN5 pin 5 = +5V** → CDK J10 **2 or 4** (5V0)
* GND: any NUCLEO GND → CDK J10 **6** (also 9/14/20/25 are GND options)

Gateway STM32N6 ↔ Seeed ESP32-C6 (shared SPI5 bus)

SPI bus: **same SPI5 on CN14**

* CLK: CN14 **D13 (PE15)** → ESP32-C6 CLK
* MISO: CN14 **D12 (PG1)** ← ESP32-C6 MISO
* MOSI: CN14 **D11 (PG2)** → ESP32-C6 MOSI

Control

* CS (unique): CN14 **D9 (PD7)** → ESP32-C6 CS
* INT (host-wake): CN14 **D14 (PC1)** → ESP32-C6 INT
* RST/EN: **CN3 pin 17 = PC0** → ESP32-C6 EN/RST

Power

* 3V3: **CN5 pin 4 (3V3)** (or the PoE 3V3 rail) → ESP32-C6 3V3
* GND: any NUCLEO GND → ESP32-C6 GND

One-page ‘schematic’ view

NUCLEO top edge header (CN14):
CN14  \[D15] \[D14] \[D13] \[D12] \[D11] \[D10] \[D9] \[D8]
INT   SCK   MISO  MOSI   CS1   CS2

* CS1 (D10) = DWM3001C-CDK
* CS2 (D9)  = ESP32-C6

Shared-bus wiring sketch:



STM32N6 (SPI5)

&nbsp; SCK  ─────────┬────────> CDK J10-23

&nbsp;               └────────> ESP32 CLK

&nbsp; MOSI ─────────┬────────> CDK J10-19

&nbsp;               └────────> ESP32 MOSI

&nbsp; MISO <────────┬────────  CDK J10-21

&nbsp;               └────────  ESP32 MISO



&nbsp; CS (CDK)  ─────────────> CDK J10-24

&nbsp; IRQ (CDK) ─────────────> CDK J10-15

&nbsp; RST (CDK) ─────────────> CDK J10-12



&nbsp; CS (C6)   ─────────────> ESP32 CS

&nbsp; INT (C6)  ─────────────> ESP32 INT

&nbsp; EN (C6)   ─────────────> ESP32 EN/RST

Notes:

Relevance of the older ‘v2.1 schematic’ text

That sketch remains useful as a functional block diagram, but it shows a different GPIO set for a ribbon-cable SPI link (PB3/PB4/PB5/PC2 plus PB9). That looks like a custom-PCB mapping (SPI3-style) rather than the header-friendly NUCLEO bench wiring above (SPI5 on CN14).

If the Gateway PCB is moving away from the NUCLEO headers, the SPI3-style mapping can be revived later. For the simplified ‘make it work on the bench’ phase, the CN14 SPI5 map is the clean baseline.

