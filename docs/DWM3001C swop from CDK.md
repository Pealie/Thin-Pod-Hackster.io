According to a document from Oct 2025 (DWM3001C Data Sheet), the DWM3001C is programmed and debugged over SWD, not over the CDK’s on-board J-Link USB path, so the carrier needs to break out SWD (plus power, ground, and reset) to an external SWD probe. 

Programming and debug pinout for DWM3001C (minimum set)
These are the pins that matter for first programming and bring-up:

* VDD (3 V supply): pin 12 (connect to the existing 3v3+ rail from the Pololu S7V8F3) 
* GND: pins 1, 11, 21, 38, 48 (tie all to the Thin-Pod ground plane) 
* SWD_CLK: pin 2 
* SWD_DIO: pin 3 
* RESET (active-low): pin 47 

A practical through-hole SWD header
A simple 0.1 inch through-hole header is enough. The common 5-wire mapping is:

1. VREF (sense) → 3v3+ (VDD)
2. GND → GND
3. SWDIO → SWD_DIO (pin 3)
4. SWCLK → SWD_CLK (pin 2)
5. nRESET → RESET (pin 47)

That matches the Thin-Pod pin budget already captured in the bring-up checklist. 

What the CDK’s USB was doing, and what replaces it on a module-only carrier
According to the CDK product brief, the DWM3001CDK has two micro-USB ports: one to the on-board J-Link debugger (which provides SWD/UART), and one that connects directly to the DWM3001C USB interface. 

* On the CDK, ‘programming over USB’ is effectively: PC → USB → on-board J-Link → SWD into the nRF52833.
* On a bare DWM3001C module, that on-board J-Link does not exist, so programming is: PC → external SWD probe (J-Link, STLink with SWD support, etc) → SWD header → module SWD pins.

The Quick Start Guide also makes this explicit by instructing selection of the nRF52833 target and ‘SWD Interface’ in JFlashLite. 

If a direct USB connection to the DWM3001C is still wanted (for runtime comms, logging, DFU, or lab convenience), the module exposes USB differential pins:

* USB_N: pin 19
* USB_P: pin 20 

That is separate from SWD programming. SWD remains the reliable ‘first flash / recovery’ path.

Additional supporting components and wiring for DWM3001C on Thin-Pod
No external RF matching network, crystal, or DW3110 support parts are required on the carrier, since those functions are integrated on the module. The carrier work is mainly: clean 3v3 power delivery, reset discipline, SWD access, and any chosen I/O breakouts. (The module does draw fast-ish current in TX/RX, so local decoupling still matters.) 

1. Power decoupling (through-hole)

* Place local decoupling at the module VDD feed:

  * 100 nF ceramic (through-hole) from VDD to GND, leads kept very short
  * 4.7 µF to 10 µF bulk (through-hole electrolytic or tantalum) from VDD to GND
* If the 3v3+ rail is shared with other switching loads, a ferrite bead or small series impedance in the VDD feed can help, but it is not a module requirement, it is a system-noise choice.

2. Reset network (through-hole, recommended for deterministic bring-up)
   RESET is an active-low input on pin 47. 
   A simple, robust pattern:

* 10 kΩ pull-up from RESET to 3v3+
* 100 nF from RESET to GND (optional, for a little POR stretch)
* momentary reset button from RESET to GND (optional but very useful on the bench)

This matches the ‘optional pull-up and reset button’ approach in the Thin-Pod bring-up checklist. 

3. Keep the existing Thin-Pod signal mapping (ADXL1005 path unchanged)
   From the DWM3001C pin diagram, the prior CDK header mapping ports cleanly onto the module:

* ADC_NODE → P0.28 (pin 33) 
* CS_RPI / PFET_GATE (as already used) → P0.30 (pin 43) 

The checklist already records these exact pins as the candidate ADC input and control line for Thin-Pod. 

4. Optional UART break-out (strongly helpful for firmware bring-up)
   A 3-pin through-hole header for a 3.3 V USB-UART dongle is typically enough:

* GND
* UART_TX from module
* UART_RX into module

The checklist proposes P0.06 (pin 24) and P0.07 (pin 40) as a clean default pairing, with an optional small series resistor close to the module if wiring is long/noisy. 

5. Optional recovery straps (only if the firmware design wants them)
   For field recovery modes (safe-mode, DFU path, verbose boot), the checklist proposes strap pins sampled at boot, with weak pull-ups or pull-downs and optional buttons/test pads. That is firmware-defined rather than a Qorvo hardware requirement, but it is a practical replacement for the CDK’s on-board buttons and test points. 

Mechanical and layout constraint that changes relative to a CDK
The module antenna keep-out becomes the carrier’s responsibility. The datasheet notes a minimum spacing of 10 mm with no metal either side of the module antenna area, and shows approaches such as removing copper around/behind the antenna or letting the antenna overhang the PCB edge. 

In other words: the CDK already ‘got this right’, but a Thin-Pod carrier needs to implement that same keep-out discipline.

The quickest schematic-level change from the screenshot is: replace the CDK symbol block with a DWM3001C symbol that brings out VDD, the five GND pins, SWD_CLK, SWD_DIO, RESET, P0.28, and P0.30, plus optional USB_P/USB_N and UART pins, then add the through-hole SWD header and reset parts around it.
