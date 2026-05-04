# Thin-Pod Gateway rev 0.1 Bring-Up and Verification Procedure

**Document:** Thin-Pod Gateway rev 0.1 Bring-Up and Verification Procedure  
**Level:** SCQF Level 10 style technical procedure  
**Project:** Thin-Pod Gateway rev 0.1  
**Prepared:** 04 May 2026  
**Board status:** Fabrication candidate uploaded to UK PCB manufacturer, pending order  
**Purpose:** Provide a structured bring-up, verification, evidence-capture and fault-isolation procedure for the first assembled Thin-Pod Gateway rev 0.1 PCB.

\---

## 1\. Purpose and scope

This procedure defines the bring-up and verification method for the first Thin-Pod Gateway rev 0.1 carrier board after the bare PCB arrives.

The procedure covers:

* bare-board inspection,
* fabrication checks,
* continuity and isolation checks,
* staged power checks,
* component/header population strategy,
* NUCLEO-N657X0-Q fit and connection checks,
* DWM3001C-CDK interface checks,
* Seeed Studio XIAO ESP32-C6 DIP/header interface checks,
* SPI5 shared-bus verification,
* chip-select pull-up verification,
* reset and interrupt line verification,
* ground and power integrity checks,
* basic firmware-assisted validation,
* acceptance criteria,
* evidence capture,
* and fault-isolation workflow.

The procedure is written for the gateway carrier board only. It does not include Thin-Pod node sensor bring-up, ADXL1005 analogue-chain validation, UWB ranging, rotor-fault firmware, or end-to-end vibration data acquisition. Those belong in later verification documents.

\---

## 2\. Engineering objective

The objective is to prove that the Thin-Pod Gateway rev 0.1 PCB is a usable first bench prototype carrier for:

* NUCLEO-N657X0-Q,
* DWM3001C-CDK,
* Seeed Studio XIAO ESP32-C6 DIP/header module,
* shared SPI5 connectivity,
* power distribution,
* ground continuity,
* chip-select biasing,
* reset and interrupt signal routing,
* and mechanical fit.

The target outcome is not final product qualification. The target outcome is a defensible engineering conclusion:

> The rev 0.1 gateway carrier is electrically safe to power, mechanically usable for bench bring-up, and suitable for firmware-level SPI/interface validation.

\---

## 3\. Board architecture under test

The board is a gateway carrier PCB connecting the NUCLEO-N657X0-Q to two attached communications/module boards.

### 3.1 Main modules

|Reference|Module|Role|
|-|-|-|
|U1|NUCLEO-N657X0-Q|Gateway host / STM32N6 development board|
|U2|DWM3001C-CDK|UWB module/dev board interface|
|U3|Seeed Studio XIAO ESP32-C6 DIP/header|Secondary wireless/communications/dev module|
|R1|10 kΩ through-hole resistor|Pull-up for one chip-select net|
|R2|10 kΩ through-hole resistor|Pull-up for second chip-select net|
|H1-H4|Corner mounting holes|Mechanical fixing holes, NPTH|

### 3.2 NUCLEO header strategy

The design intentionally consolidates the NUCLEO-side wiring onto:

* **CN15** for SPI5 and most logic/control signals,
* **CN3** for power, ground and XIAO enable/reset.

The design avoids using the Arduino/Zio CN5, CN13 and CN14 headers for this final rev 0.1 gateway carrier wiring.

### 3.3 Main net list

|Net|NUCLEO source|Destination(s)|Purpose|
|-|-|-|-|
|`SPI5\_SCK`|U1 CN15.11 / PE15|U2 J10.23, U3 SCK|Shared SPI clock|
|`SPI5\_MISO`|U1 CN15.13 / PG1|U2 J10.21, U3 MISO|Shared SPI MISO|
|`SPI5\_MOSI`|U1 CN15.15 / PG2|U2 J10.19, U3 MOSI|Shared SPI MOSI|
|`DWM\_CS`|U1 CN15.17 / PA3|U2 J10.24, pull-up|DWM chip-select|
|`DWM\_IRQ`|U1 CN15.16 / PB9|U2 J10.15|DWM interrupt|
|`DWM\_RESET`|U1 CN15.33 / PD0|U2 J10.12|DWM reset|
|`C6\_CS`|U1 CN15.19 / PD7|U3 CS, pull-up|XIAO ESP32-C6 chip-select|
|`C6\_INT`|U1 CN15.5 / PC1|U3 INT|XIAO interrupt / host-wake|
|`C6\_EN\_RST`|U1 CN3.17 / PC0|U3 EN/RST|XIAO enable/reset|
|`5V\_DWM`|U1 CN3.6 / 5 V|U2 5 V input|DWM power|
|`3V3\_C6`|U1 CN3.16 / 3.3 V|U3 3V3 and pull-ups|XIAO and pull-up rail|
|`GND`|U1 CN3.8 and ground pour|U2 GND, U3 GND|Ground reference|

