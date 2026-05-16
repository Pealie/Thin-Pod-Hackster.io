# Thin-Pod: a small open-hardware vibration telemetry platform using UWB

## Short description

Thin-Pod is a small, reproducible vibration-sensing node for rotating machinery and mechanical structures. It combines an analogue MEMS vibration sensor, a custom carrier PCB, a Qorvo DWM3001C UWB development kit, and a gateway architecture based around an STM32 NUCLEO-N657X0-Q board. The project is designed as a practical open-hardware route into machinery-health sensing: measure vibration locally, transmit short diagnostic windows over UWB, and hand the data to a gateway for DSP, feature extraction and eventual TinyML.

The current rev 0.1 build has reached physical hardware bring-up. The manufactured Thin-Pod PCB has been assembled and tested far enough to prove the power path, PFET-switched accelerometer rail, ADXL1005 mid-rail output bias, and visible dynamic response to mechanical taps on an oscilloscope. The UWB development boards have also been brought up separately with working firmware, serial console access and successful two-way ranging between the Thin-Pod side and Gateway side.

## Project story

Thin-Pod began with a fairly simple question: could a small, understandable open-hardware node do useful vibration telemetry without becoming a black-box industrial product?

Industrial vibration monitoring is powerful, but much of the commercial ecosystem is expensive, closed, or designed around complete vendor stacks. Thin-Pod takes a different route. It is deliberately visible. The measurement chain can be followed from the accelerometer output, through a simple analogue conditioning network, into an ADC-capable UWB board, across a local UWB link, and onward to a gateway where the heavier signal processing can happen.

The project uses the Analog Devices ADXL1005 as the vibration sensor. Its analogue output is intentionally useful for learning because it does not hide the signal path behind a digital sensor interface. The board exposes the supply rails, filtered signal node, ADC node and power-control path as labelled test points. This makes Thin-Pod less like a sealed sensor puck and more like a small vibration instrument that can be inspected, modified and taught from.

The project also uses UWB in a slightly different way from many hobby builds. Rather than treating UWB only as an indoor-positioning technology, Thin-Pod treats it as a robust local transport layer for short vibration data windows. The design direction is not to stream everything forever. A better systems approach is to capture short, meaningful windows of vibration, attach metadata, and transmit selected frames to a gateway for analysis. That keeps the radio traffic realistic and leaves a clear path towards multi-pod systems.

## Relevance

A beginner can build a sensor that blinks an LED when something vibrates. Thin-Pod aims at the next level: a documented measurement chain that starts to resemble a real engineering instrument.

The project is useful because it sits at a fertile boundary between analogue electronics, embedded firmware, RF transport, mechanical measurement, DSP and open hardware. It is not trying to replace an HBK, Dewesoft, SKF or Bently Nevada system. It is a small, legible platform for learning how such systems begin: a sensor, a power supply, grounding discipline, signal conditioning, sampling, transport, and a gateway that can convert raw vibration into useful information.

This makes Thin-Pod suitable for education, experimentation and early research. It can support discussions about anti-alias filtering, ADC sampling, UWB packetisation, edge feature extraction, secure firmware update strategy, power budgeting, PCB manufacture, test-point planning and repeatable bring-up.

## System overview

The current system has two main hardware roles.

The first is the Thin-Pod sensor node. This is the board attached to, or placed near, a machine or test object. It contains the ADXL1005 vibration sensor path, local power regulation, reverse-polarity protection, a high-side PFET switch for the accelerometer supply, labelled test points and an interface to the Qorvo DWM3001C-CDK.

The second is the Gateway. This is the receiving and processing side. The Gateway architecture uses a Qorvo DWM3001C-CDK for UWB reception and an STM32 NUCLEO-N657X0-Q as the main MCU platform for future DSP, feature extraction and TinyML experimentation. A Seeed Studio XIAO ESP32-C6 has also been included in the Gateway carrier design as a possible Wi-Fi backhaul or network interface.

The intended data path is:

```text
Mechanical vibration
        ↓
ADXL1005 analogue output
        ↓
RC filtering and ADC input node
        ↓
DWM3001C-CDK ADC-capable input on the Thin-Pod side
        ↓
UWB packet/window transport
        ↓
Gateway DWM3001C-CDK
        ↓
STM32 NUCLEO-N657X0-Q
        ↓
DSP, feature extraction, logging and future TinyML
```

