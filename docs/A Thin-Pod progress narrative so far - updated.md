# Thin-Pod progress narrative for Hackster.io Best of 2025

Thin-Pod is a compact, UWB-linked vibration sensing system for rotating machinery monitoring. The project began as a question about whether a small vibration node could capture meaningful analogue acceleration data, packetise it over UWB, and pass it into a more capable gateway for DSP and TinyML. It has since become a staged engineering build with repeatable firmware bring-up, a validated analogue sensing path, project-local KiCad assets, and two board workstreams moving towards physical prototype hardware.

The current Hackster scope is deliberately focused: vibration sensing, UWB telemetry, gateway ingest, and edge analytics. Energy harvesting is not part of this entry. That boundary has helped keep the build testable and evidence-led.

Work to date now spans **23 December 2025 to 4 May 2026**. The central achievement is that Thin-Pod has moved from loose bench wiring and development-kit experiments into manufacturable PCB packages, with a clear route from sensor signal to Gateway-side analytics.

\---

## Current status at a glance

|Area|Status|Evidence so far|
|-|-|-|
|UWB firmware and console bring-up|Proven|DWM3001C-CDK boards flashed, CLI/RTT output confirmed, stable known-good firmware baseline established.|
|UWB link and ranging|Proven|One board receives frames while the other repeatedly reports successful ranging notifications.|
|Analogue sensor path|Bench-proven, firmware capture next|ADXL1005 mid-rail output confirmed, tap-correlated transients observed, ADC input mapped to J10 pin 15 / P0.28 / SAADC AIN4.|
|Thin-Pod node PCB|Ordered|Schematic ERC and PCB DRC clean, Gerbers and drill files generated and inspected, first Thin-Pod rev 0.1 PCB ordered from a UK manufacturer.|
|Gateway rev 0.1 PCB|Pre-order fabrication candidate|NUCLEO-N657X0-Q, DWM3001C-CDK and XIAO ESP32-C6 carrier design reached clean ERC/DRC, Gerbers generated, PTH/NPTH split checked, fabrication ZIP uploaded to a UK manufacturer.|
|DSP and TinyML path|Architected, not yet fully implemented|Gateway is the intended home for heavier DSP and inference; first node-side feature and window-capture plan is defined.|

The project is now less a speculative concept and more a staged hardware system: sensor node, UWB transport, gateway carrier, and analytics layer are each being treated as separately provable engineering blocks.

\---

## The validation ladder

Thin-Pod has been organised around a four-stage validation ladder. This has prevented the project from jumping too quickly into machine learning before the sensor, power, radio and interconnect layers are trustworthy.

### Stage 1: tooling, flashing, console, known-good firmware

The first milestone was to make the Qorvo DWM3001C-CDK boards boringly repeatable. J-Link flashing was stabilised, FreeRTOS SDK images were programmed successfully, and console access was brought under control. Early serial confusion, including terminal echo and spurious command behaviour, was traced to terminal setup rather than firmware instability. Device identity and SDK build information were confirmed through RTT and CLI output.

This matters because every later stage depends on the same basic discipline: known firmware, known console behaviour, known reset path, and a way to tell whether a failure is in the application, the wiring, the RF link, or the test setup.

### Stage 2: UWB link bring-up and ranging

The two-board UWB link then reached a known-good state. One board, acting as the Thin-Pod side, continuously reported received frames. The other, acting as the Gateway side, repeatedly produced successful ranging notifications with live distance values.

The project also captured an important bench lesson: UWB behaviour was sensitive to cable placement, cable length and local physical arrangement. Rather than treating that as noise to ignore, it has been recorded as a controlled variable for future RF testing. That is useful for Hackster documentation because it shows not just a success case, but also the physical realities of a bench UWB setup.

### Stage 3: analogue sensor path and ADC boundary

The analogue path has moved from uncertain wiring to a defined signal chain.

The sensor is the ADXL1005 analogue accelerometer. Its output was measured around mid-rail at rest, approximately 1.7 V to 1.8 V in the 3.3 V system, which is plausible for a biased analogue accelerometer output. Tap-correlated movement was initially hard to see because the handheld scope setup was dominated by pickup, scaling and trigger issues. Once probe technique, coupling and vertical sensitivity were improved, transient response became visible on the sensor node.

The measurement boundary is now explicit:

```text
ADXL1005 VOUT -> 8 kΩ series resistor -> ADC\_NODE -> DWM3001C-CDK J10 pin 15
                                  |
                                680 pF
                                  |
                                 GND

ADC\_NODE also has a 1 MΩ pull-down to GND.
```