\---

## 4\. Required equipment

### 4.1 Minimum equipment

* Digital multimeter with continuity, resistance and DC voltage modes.
* Bench power supply with current limit.
* USB-C cable for NUCLEO/STLINK.
* USB-C cable for XIAO ESP32-C6, if separate programming/checking is required.
* Fine probe leads or needle probes.
* Magnifier or microscope.
* Soldering iron suitable for through-hole soldering.
* Flux, solder, side cutters, tweezers.
* IPA or suitable PCB cleaning fluid.
* ESD mat and wrist strap if available.

### 4.2 Recommended equipment

* Oscilloscope, preferably 100 MHz or better.
* Logic analyser, 8-channel minimum.
* Current-limited USB power meter.
* Bench jumper wires.
* Known-good 10 kΩ through-hole resistors if not already fitted.
* Header sockets/pins matching NUCLEO, DWM3001C-CDK and XIAO footprints.
* Camera or phone for evidence photographs.
* KiCad project, schematic PDF and board layout available during testing.

### 4.3 Firmware/tools

At minimum, prepare:

* a basic STM32N6 GPIO test firmware,
* a basic SPI5 loop/activity test firmware,
* a pin-toggling firmware build for CN15/CN3 signal validation,
* a serial console or debug output route,
* and, if possible, a known-good DWM3001C-CDK SPI ID/register read test.

\---

## 5\. Safety and handling

The first board should be treated as unproven hardware.

The following precautions apply:

* Do not populate all modules before bare-board checks are complete.
* Do not power the board until 5 V, 3.3 V and GND shorts have been checked.
* Use current limiting during first power-up.
* Avoid powering the XIAO from both USB-C and the NUCLEO/carrier 3.3 V rail unless back-feed behaviour has been intentionally checked.
* Avoid connecting the DWM3001C-CDK until the 5 V rail has been checked at the carrier header.
* Avoid using the NUCLEO NRST line as a module reset line. The DWM reset is intended to be independently driven by `DWM\_RESET`.
* If any rail current is unexpectedly high, remove power immediately and inspect for shorts, reversed parts, solder bridges or incorrect headers.

\---

## 6\. Evidence capture requirements

SCQF Level 10 style verification requires more than casual success/failure notes. The test record should show the basis for the engineering conclusion.

For each test stage, record:

* date and time,
* board serial/identifier,
* operator,
* instrument used,
* test setup,
* measured values,
* pass/fail outcome,
* photographs where useful,
* anomalies,
* corrective action,
* and retest result if applicable.

Recommended evidence filenames:

```text
TPGW\_rev0\_1\_bare\_board\_top\_photo.jpg
TPGW\_rev0\_1\_bare\_board\_bottom\_photo.jpg
TPGW\_rev0\_1\_continuity\_table.md
TPGW\_rev0\_1\_first\_power\_current\_limit.jpg
TPGW\_rev0\_1\_3v3\_rail\_measurement.jpg
TPGW\_rev0\_1\_spi5\_logic\_analyser\_capture.sal
TPGW\_rev0\_1\_dwm\_cs\_scope\_capture.png
TPGW\_rev0\_1\_c6\_cs\_scope\_capture.png
TPGW\_rev0\_1\_bringup\_results.md
```

\---

## 7\. Acceptance criteria

The board may be considered a successful rev 0.1 bring-up candidate if all of the following are true.

### 7.1 Bare-board acceptance

* Board outline is correct, including 3 mm corner radii.
* H1-H4 mounting holes are present and clear.
* Header holes and through-hole resistor holes are drilled.
* No obvious manufacturing defects are visible.
* No short exists between 5 V and GND.
* No short exists between 3.3 V and GND.
* No short exists between 5 V and 3.3 V.
* Required nets show expected continuity.

### 7.2 Assembly acceptance