## What was built

The first stage was a controlled bench system.

The DWM3001C-CDK boards were flashed, their console behaviour was brought under control, and a known-good UWB baseline was established. One board was configured on the Thin-Pod side and the other on the Gateway side. The Thin-Pod side produced continuous listener reports, and the Gateway side produced repeated successful ranging notifications. That closed the first major uncertainty: the radio link and firmware baseline worked before the analogue sensor path was added.

The next stage was the ADXL1005 signal path. The sensor output was measured at rest at approximately mid-rail, around 1.7 V, and mechanical taps produced visible transient behaviour. Early scope readings also exposed an important lesson: not every waveform seen on a handheld scope is real signal. Probe-loop pickup and measurement settings produced misleading traces until the grounding and coupling were made more disciplined.

The breadboard build then became a perfboard prototype. This was an important step because the project had outgrown loose Dupont wiring. The perfboard version brought together the DWM3001C-CDK, the Pololu S7V8F3 regulator and the ADXL1005 breakout on a rigid carrier. It reduced wiring variability and made the later PCB constraints easier to reason about.

From there, the design moved into KiCad. The Thin-Pod carrier PCB was designed around these main blocks:

```text
RAW\_IN battery or bench input
        ↓
1N5817 reverse-polarity protection
        ↓
RAW\_PROT protected input rail
        ↓
Pololu S7V8F3 regulator
        ↓
3V3+ regulated rail
        ↓
PFET-switched accelerometer rail
        ↓
ADXL1005 supply and analogue output path
        ↓
RC filter and ADC/test point path
        ↓
DWM3001C-CDK interface
```

The board includes labelled test points for the major rails and signal nodes. They are central to the project philosophy as every important stage should be measurable.

## Thin-Pod rev 0.1 hardware

Thin-Pod rev 0.1 is a hand-assembly-friendly carrier board. It was designed to make the DWM3001C-CDK, ADXL1005 sensor board and regulator behave as one coherent prototype rather than a collection of loose modules.

The key hardware blocks are:

|Block|Purpose|
|-|-|
|JST-PH 2-pin power input|Battery or bench-supply entry|
|1N5817 Schottky diode|Series reverse-polarity protection|
|Pololu S7V8F3|3.3 V step-up/step-down regulation|
|ZVP2106A PFET switch|High-side switching of the accelerometer supply|
|ADXL1005 / EVAL-ADXL1005Z|Analogue vibration sensing|
|8 kΩ / 680 pF RC network|Simple low-pass conditioning before the ADC node|
|1 MΩ pull-down|Defines the ADC node when the sensor path is inactive|
|100 nF and 10 µF capacitors|Local accelerometer supply decoupling|
|DWM3001C-CDK interface|UWB compute/radio board connection|
|Labelled test points|Bring-up, debugging and teaching access|
|Mounting holes and spacers|Mechanical stability during bench testing|

The first manufactured PCB arrived and passed initial visual inspection. The outline, rounded corners, solder mask, silkscreen, drill holes, mounting holes, test points and main component footprints were all broadly consistent with the design intent.

During bring-up, the board initially appeared to supply approximately 3.37 V to the CDK power pads, but the CDK did not power up. The fault was traced to a missing or ineffective ground return between the carrier board and the mounted CDK. A short soldered jumper from the Thin-Pod ground path to a confirmed CDK ground loop corrected the rev 0.1 test configuration. With that jumper fitted, the CDK powered from the Thin-Pod battery input and the red LED flashed as expected.

The PFET-switched accelerometer rail then measured correctly:

```text
PFET source:    approximately 3.36 V
PFET gate:      0 V
PFET drain:     approximately 3.35 V
TP6 /ACC\_3V3\_SW approximately 3.35 V
TP7 filtered sensor output approximately 1.76 V
```

An oscilloscope connected to TP7 showed clear transient bursts when the board was tapped. One capture showed approximately 539 mV peak-to-peak during tapping, with visible damped ringing around the baseline. That is the first strong rev 0.1 evidence that the manufactured board is not merely powered, but actually responding to vibration through the analogue sensor path.