On the DWM3001C-CDK, J10 pin 15 maps to module pin P0.28, which corresponds to nRF52 SAADC AIN4. The planned firmware configuration is conservative and practical: internal 0.6 V reference, gain 1/6 for 3.3 V-range input, 12-bit resolution, offset calibration, and a longer acquisition time suitable for the 8 kΩ source impedance. Timer and PPI-triggered sampling into double-buffered windows remains the next firmware proof point.

The important lesson from this phase was that the ADC front end must be validated electrically before raw samples are trusted. A previously confusing loading issue involving the 680 pF capacitor was isolated during diagnosis, then the RC network was reintroduced deliberately as a controlled conditioning stage rather than as an accidental load.

### Stage 4: end-to-end stream into Gateway DSP and TinyML

The full data path has been planned but not yet fully closed. The intended staged proof is:

1. Confirm analogue sanity and SAADC sampling on known input levels.
2. Capture timed ADC windows on the Thin-Pod node.
3. Transport synthetic payloads over UWB and prove packet integrity.
4. Replace synthetic payloads with real ADC windows and compare statistics before and after transport.
5. Extract received frames from the Gateway DWM3001C-CDK into the STM32 NUCLEO-N657X0-Q over SPI.
6. Run DSP feature extraction and then TinyML classification on the Gateway side.

A key decision was that an oscilloscope can confirm SPI signalling, but not reliable data extraction. For the final hop, a real SPI master is needed. The planned Gateway validation tool is therefore a minimal STM32 SPI5 reader that deselects the ESP32-C6, listens for the DWM3001C-CDK data-ready interrupt, clocks out fixed-size frames, computes CRC32, and reports concise diagnostics over SWV, ITM or UART.

\---

## Hardware narrative: from bench wiring to PCB fabrication

### 1\. The bench prototype established the system boundaries

The early Thin-Pod bench setup connected the ADXL1005, the DWM3001C-CDK, a Pololu S7V8F3 regulator and supporting wiring on a perfboard-style platform. This was not yet elegant hardware, but it was invaluable because it exposed the real integration problems: power entry, USB backfeed assumptions, sensor standby behaviour, ADC loading, wiring pickup, and the physical awkwardness of relying on flying leads.

Several design decisions came from this phase:

* The DWM3001C-CDK should not be powered from J10 pins that are NC on the board.
* The CDK can be powered through its bottom battery or external supply pads during bench work.
* The ADXL1005 standby pin needs a defined default state.
* A 3.3 V rail should feed both the CDK and the sensor system through a controlled, testable power path.
* The accelerometer output must have accessible test points before and after the RC network.

The bench prototype therefore became the evidence base for the PCB rather than a throwaway lash-up.

### 2\. The Thin-Pod node carrier became a manufacturable board

The first PCB workstream focused on the Thin-Pod node carrier. The goal was to harden the sensor, power and UWB node wiring into a reproducible board.

The schematic and PCB converged around these named nets and functions:

|Net or block|Function|
|-|-|
|`RAW\_IN`|Battery or bench input before protection.|
|`RAW\_FUSED`|Rail after the fuse holder.|
|`RAW\_PROT`|Protected input rail after the 1N5817 Schottky diode.|
|`3V3+`|Regulator output rail from the S7V8F3.|
|`ACC\_3V3\_SW`|PFET-switched accelerometer supply rail.|
|`PFET\_GATE` / `PFET\_CTRL`|Firmware-controlled power-gating network for the accelerometer supply.|
|`ACC\_VOUT`|Raw ADXL1005 analogue output.|
|`ADC\_NODE`|Filtered ADC input node after the 8 kΩ / 680 pF network.|

The board also gained a practical bring-up test-point set:

|Test point|Purpose|
|-|-|
|TP1|Raw input voltage before fuse/protection.|
|TP2|Fuse output / diode input side.|
|TP3|Protected rail after the reverse-polarity diode.|
|TP4|3.3 V regulator output.|
|TP5|Ground reference.|
|TP6|Raw ADXL1005 output before filtering.|
|TP7|Filtered ADC node at the DWM3001 ADC boundary.|

That test-point strategy is one of the most important project improvements. It turns first power-up from guesswork into a measured sequence.

### 3\. KiCad cleanup turned the node PCB into a real fabrication package