* Through-hole pull-up resistors are fitted correctly.
* Header sockets/pins are mechanically aligned.
* NUCLEO, DWM3001C-CDK and XIAO can be fitted without mechanical clash.
* Silkscreen orientation and labels are usable during bench work.

### 7.3 Electrical acceptance

* 5 V appears at the intended DWM power connection only when expected.
* 3.3 V appears at the XIAO 3V3 rail only when expected.
* GND continuity exists across U1, U2 and U3 ground points.
* `DWM\_CS` and `C6\_CS` idle high via 10 kΩ pull-ups.
* NUCLEO firmware can actively drive `DWM\_CS` and `C6\_CS` low.
* `SPI5\_SCK`, `SPI5\_MOSI` and `SPI5\_MISO` reach the intended module pins.
* `DWM\_RESET`, `DWM\_IRQ`, `C6\_INT` and `C6\_EN\_RST` reach the intended pins.
* No unintended rail collapse or excessive current occurs during testing.

### 7.4 Functional acceptance

* SPI5 signal activity can be observed on the shared bus.
* Chip-select lines can be independently controlled.
* At least one meaningful module-level communication check succeeds, or a clear reason is documented if firmware support is not yet available.
* Any failures are localised to firmware/configuration, module setup or board hardware with supporting evidence.

\---

## 8\. Stage 0: Documentation and manufacturing-file check

### 8.1 Objective

Confirm that the board received from the manufacturer matches the intended rev 0.1 design.

### 8.2 Procedure

1. Confirm the order corresponds to the latest fabrication ZIP, not an older checkpoint export.
2. Confirm board quantity, solder mask colour, silkscreen colour, copper weight and board thickness against the order.
3. Compare the physical board outline with the Gerber Viewer output.
4. Confirm 3 mm corner radii are present.
5. Confirm H1-H4 mounting holes are present.
6. Confirm the two through-hole resistor positions are present.
7. Confirm module/header hole arrays are present.
8. Confirm silkscreen text is readable and not mirrored.
9. Photograph the top and bottom sides of the bare board.

### 8.3 Pass criteria

* Board visually matches the latest Gerber preview.
* Rounded corners and mounting holes are present.
* No obvious fabrication mismatch is observed.

### 8.4 Failure response

If the board does not match the latest Gerber preview, stop bring-up and compare the manufacturer’s submitted ZIP against the KiCad project outputs. Determine whether the wrong ZIP was uploaded, the manufacturer interpreted the files incorrectly, or the KiCad output was incorrect.

\---

## 9\. Stage 1: Bare-board visual inspection

### 9.1 Objective

Identify manufacturing defects before soldering or power application.

### 9.2 Inspection points

Check for:

* broken or incomplete tracks,
* scratches through copper,
* solder mask defects,
* missing drill holes,
* blocked holes,
* misregistered holes,
* poor board-edge routing,
* silkscreen over critical pads,
* copper slivers,
* unexpected bridges between adjacent pads,
* and rough hole plating in PTH holes.

### 9.3 Special checks

Confirm that:

* H1-H4 are non-plated mechanical holes,
* R1/R2 resistor holes are plated through holes,
* header holes are plated through holes,
* ground pour is present,
* Edge.Cuts has been manufactured with rounded corners,
* and there are no visible shorts around the dense NUCLEO footprint.

### 9.4 Pass criteria

* No defect is visible that would prevent safe continuity testing or soldering.

\---

## 10\. Stage 2: Bare-board continuity and isolation checks

### 10.1 Objective

Confirm that the bare PCB has no obvious shorts and that critical nets connect as intended.

### 10.2 Isolation checks

With no components or headers fitted, measure resistance between:

|Test|Expected result|
|-|-|
|`5V\_DWM` to `GND`|Open / high resistance|
|`3V3\_C6` to `GND`|Open / high resistance|
|`5V\_DWM` to `3V3\_C6`|Open / high resistance|
|`DWM\_CS` to `GND`|Open / high resistance|
|`C6\_CS` to `GND`|Open / high resistance|
|`SPI5\_SCK` to `GND`|Open / high resistance|
|`SPI5\_MISO` to `GND`|Open / high resistance|
|`SPI5\_MOSI` to `GND`|Open / high resistance|

Because some nets may be connected to footprints with no fitted components, the expected reading is normally open circuit or very high resistance.

### 10.3 Continuity checks

Check continuity across each intended net:

|Net|Continuity points|
|-|-|
|`5V\_DWM`|U1 CN3.06 to U2 5 V pin|
|`3V3\_C6`|U1 CN3.16 to U3 3V3 pin and pull-up resistor pads|
|`GND`|U1 CN3.08 to U2 GND to U3 GND and ground pour|
|`SPI5\_SCK`|U1 CN15.11 to U2 J10.23 to U3 SCK|
|`SPI5\_MISO`|U1 CN15.13 to U2 J10.21 to U3 MISO|
|`SPI5\_MOSI`|U1 CN15.15 to U2 J10.19 to U3 MOSI|
|`DWM\_CS`|U1 CN15.17 to U2 J10.24 to R pull-up pad|
|`C6\_CS`|U1 CN15.19 to U3 CS to R pull-up pad|
|`DWM\_RESET`|U1 CN15.33 to U2 J10.12|
|`DWM\_IRQ`|U1 CN15.16 to U2 J10.15|
|`C6\_INT`|U1 CN15.5 to U3 INT|
|`C6\_EN\_RST`|U1 CN3.17 to U3 EN/RST|

### 10.4 Pass criteria

* All intended nets show continuity.
* No power rail shorts are detected.
* No adjacent SPI/control nets are shorted together.

### 10.5 Failure response

If continuity is missing, inspect the Gerber, PCB trace and pad. If a short is found, inspect for copper bridge, manufacturing defect or design error. Do not proceed to power testing until all rail shorts are understood.

\---

## 11\. Stage 3: Minimal assembly strategy

### 11.1 Objective

Populate the board in a controlled sequence so that failures can be isolated.

### 11.2 Recommended population order

1. Fit only the two through-hole pull-up resistors R1/R2.
2. Fit any required low-profile headers or sockets for the XIAO and DWM interfaces.
3. Fit NUCLEO mating headers if they are separate from the NUCLEO board.
4. Leave DWM3001C-CDK and XIAO modules disconnected initially.
5. Perform post-solder continuity checks.
6. Fit NUCLEO only.
7. Perform NUCLEO-side power and GPIO checks.
8. Fit XIAO only if required for XIAO interface checks.
9. Fit DWM3001C-CDK only after 5 V and reset/CS nets are verified.
10. Test the complete assembly last.

### 11.3 Soldering checks

After soldering, inspect:

* resistor solder joints,
* header alignment,
* bridges between header pins,
* cold joints,
* lifted pads,
* and damaged silkscreen/copper.

### 11.4 Pass criteria

* No visible solder defects.
* Continuity remains correct after assembly.
* No new shorts are introduced.

\---

## 12\. Stage 4: Pull-up resistor verification

### 12.1 Objective

Confirm that R1/R2 provide correct chip-select idle bias.

### 12.2 Unpowered resistance check

With the board unpowered:

|Measurement|Expected result|
|-|-|
|`DWM\_CS` to `3V3\_C6`|approximately 10 kΩ|
|`C6\_CS` to `3V3\_C6`|approximately 10 kΩ|
|`DWM\_CS` to `GND`|high resistance|
|`C6\_CS` to `GND`|high resistance|

### 12.3 Powered voltage check

With NUCLEO fitted and 3.3 V present, but before firmware actively drives the pins:

|Net|Expected idle voltage|
|-|-|
|`DWM\_CS`|near 3.3 V|
|`C6\_CS`|near 3.3 V|

### 12.4 Pass criteria

* Both CS nets idle high.
* Neither CS net is shorted to GND.
* NUCLEO firmware can later pull each CS line low.

### 12.5 Failure response

If a CS net does not idle high, check resistor value, soldering, net label assignment, wrong footprint pin, 3.3 V presence and possible GPIO configuration conflict.

\---

## 13\. Stage 5: Mechanical fit check

### 13.1 Objective

Confirm that the board can physically host the intended modules and connectors.

### 13.2 Procedure

Without powering the board:

1. Fit the NUCLEO-N657X0-Q mechanically.
2. Confirm connector/header alignment.
3. Confirm no board corner, mounting hole or rounded edge clashes with the NUCLEO.
4. Fit the XIAO ESP32-C6 DIP/header module.
5. Confirm USB-C access if the XIAO requires independent programming.
6. Fit the DWM3001C-CDK.
7. Confirm no mechanical clash between modules.
8. Confirm access to test points, headers and reset/programming connections.
9. Confirm mounting holes remain usable with standoffs or screws.