## Gateway rev 0.1 hardware

The Gateway is the receiving and processing side of Thin-Pod. It is intended to host the more computationally expensive parts of the system: packet reception, reassembly, DSP, feature extraction, logging, communications and future TinyML experiments.

The Gateway rev 0.1 carrier design brings together:

|Block|Purpose|
|-|-|
|STM32 NUCLEO-N657X0-Q|Main gateway MCU and future DSP/TinyML host|
|Qorvo DWM3001C-CDK|UWB radio interface|
|Seeed Studio XIAO ESP32-C6|Optional Wi-Fi or network backhaul|
|Shared SPI5 bus|Data/control connection from the STM32 side|
|Separate chip-select lines|Avoids peripheral contention|
|10 kΩ pull-up resistors|Keeps SPI devices deselected during reset|
|Mounting holes|Mechanical stability|
|Project-local KiCad symbols and footprints|Reproducible design files|

The Gateway design was deliberately routed through the ST morpho headers rather than mixing Arduino/Zio and morpho wiring. This made the design cleaner and easier to document. The PCB reached a clean KiCad ERC/DRC state, Gerbers and Excellon drill files were generated, and the PTH/NPTH distinction was checked in the fabrication outputs.

## Build process

### 1\. Establish a known-good UWB baseline

Before committing to the vibration path, the DWM3001C-CDK boards were flashed and tested. The goal was to prove that firmware loading, serial console access and UWB exchange worked independently of the sensor hardware.

The important outcome was a stable two-node UWB setup. The Thin-Pod-side board could receive frames in listener mode, and the Gateway-side board produced successful ranging notifications. This meant later failures could be isolated to the sensor, power or ADC path rather than immediately blaming the radio link.

### 2\. Prove the analogue sensor path

The ADXL1005 output was checked with a DVM and oscilloscope. At rest, the output sat near mid-rail. Tapping the board created transient activity. Several early measurements were misleading, especially where probe pickup appeared as low-frequency ripple. The measurement method was tightened until the sensor output could be distinguished from scope artefacts.

This stage was important because it created the project’s central test philosophy: always separate rail integrity, sensor bias, real mechanical response and measurement-tool artefact.

### 3\. Replace loose wiring with a rigid prototype

The project then moved from breadboard wiring to a perfboard prototype. This reduced intermittent wiring behaviour and made the physical layout easier to understand. It also helped define which connections needed named nets, test points and mechanical support on the PCB.

### 4\. Translate the bench system into KiCad

The KiCad design captured the power path, protection diode, regulator, PFET switch, accelerometer supply rail, analogue filter, ADC node and DWM3001C-CDK interface. A project-local library structure was used so the symbols, footprints and 3D model assets could travel with the project.

Several design issues were corrected before fabrication: connector footprint choices, component polarity, ADXL1005 pin mapping, test-point placement, capacitor footprint selection, net names, mounting holes and DRC/footprint warnings.

### 5\. Generate and inspect fabrication files

Before ordering the PCB, the final manufacturing workflow was:

```text
Refill copper zones
Save the project
Run ERC
Run DRC
Plot Gerbers
Generate Excellon drill files
Open the files in KiCad Gerber Viewer
Check F.Cu and B.Cu
Check F.Mask and B.Mask
Check F.Silkscreen
Check Edge.Cuts
Check PTH and NPTH drill layers
Zip the fabrication outputs
Upload to manufacturer preview
Inspect the manufacturer preview
Place the order
```

This was one of the most important practical achievements of the project. Thin-Pod became not only a circuit, but a reproducible manufacturing package.

### 6\. Bring up the manufactured board

After the board arrived, visual inspection came first. Component assembly and electrical testing were staged rather than rushed.

The first major issue was the CDK ground return. This was a useful discovery rather than a failure of the project. The board made the problem visible. The fault was narrowed by continuity testing, control testing with a spare CDK, and measurement against known ground points. A soldered jumper then created a stable rev 0.1 bring-up configuration.

With that corrected, the accelerometer supply, filtered output and dynamic vibration response were measured successfully.

## Current results

Thin-Pod rev 0.1 has achieved these milestones:

|Stage|Result|
|-|-|
|DWM3001C-CDK firmware flashing|Complete|
|Serial console and CLI access|Complete|
|Two-node UWB ranging baseline|Complete|
|ADXL1005 DC output sanity check|Complete|
|Breadboard-to-perfboard stabilisation|Complete|
|KiCad carrier PCB design|Complete|
|ERC/DRC clean state before fabrication|Complete|
|Gerber and drill generation|Complete|
|Manufacturer preview and PCB order|Complete|
|Thin-Pod rev 0.1 PCB received|Complete|
|Initial visual inspection|Complete|
|Regulated 3.3 V path|Working|
|PFET-switched accelerometer rail|Working|
|ADXL1005 mid-rail filtered output|Working|
|Tap-induced vibration response on scope|Working|
|CDK J1 ground return|Rev 0.1 issue found and patched|
|ADC sampling and UWB vibration-payload transport|Next integration step|

Thin-Pod is not yet a polished commercial sensor, and is a working open-hardware prototype at the point where the electrical measurement chain is alive, the board has been manufactured, and the next work is firmware integration and cleaner rev 0.2 hardware.

## How to recreate the project

### Hardware build outline

1. Prepare two Qorvo DWM3001C-CDK boards, one for the Thin-Pod side and one for the Gateway side.
2. Flash known-good SDK firmware and confirm serial console access before connecting the sensor chain.
3. Confirm UWB reception/ranging between the two boards.
4. Assemble the Thin-Pod carrier board in stages: power input, reverse-protection diode, regulator, PFET switch, accelerometer supply, analogue filter and CDK interface.
5. Before attaching modules, check continuity and shorts on RAW\_IN, RAW\_PROT, 3V3+, ACC\_3V3\_SW, ACC\_VOUT, ADC\_NODE and GND.
6. Power the board from a current-limited bench supply and confirm 3.3 V at the regulator output.
7. Confirm the PFET-switched accelerometer rail.
8. Confirm the ADXL1005 output bias at the filtered test point.
9. Use an oscilloscope on the filtered output and apply light mechanical taps to confirm dynamic response.
10. Only after the analogue path is proven, begin ADC sampling and UWB payload integration.

### Firmware and software outline

The current software work is staged rather than monolithic.

The first layer uses the Qorvo SDK examples to prove flashing, console access and UWB operation. This avoids debugging sensor code and RF code at the same time.

The second layer is the Thin-Pod ADC sampler. The intended ADC input is the DWM3001C-CDK path connected to the filtered ADXL1005 signal. The goal is to capture short raw vibration windows, not continuous unbounded streaming.

The third layer is UWB payload transport. Synthetic payloads should be transported first, followed by real ADC windows.

The fourth layer is the Gateway handoff. The STM32 NUCLEO-N657X0-Q reads or receives the vibration data, computes simple features and later provides a place for TinyML experiments.

Recommended first gateway features are deliberately modest:

```text
RMS
Peak
Peak-to-peak
Crest factor
Simple band-energy estimates
Event timestamp
Battery or rail status
Packet count and dropped-frame count
```

TinyML should come later, after the measurement chain is repeatable. The correct sequence is: first a vibration instrument, then an edge analytics instrument, then a TinyML product.

## Bill of materials

|Item|Quantity|Notes|
|-|-:|-|
|Custom Thin-Pod rev 0.1 PCB|1|KiCad-designed carrier board|
|Qorvo DWM3001C-CDK|2|One Thin-Pod side, one Gateway side|
|Analog Devices EVAL-ADXL1005Z or ADXL1005 breakout|1|Analogue vibration sensor|
|Pololu S7V8F3 3.3 V step-up/step-down regulator|1|Main 3.3 V rail for prototype|
|STM32 NUCLEO-N657X0-Q|1|Gateway MCU platform|
|Seeed Studio XIAO ESP32-C6|1|Optional Gateway network/backhaul module|
|1N5817 Schottky diode|1|Series reverse-polarity protection|
|ZVP2106A P-channel MOSFET|1|High-side accelerometer supply switch|
|JST-PH 2-pin connector|1|Battery or bench power input|
|8 kΩ resistor|1|ADC input series resistor|
|680 pF capacitor|1|ADC node low-pass capacitor|
|1 MΩ resistor|1|ADC node pull-down|
|100 nF capacitor|1|Local accelerometer decoupling|
|10 µF capacitor|1|Local accelerometer bulk decoupling|
|10 kΩ resistors|2|Gateway SPI chip-select pull-ups|
|Pin headers and sockets|As required|CDK, sensor and module interfaces|
|Test points|As required|Rails and signal nodes|
|M2.5 hardware/standoffs|As required|Mechanical mounting|
|Current-limited bench supply|1|Bring-up and fault isolation|
|DVM/multimeter|1|Continuity and voltage checks|
|Oscilloscope|1|Sensor output and transient checks|
|KiCad|1|Schematic, PCB and manufacturing files|
|SEGGER J-Link tools / Qorvo tooling|1|DWM3001C-CDK firmware flashing|
|Serial terminal|1|CLI and bring-up logging|