The node PCB went through the usual first-board grind: missing or mismatched footprints, ratsnest confusion, connector pad issues, ground-pour behaviour, symbol/library mismatches, and silkscreen/clearance warnings. These were not treated as cosmetic annoyances. They were worked through until the design had a clean schematic and PCB state.

Notable fixes included:

* Correcting the JST-PH power connector so it had two real electrical pads: `RAW\_IN` and `GND`.
* Resolving the DWM3001C-CDK symbol-to-footprint mapping so ratsnest connectivity persisted after PCB updates.
* Adding and refilling a B.Cu ground zone assigned to the correct KiCad net name.
* Routing the sensitive analogue nets and the switched accelerometer supply.
* Confirming D1 polarity so the 1N5817 cathode faces the protected regulator input side.
* Correcting the ADXL1005 symbol pin mapping so VOUT, GND and VDD landed on the intended footprint pads.
* Assigning a project-local ZVP2106A PFET footprint with meaningful gate, source and drain mapping.
* Confirming that the fuse copper did not bypass the fuse holder.
* Changing C3 to a non-polar 10 µF capacitor footprint for local bulk decoupling on `ACC\_3V3\_SW`.
* Using a right-angle JST-PH connector so the power cable exits sensibly from the board edge.
* Adding mounting-hole keep-outs for screw or washer clearance.

By 29 April 2026, the Thin-Pod rev 0.1 board had clean ERC, clean DRC, filled zones, zero unconnected pads, generated Gerbers, generated drill files, a checked manufacturer preview and an active UK PCB fabrication order. That is a major transition: the node carrier has crossed from design intent into physical manufacturing.

\---

## DWM3001C module migration and library work

Alongside the CDK-based node carrier, a second thread investigated migration from the DWM3001C-CDK to the bare DWM3001C castellated module. This is not the immediate lowest-risk path for the first prototype, but it matters for future size, repeatability and product shape.

Work on this thread produced:

* A KiCad-compatible DWM3001C module footprint with 48 pads.
* Corrected pin numbering and orientation, including the ‘flipy’ footprint variant with pin 1 at the top-left and the antenna keep-out aligned to the antenna end.
* A project-local `.pretty` footprint library workflow using `${KIPRJMOD}` paths.
* WRL 3D model iterations with clearer antenna-end orientation cues.
* Assembly notes for stencil, paste, reflow, hot plate and hot-air approaches.
* RF keep-out guidance, especially avoiding copper, batteries and large conductors beneath or near the integrated antenna.

The current decision remains conservative: use the DWM3001C-CDK for the first docked and testable build, then migrate to the castellated module after the data path and mechanical approach are proven. This preserves momentum while still preparing the project for a smaller, cleaner later revision.

\---

## ADXL1005 module integration

The ADXL1005 evaluation board was also turned into a proper KiCad object rather than an informal off-board part. A custom symbol and footprint were produced for the EVAL-ADXL1005Z style board, exposing the six key signals:

```text
VDD
GND
ST
VOUT
OR
STANDBY
```

The footprint uses the 2.54 mm header structure and measured board outline, with the mechanical outline converging around 20.35 mm × 20.35 mm. This gives the sensor board a defined position on the carrier and makes the analogue route, test points and local decoupling much more repeatable.

Control pins such as ST, STANDBY and OR can be left unused in the earliest bring-up when not required. However, the design preserves the option to use STANDBY and PFET gating for power behaviour experiments.

\---

## Gateway rev 0.1: from architecture to fabrication candidate

After the node carrier reached fabrication, attention shifted to the Gateway rev 0.1 PCB. This board is a carrier and wiring concentrator for the heavier compute side of the system.

The Gateway architecture now consists of:

* STM32 NUCLEO-N657X0-Q as the main compute and DSP/TinyML host.
* DWM3001C-CDK as the UWB receiver/transport endpoint during bring-up.
* Seeed Studio XIAO ESP32-C6 as the removable network/backhaul module.
* Shared SPI5 bus for Gateway-side data movement.
* Separate chip-selects and control lines for the UWB and ESP32-C6 devices.

A significant design clean-up was moving away from a mixed Arduino/Zio-header strategy and consolidating the NUCLEO wiring around the ST morpho headers. CN15 now carries the main SPI5 and control signals, while CN3 carries power, ground and auxiliary control such as the XIAO enable/reset line. This creates a cleaner gateway carrier and reduces the chance of confusing multiple header naming schemes.