### 13.3 Pass criteria

* Modules fit without force.
* USB and debug access remain practical.
* Board can be handled without stressing solder joints or headers.

\---

## 14\. Stage 6: First power-up without peripheral modules

### 14.1 Objective

Power the carrier/NUCLEO side while reducing risk to external modules.

### 14.2 Procedure

1. Remove DWM3001C-CDK and XIAO if already fitted.
2. Fit only the NUCLEO and passive components/headers required.
3. Set bench supply current limit conservatively if external power is used.
4. Alternatively, power the NUCLEO through its normal USB/STLINK path.
5. Measure `GND`, `3V3\_C6` and `5V\_DWM` at the carrier pads.
6. Record current consumption.
7. Check for heating, smell, unstable voltage or repeated reset.

### 14.3 Expected results

|Measurement|Expected result|
|-|-|
|`GND`|common reference|
|`3V3\_C6`|approximately 3.3 V|
|`5V\_DWM`|approximately 5 V if the NUCLEO 5 V rail is present|
|`DWM\_CS`|approximately 3.3 V|
|`C6\_CS`|approximately 3.3 V|

### 14.4 Pass criteria

* Power rails are present and stable.
* No unexpected current or heating.
* CS pull-ups remain high.

\---

## 15\. Stage 7: GPIO line verification from NUCLEO

### 15.1 Objective

Confirm that NUCLEO GPIO pins reach the correct carrier nets before module-level communication is attempted.

### 15.2 Firmware behaviour

Load a simple firmware image that toggles or drives the following pins one at a time:

|Net|STM32 pin|
|-|-|
|`DWM\_CS`|PA3|
|`C6\_CS`|PD7|
|`DWM\_RESET`|PD0|
|`C6\_EN\_RST`|PC0|
|`SPI5\_SCK`|PE15, test as GPIO if needed|
|`SPI5\_MOSI`|PG2, test as GPIO if needed|

For input nets, configure pull-ups/pull-downs carefully and read status:

|Net|STM32 pin|
|-|-|
|`DWM\_IRQ`|PB9|
|`C6\_INT`|PC1|
|`SPI5\_MISO`|PG1|

### 15.3 Measurement method

Use a multimeter for slow manual toggles or an oscilloscope/logic analyser for faster toggles.

Measure at:

* NUCLEO header side,
* destination module header side,
* and pull-up resistor side where relevant.

### 15.4 Pass criteria

* Each output net toggles only the expected net.
* No adjacent nets toggle unintentionally.
* Input nets can be observed and are not stuck unexpectedly.
* `DWM\_CS` and `C6\_CS` can both be driven low against their 10 kΩ pull-ups.

\---

## 16\. Stage 8: SPI5 electrical activity check

### 16.1 Objective

Confirm that SPI5 activity reaches the intended shared bus pins.

### 16.2 Procedure

1. Keep modules disconnected initially if possible.
2. Configure SPI5 on the NUCLEO.
3. Generate repeated SPI transfers.
4. Observe:

   * `SPI5\_SCK`,
   * `SPI5\_MOSI`,
   * `SPI5\_MISO`,
   * `DWM\_CS`,
   * `C6\_CS`.
5. Confirm chip-select independence:

   * assert `DWM\_CS` while `C6\_CS` remains high,
   * assert `C6\_CS` while `DWM\_CS` remains high.

### 16.3 Expected signal behaviour

|Signal|Expected behaviour|
|-|-|
|`SPI5\_SCK`|Clock pulses during transfer|
|`SPI5\_MOSI`|Data transitions during transfer|
|`SPI5\_MISO`|Idle or module-driven data depending on fitted module|
|`DWM\_CS`|High idle, low only during DWM transaction|
|`C6\_CS`|High idle, low only during C6 transaction|

### 16.4 Pass criteria

* SPI clock and MOSI activity appear at both module headers.
* Chip-select lines are independent.
* No bus contention is visible.

\---

## 17\. Stage 9: XIAO ESP32-C6 interface check

### 17.1 Objective

Validate the carrier interface to the XIAO ESP32-C6 module.

### 17.2 Power caution

Do not power the XIAO from both its USB-C port and `3V3\_C6` unless back-feed behaviour has been deliberately checked.

### 17.3 Procedure