## What makes Thin-Pod creative

Thin-Pod is not creative because vibration sensing is new. It is creative because it turns vibration telemetry into a small, inspectable, open and teachable system.

Most hobby projects either stop at 'sensor reads value' or jump too quickly to 'AI predicts fault'. Thin-Pod deliberately fills the space between those extremes. It treats the analogue front end, power path, grounding, ADC node, radio payload and gateway as first-class engineering objects. That makes the project more credible and more useful.

The UWB angle is also a fresh take. UWB is commonly associated with ranging and localisation. Thin-Pod uses that same local RF ecosystem as part of a machinery-health telemetry architecture, where short diagnostic windows can be sent from multiple pods to a local gateway.

The project is also creative in its documentation. The engineering logs do not hide the false starts. They show how the board was debugged: measurement artefacts, power-path confusion, footprint assumptions, grounding issues and rev 0.2 corrections. That is valuable for beginners because it shows the real shape of hardware engineering, not only the polished final photograph.

## Rev 0.1 lessons and rev 0.2 corrections

The most important rev 0.1 lesson is that the carrier-to-CDK ground return must be explicit and verified. The J1 negative pad alone should not be treated as sufficient unless it is proven to bond reliably to the CDK ground plane. Rev 0.2 should add a robust confirmed CDK ground connection, either through a verified J10 GND pin, a confirmed ground loop, or a better documented mechanical/electrical interface.

The ADXL1005 control pins also need defined inactive states. STANDBY should be pulled low to keep the sensor in measurement mode unless MCU duty-cycling is deliberately implemented. ST/self-test should also be pulled low, with a test pad or MCU-controlled option for deliberate self-test.

The physical placement can also improve. The regulator and local sensor conditioning components should be reviewed so that the sensor supply and analogue path are shorter, cleaner and easier to probe. Rev 0.1 is good enough for bring-up; rev 0.2 should make the discovered assumptions explicit in copper.

## Competition fit

Thin-Pod fits the Best of 2025 Competition as an MCU-centred embedded systems project. It uses development boards in a real hardware application, documents the build path, includes a custom PCB, exposes a complete BOM, and has a clear contribution beyond a one-off demo.

Against the judging rubric:

|Criterion|How Thin-Pod addresses it|
|-|-|
|Project Documentation|Engineering logs, staged bring-up, PCB screenshots, photographs, measurements and demo plan|
|Complete BOM|Sensor, UWB boards, regulator, passives, connectors, tools and software listed|
|Schematics|KiCad schematic, PCB, Gerbers and test-point map available|
|Code \& Contribution|Qorvo bring-up baseline, planned ADC sampler, Gateway SPI/DSP structure, documented firmware staging|
|Creativity|UWB used as local vibration telemetry, open instrumentation approach, gateway DSP/TinyML path|

## Closing summary

Thin-Pod rev 0.1 is now a real manufactured object with measured evidence behind it. The board has powered up, the accelerometer rail works, the filtered ADXL1005 output sits at the expected bias, and mechanical taps produce a clear dynamic response on the oscilloscope. The UWB link has already been brought up separately, giving the project a credible path from sensor node to gateway.

The next step is firmware integration: sample the filtered accelerometer output, packetise short vibration windows, transport them across UWB and hand them to the STM32 gateway for feature extraction. That is where Thin-Pod becomes a small open vibration telemetry platform: visible, measurable, reproducible and ready to grow.