The XIAO ESP32-C6 is represented as a DIP/header module rather than a direct castellated SMD part. That is the right trade for rev 0.1 because it makes the module removable, easier to probe and easier to replace during bring-up.

By 3 May 2026, the Gateway rev 0.1 design had reached a clean KiCad milestone:

```text
ERC: 0 errors, 0 warnings
DRC: 0 violations
Unconnected pads: 0
Footprint errors: 0
Gerbers: generated
```

The remaining work then shifted from circuit correction to manufacturing-output validation.

### Final Gateway fabrication checks

The last Gateway revision pass added two important practical details:

* Four corner mounting holes.
* Two 10 kΩ through-hole pull-up resistors on `DWM\_CS` and `C6\_CS`, pulled up to `3V3\_C6`.

The pull-ups prevent accidental SPI peripheral selection during reset or before the STM32 firmware has configured the relevant GPIO pins. Through-hole resistors were chosen deliberately for the first bench prototype because they are easy to solder, easy to replace and entirely adequate for low-speed biasing.

The Gerber and Excellon drill outputs were regenerated and checked. The critical manufacturing split was confirmed:

```text
H1-H4 mounting holes: NPTH mechanical holes
R1/R2 pull-up resistor holes: PTH solderable holes
Header holes: PTH solderable holes
```

On 4 May 2026, the Gateway rev 0.1 fabrication ZIP was uploaded to a UK PCB manufacturer. A late outline improvement added 3 mm radius corners, which were checked successfully in the manufacturer preview. The Gateway board is now a stable pre-order fabrication candidate, pending purchase.

\---

## Firmware direction

### Node firmware

The immediate node firmware task is SAADC window sampling on P0.28 / AIN4. The sensible first implementation is deliberately modest:

* Configure a single-ended SAADC channel on P0.28.
* Use a gain/reference combination that safely covers the 0 V to 3.3 V signal range.
* Use conservative acquisition time because of the 8 kΩ source impedance.
* Calibrate offset.
* Trigger sampling from a timer rather than from an ordinary task loop.
* Use double-buffered windows so UWB activity and FreeRTOS scheduling do not dominate sample timing.
* Start with simple known-input tests, then move to real ADXL1005 tap and vibration captures.

The target is not yet a sophisticated vibration analyser. The target is first to prove that the node can capture repeatable windows with correct timing and plausible amplitude.

### Gateway firmware

The Gateway firmware should start with a minimal SPI data-extraction harness. Before feature extraction or TinyML is introduced, the Gateway must prove that it can receive frames from the UWB side and extract them deterministically into the STM32.

The proposed first Gateway firmware should:

* Configure SPI5 as the master data path.
* Hold the ESP32-C6 chip-select inactive while talking to the DWM3001C-CDK.
* Use a DWM interrupt or data-ready signal to trigger reads.
* Clock out fixed-size test frames first.
* Compute CRC32 or an equivalent integrity check.
* Emit concise diagnostics over SWV, ITM or UART.

Only once this is stable should the Gateway move into DSP feature extraction.

### DSP and TinyML

The project has deliberately avoided putting TinyML ahead of evidence. The right sequence is:

```text
signal first -> windows second -> features third -> classifier fourth
```

The first useful feature set should be small and interpretable: RMS, peak, crest factor, basic spectral energy bands, envelope-style indicators and simple time-domain statistics. These can later feed a compact classifier. The Gateway, with the STM32N657 and its stronger compute path, remains the natural place for heavier inference, model comparison and richer diagnostics.

The long-term direction remains ‘features on the Pod, model on the Gateway’ unless later testing proves that on-node inference provides a clear power, latency or robustness advantage.

\---

## How this maps to Hackster judging criteria

### Project documentation

Thin-Pod now has a strong evidence trail. The logs show a staged path from firmware flashing, console control and UWB ranging through analogue sensor debugging, KiCad schematic capture, PCB routing, Gerber generation, manufacturer preview and board order. Failures and corrections are documented rather than hidden: ADC loading, probe pickup, NC header assumptions, KiCad connector mismatches, footprint-library errors, ground-zone net naming and PTH/NPTH drill distinctions all became part of the engineering story.

### Complete BOM

The core BOM is now stable enough for a public project page:

* Qorvo DWM3001C-CDK for UWB bring-up.
* Future DWM3001C module path for compact revisions.
* Analog Devices ADXL1005 / EVAL-ADXL1005Z analogue accelerometer front end.
* Pololu S7V8F3 3.3 V regulator module.
* 1N5817 Schottky diode for reverse-polarity protection.
* Fuse holder in the raw input path.
* ZVP2106A P-channel MOSFET for switched accelerometer rail control.
* 8 kΩ / 680 pF / 1 MΩ ADC conditioning network.
* Local 100 nF and 10 µF switched-rail decoupling.
* JST-PH power entry.
* STM32 NUCLEO-N657X0-Q Gateway board.
* Seeed Studio XIAO ESP32-C6 network/backhaul module.
* Test points, headers, mounting holes, spacers and hand-assembly passives.

### Schematics and PCB design

The project now contains explicit rail naming, signal naming and KiCad-ready structure. The node board has been ordered after clean ERC/DRC and checked Gerber/drill output. The Gateway board has reached clean ERC/DRC, generated Gerbers, checked PTH/NPTH split and a manufacturer-upload milestone.

This is a strong Hackster point: the project is no longer just a block diagram. It has passed through the uncomfortable but essential fabrication-prep layer where many prototypes reveal their real weaknesses.

### Code and reproducibility

The code direction is narrow enough to be credible:

* proven vendor SDK flashing and console baseline,
* next node-side SAADC window sampler,
* synthetic UWB payload transport test,
* real ADC-window payload test,
* Gateway SPI5 reader,
* CRC/integrity diagnostics,
* then DSP feature extraction and TinyML experiments.

That sequence is easier to reproduce than a single large ‘final firmware’ drop, because each step has a measurable pass/fail condition.

### Creativity

Thin-Pod’s creative strength is not only the use of UWB with vibration telemetry. It is the system split: a small sensor node that captures vibration windows and a Gateway that can do the heavier thinking. The project also keeps development-kit convenience long enough to reduce RF and firmware risk, while steadily building the footprints, libraries and carrier boards needed to move beyond the kits.

In Hackster terms, this gives the project a strong maker-engineering arc: start with accessible boards, prove the physics and data path, then harden the design into manufacturable hardware.

\---

## What changed since the January narrative

The January version of this narrative ended with carrier-board layout and SAADC sampling as the next major milestones. Since then, the project has made three large jumps.

First, the DWM3001C and ADXL1005 were turned into better KiCad citizens. Footprints, symbols, 3D models, project-local libraries, antenna orientation markers and import workflows were built or corrected.

Second, the Thin-Pod node carrier was routed, cleaned, checked and ordered. That work included practical fabrication details such as connector orientation, fuse-path verification, diode polarity, ground-zone behaviour, test-point access, mounting-hole clearance and Gerber Viewer inspection.

Third, the Gateway rev 0.1 PCB was brought from concept to a clean fabrication candidate. It now has the NUCLEO-N657X0-Q, DWM3001C-CDK and XIAO ESP32-C6 integrated into a coherent carrier-board design with shared SPI, proper chip-select biasing, mounting holes, generated fabrication files and manufacturer upload.

The project has therefore shifted from ‘can this architecture be made plausible?’ to ‘how cleanly can the first manufactured prototypes be assembled, powered, tested and documented?’

\---

## Immediate next milestones

The next work should stay practical and measurement-led.

1. Place the Gateway rev 0.1 PCB order after one final manufacturer preview check.
2. Prepare a Thin-Pod rev 0.1 bring-up and verification report before the ordered board arrives.
3. On arrival, inspect the bare Thin-Pod PCB, then perform continuity checks before fitting parts.
4. Assemble the power-entry and regulator section first, then bring the board up from a current-limited supply.
5. Measure TP1 to TP7 in sequence before attaching or trusting the CDK and sensor sections.
6. Verify PFET switching of `ACC\_3V3\_SW` and confirm that the ADXL1005 output appears at TP6 and the filtered ADC node at TP7.
7. Implement and test SAADC window capture on P0.28.
8. Transport synthetic UWB payloads first, then real ADC windows.
9. Bring up the Gateway SPI5 reader and prove frame extraction with CRC checks.
10. Begin the first feature-extraction loop on captured vibration windows.

The next visible Hackster milestone should be a photographed and measured bring-up sequence: bare board, staged assembly, first power, test-point voltages, ADC capture, UWB payload, and Gateway ingest. That would turn the project from a well-documented design into a demonstrable end-to-end prototype.

\---

## One-sentence project position

Thin-Pod is becoming a reproducible open-hardware vibration telemetry platform: an analogue accelerometer node using UWB for robust local transport, paired with an STM32N6-class Gateway for DSP, TinyML and higher-level machinery-health decisions.