1. Fit XIAO module.
2. Confirm `3V3\_C6` at the XIAO 3V3 pin.
3. Confirm XIAO GND continuity.
4. Confirm `C6\_CS` idles high.
5. Toggle `C6\_CS` from the NUCLEO and measure at the XIAO pin.
6. Toggle or control `C6\_EN\_RST` and observe XIAO reset/enable behaviour if accessible.
7. If firmware exists on the XIAO, test `C6\_INT` or a simple host-wake line.
8. If SPI peripheral firmware exists on the XIAO, test one SPI transaction.

### 17.4 Pass criteria

* XIAO powers as expected.
* XIAO can be reset/enabled if the line is connected to its reset/enable behaviour.
* `C6\_CS` reaches the module.
* No rail conflict occurs during USB/programming scenarios.

\---

## 18\. Stage 10: DWM3001C-CDK interface check

### 18.1 Objective

Validate the carrier interface to the DWM3001C-CDK.

### 18.2 Procedure

1. Fit DWM3001C-CDK only after 5 V and GND have been verified.
2. Measure 5 V at the DWM power input.
3. Confirm DWM GND continuity.
4. Confirm `DWM\_CS` idles high.
5. Toggle `DWM\_CS` low/high from the NUCLEO and measure at DWM J10.24.
6. Toggle `DWM\_RESET` and measure at DWM J10.12.
7. Observe `DWM\_IRQ` at the NUCLEO side and DWM side.
8. Run a firmware test to read a DWM register or identification value if firmware support is ready.

### 18.3 Pass criteria

* DWM power is correct.
* `DWM\_CS`, `DWM\_RESET` and `DWM\_IRQ` are electrically present.
* SPI transaction to DWM succeeds, or failure is localised with evidence.

\---

## 19\. Stage 11: Combined shared-bus test

### 19.1 Objective

Confirm that both SPI peripherals can coexist on the shared SPI5 bus.

### 19.2 Procedure

1. Fit NUCLEO, DWM3001C-CDK and XIAO.
2. Power the system using the intended bench method.
3. Confirm both CS lines idle high.
4. Assert only `DWM\_CS` and perform a DWM SPI transaction.
5. Assert only `C6\_CS` and perform a C6 SPI transaction, if firmware support exists.
6. Monitor shared `SCK`, `MOSI` and `MISO` lines with a logic analyser.
7. Confirm no peripheral drives MISO when deselected.
8. Confirm current consumption remains stable.

### 19.3 Pass criteria

* Each device is selectable independently.
* Non-selected device remains inactive.
* No MISO contention is observed.
* Shared-bus signal integrity is adequate for bench testing.

\---

## 20\. Fault-isolation guide

### 20.1 No power on 3.3 V rail

Likely causes:

* NUCLEO not supplying 3.3 V as expected,
* wrong header pin,
* open trace,
* soldering/header fault,
* measurement reference error.

Checks:

* measure directly on NUCLEO CN3.16,
* measure at carrier pad,
* measure at XIAO pad,
* check continuity unpowered.

### 20.2 CS line not high at idle

Likely causes:

* missing 10 kΩ resistor,
* wrong resistor value,
* poor solder joint,
* wrong net label,
* `3V3\_C6` not present,
* GPIO configured low,
* module pulling CS low unexpectedly.

Checks:

* resistance from CS to `3V3\_C6`,
* voltage at both resistor pads,
* firmware GPIO state,
* remove module and retest.

### 20.3 SPI clock absent

Likely causes:

* SPI5 not configured,
* wrong alternate function,
* wrong STM32 pin mapping,
* firmware using wrong SPI peripheral,
* hardware trace/pad issue.

Checks:

* toggle PE15 as GPIO first,
* verify CN15.11 continuity,
* check SCK at NUCLEO and module headers,
* inspect firmware pinmux.

### 20.4 MISO stuck or bus contention

Likely causes:

* both peripherals selected,
* one CS line not high,
* module firmware/hardware holding MISO,
* wrong wiring,
* pull-up/pull-down conflict.

Checks:

* observe both CS lines with logic analyser,
* remove one module and retest,
* test DWM alone,
* test XIAO alone.

### 20.5 DWM reset does not respond

Likely causes:

* wrong reset polarity assumption,
* wrong DWM J10 pin,
* PD0 not configured as output,
* solder/header fault,
* reset pin held by module circuit.

Checks:

* scope `DWM\_RESET`,
* toggle PD0 slowly,
* check continuity U1 CN15.33 to U2 J10.12,
* verify DWM documentation for reset behaviour.

### 20.6 XIAO reset/enable behaviour unexpected

Likely causes:

* EN/RST function not equivalent to assumed reset input,
* XIAO power-source conflict,
* PC0 configuration issue,
* boot/enable behaviour specific to XIAO ESP32-C6 module.

Checks:

* verify XIAO pin function,
* test with USB-only power,
* test with carrier-only power,
* monitor 3.3 V rail during reset.

\---

## 21\. Bring-up record template

Use the following table during bench work.

|Stage|Test|Expected result|Measured result|Pass/Fail|Evidence file|Notes|
|-|-|-|-|-|-|-|
|0|Fabrication package match|Board matches Gerbers|||||
|1|Visual inspection|No visible defects|||||
|2|5 V to GND isolation|High resistance|||||
|2|3.3 V to GND isolation|High resistance|||||
|2|GND continuity|Continuous|||||
|4|DWM\_CS pull-up|\~10 kΩ to 3V3, high idle|||||
|4|C6\_CS pull-up|\~10 kΩ to 3V3, high idle|||||
|6|First power-up|Stable rails|||||
|7|GPIO toggle|Expected pins toggle|||||
|8|SPI5 activity|SCK/MOSI observed|||||
|9|XIAO interface|Power and CS verified|||||
|10|DWM interface|Power, CS, reset verified|||||
|11|Shared bus|No CS/MISO contention|||||

\---

## 22\. Decision gates

### Gate A: Safe to solder

Proceed only if:

* visual inspection passes,
* no rail shorts exist,
* critical continuity checks pass.

### Gate B: Safe to power

Proceed only if:

* resistors/headers are inspected,
* rail isolation still passes,
* no visible solder bridges are present.

### Gate C: Safe to fit modules

Proceed only if:

* 5 V and 3.3 V rails are correct,
* CS pull-ups work,
* reset/control nets do not show unexpected states.

### Gate D: Suitable for firmware bring-up

Proceed only if:

* GPIO lines can be toggled,
* SPI5 activity is visible,
* chip-select lines are independent,
* no rail instability is observed.

### Gate E: Rev 0.1 accepted as bench prototype

Accept if:

* ERC/DRC/manufacturing checks are traceable,
* board powers safely,
* modules fit,
* main nets verify,
* shared SPI bus can be exercised,
* and remaining issues are documented with mitigation.

\---

## 23\. Final acceptance statement

At the end of bring-up, complete one of the following statements.

### Accepted

```text
Thin-Pod Gateway rev 0.1 has passed bare-board, power, connectivity and initial interface verification. The board is accepted as a usable bench prototype for firmware and gateway integration work.
```

### Accepted with limitations

```text
Thin-Pod Gateway rev 0.1 is accepted as a bench prototype with the following limitations:
\[limitations]
The board may be used for controlled testing but requires the following mitigations:
\[mitigations]
```

### Rejected / rework required

```text
Thin-Pod Gateway rev 0.1 has not passed bring-up verification. The following faults block use:
\[faults]
Required rework or rev 0.2 changes:
\[actions]
```

\---

## 24\. Rev 0.2 feedback capture

During bring-up, capture all possible improvements for rev 0.2:

* footprint refinements,
* connector placement changes,
* test point additions,
* labelling improvements,
* power-entry changes,
* grounding changes,
* module placement changes,
* SMD conversion candidates,
* mounting-hole changes,
* silkscreen improvements,
* and firmware/programming access improvements.

Suggested rev 0.2 log table:

|Observation|Severity|Evidence|Proposed change|Rev 0.2 action|
|-|-|-|-|-|
||||||

\---

## 25\. Immediate next preparation before boards arrive

Before the PCB order arrives, prepare:

1. Printed schematic and net list.
2. Printed PCB top/bottom reference plots.
3. Bill of materials for headers, resistors and mounting hardware.
4. Known-good 10 kΩ through-hole resistors.
5. Header sockets/pins for NUCLEO, DWM3001C-CDK and XIAO.
6. Minimal STM32N6 pin-toggle firmware.
7. Minimal SPI5 test firmware.
8. Logic analyser channel plan.
9. Bring-up evidence folder.
10. Fault log template.

This will allow the first board to be inspected, assembled and powered systematically rather than improvised under bench pressure.

