# Engineering Log Update: Thin-Pod bench bring-up (ADXL1005 to DWM3001-CDK)

Date: 2026-01-12

## Scope of work
Bench validation of Thin-Pod power integrity, ADXL1005 analogue output behaviour, and oscilloscope workflow on the FNIRSI 2C53T.

## Power integrity and distribution
- Regulated input confirmed stable at approximately 3.37 V.
- CDK 3V3 rail confirmed stable and closely matched the regulator output (millivolt-level difference previously observed).
- CDK pin 15 domain previously observed at ~1.788 V in-circuit; later tests clarified the measurement context (see ‘Signal path verification’).

## ADXL1005 control and mode confirmation
- STANDBY behaviour was inconsistent when left weakly defined. A DVM reading of ~0.192 V was observed once, then 0 V on a subsequent measurement, consistent with a high-impedance node.
- A 10 kΩ pull-down resistor was added between STANDBY and GND.
- Post-change, STANDBY consistently measures near 0 V (meter noise around zero, approximately -0.0004 V), confirming a solid logic-low default state.

## Wiring and pickup reduction
- Lead lengths were shortened to reduce loop area and pickup:
  - ADXL1005 Vout lead to the CDK was shortened.
  - 3.3 V supply leads to the ADXL1005 and CDK were shortened.
- A control test was performed with the probe tip placed on ground. The presence of a similar ~100 Hz pattern under this condition confirmed that earlier large ‘rail waveforms’ were dominated by probe-loop pickup rather than true rail ripple.
- After shortening leads and improving measurement technique, visible pickup reduced significantly, enabling smaller real signals to be observed.

## Signal path verification
- ADXL1005 was disconnected from CDK pin 15 for isolation testing.
  - With the sensor disconnected, CDK pin 15 measured ~0.001 V, consistent with a floating or high-impedance node when undriven.
  - With the sensor disconnected, ADXL1005 Vout measured ~1.78 V at rest, consistent with a midscale-biased analogue output at 3.3 V supply plus device and chain offset.
- Earlier readings near 1.78 V at CDK pin 15 were consistent with the sensor output driving the selected CDK pin rather than an internal fixed 1.8 V rail.

## Oscilloscope workflow and tap visibility
- FNIRSI 2C53T settings were stabilised for repeatable measurement:
  - Manual volts per division control confirmed.
  - DC coupling used for rail checks.
  - AC coupling used to centre small-signal motion around zero for tap visibility.
- Initial tap testing at 2 V per division and 200 ms timebase showed no discernible response, consistent with insufficient vertical sensitivity for tens-of-millivolts transients.
- After tightening vertical scale and using AC coupling on the sensor node, a clear waveform and transient spikes were observed when tapping, indicating mechanical excitation is reaching the ADXL1005 output and is measurable on the bench setup.


## ADXL1005 duty-cycling concept (fast wake)
STANDBY on the ADXL1005 is active high. Holding STANDBY low keeps the device in measure mode; driving STANDBY high places the device into standby for reduced current draw. Standby-to-measure wake is fast (tens of microseconds order), making STANDBY suitable for periodic sampling windows such as 15 minutes per hour.

A pull-down resistor on STANDBY remains the clean default, ensuring a deterministic logic-low state during CDK reset, boot, or GPIO high-impedance conditions.

### Simple wiring sketch (GPIO-controlled STANDBY)
```text
                DWM3001-CDK
            +------------------+
            |                  |
3V3 ------- +------------------+-----------------------> ADXL1005 VDD
GND ------- +------------------+-----------------------> ADXL1005 GND

GPIOx (push-pull) ----[ 100Ω..1k ]----+----------------> ADXL1005 STANDBY
                                      |
                                     [10k]
                                      |
GND ----------------------------------+----------------> ADXL1005 GND
```

- GPIO low drives STANDBY low (measure mode).
- GPIO high drives STANDBY high (standby mode).
- 10 kΩ pull-down provides a defined default low; the small series resistor is optional and can help with long leads and transients.


### Alternative option (hard off): high-side P-MOSFET power gating
STANDBY reduces current but does not fully power the device down. If near-zero sensor draw is required, a simple high-side P-MOSFET can switch ADXL1005 VDD on and off. A pull-up on the gate provides a defined default OFF state during CDK reset. The GPIO then pulls the gate low to turn the sensor ON.

#### Simple wiring sketch (GPIO-controlled P-MOSFET high-side switch)
```text
                         DWM3001-CDK
                     +------------------+
                     |                  |
3V3 (reg) -----------+------------------+-------------------+
GND -----------------+------------------+-------------------+-----------------> ADXL1005 GND
                                                         ___|___
                                                        |       |
                                                        | 10uF  |  (local decoupling)
                                                        |_______|
                                                            |
                                                            +-----------------> ADXL1005 VDD
                                                            |
3V3 (reg) --------------------+-----------------------------+
                              |
                             [100k]   (gate pull-up, default OFF)
                              |
GPIOy (push-pull) ----[100Ω..1k]----+-----G
                                    |
                               S ---+--- P-MOSFET --- D
                                    |
3V3 (reg) --------------------------+

Logic:
- GPIOy HIGH (≈3V3) -> gate pulled to source -> P-MOSFET OFF -> ADXL1005 unpowered.
- GPIOy LOW (0V)    -> Vgs negative -> P-MOSFET ON  -> ADXL1005 powered.
```

Notes:
- The gate pull-up sets a deterministic OFF state at boot.
- A small series resistor on the gate is optional and can reduce ringing on longer wires.
- If Vout is wired to an ADC pin, ensure the ADC pin remains high-impedance when the sensor is off to avoid back-powering through protection structures. A small series resistor in the Vout line can help if needed.


## CDK ADC node identification
- The analogue signal from the ADXL1005 was monitored at the CDK header ADC node: DWM3001C-CDK **J10.15**.
- J10.15 is routed to the DWM3001C module as **P0.28**, which corresponds to **SAADC AIN4** on the nRF52.
- Tap-correlated transients were observed on the oscilloscope with the probe tip placed at J10.15, supporting that the sensor output is reaching the intended ADC input node.

### Firmware implication (later step)
- A straightforward next validation is SAADC single-ended sampling of AIN4, verifying a steady mid-rail bias (~1.78 V) with superimposed tap spikes.
- A common SAADC configuration for a 0 to 3.3 V domain signal is internal reference with a gain setting that covers the full input swing (for example, gain 1/6 to cover up to ~3.6 V full-scale).


## Current status
- Power delivery is stable and repeatable.
- ADXL1005 is confirmed active (STANDBY held low) with a stable rest output near 1.78 V.
- Tap-correlated transients are now visible on the oscilloscope at appropriate scale and coupling, indicating the analogue sensing chain is behaving plausibly at the bench level.

## Next steps
- Capture a clean single-shot tap event with a millisecond timebase and edge trigger just above baseline noise.
- Verify the tap signature at both the ADXL1005 Vout pad and the intended DWM ADC input pin to confirm continuity and loading behaviour.
- Exercise ADXL1005 self-test (ST) if accessible to obtain a known step response for calibration and sanity checking of volts-to-g scaling.


## USB present versus battery feed at J1 (bench measurements)
The CDK was powered from an external regulated 3.3 V feed via J1 (+/−), with USB alternately applied to the J20 connector.

### Observations
- J1+ to GND (battery only): 3.372 V
- J1+ to GND (USB only, battery disconnected): 0.0010 V
- J1+ to GND (battery + USB on J20): 3.374 V

These measurements indicate no appreciable backfeed from USB onto the J1 battery node.

### Battery current on USB
A 1 Ω series shunt was inserted in the battery feed to J1+. The measured shunt voltage implies:

- Battery only: 0.0061 V to 0.04 V across 1 Ω, approximately 6.1 mA to 40 mA
- Battery + USB on J20: 0.006 V across 1 Ω, approximately 6 mA

USB therefore carries nearly all of the CDK load when present. A residual ~6 mA remains on the battery path, consistent with always-on draw in the external regulation chain and or leakage paths.

## Recommended fix (concrete): disable the external regulator on USB via SHDN
The Pololu S7V8F3 includes a shutdown input (SHDN). The board is enabled when SHDN is high, and can be powered down by pulling SHDN low. The S7V8F3 documentation notes an internal pull-up on SHDN to VIN (100 kΩ), so the default state is enabled unless SHDN is actively driven low.

The cleanest approach is to use USB VBUS presence to pull SHDN low, disabling the external regulator whenever either USB connector is plugged in. This removes the residual battery drain while USB is attached, without needing to modify the CDK power path.

### Simple wiring sketch (USB VBUS detect -> NPN -> S7V8F3 SHDN)
```text
DWM3001C-CDK (USB)                         External regulator (Pololu S7V8F3)
-------------------                       ------------------------------------
J9 VBUS (5V) ---|<|---+
                 D1    \
                        +---- USB_PRESENT ----[Rb]----B   Q1 (NPN, e.g. MMBT3904)
J20 VBUS (5V) --|<|----+                      |             C-----> SHDN
                 D2                           +----[Rpd]----+
                                              |             E-----> GND
                                             GND

S7V8F3:
VIN  <---- battery pack
GND  <---- battery minus
VOUT ----> CDK J1+ (3.3 V feed)
SHDN <---- from Q1 collector (pulled low when USB_PRESENT is high)

Logic:
- USB absent: Q1 off, SHDN pulled up internally to VIN, regulator enabled.
- USB present: USB_PRESENT high, Q1 on, SHDN pulled low, regulator disabled.
```

### Suggested parts and values (hand-solder friendly)
- D1, D2: Schottky diodes for diode-OR of VBUS, e.g. BAT54 (SOT-23) or SS14 (SMA) depending on preferred footprint.
- Q1: small NPN transistor, e.g. MMBT3904 (SOT-23) or BC817 (SOT-23).
- Rb: 47 kΩ to 220 kΩ (base resistor). Start at 100 kΩ.
- Rpd: 100 kΩ (base pulldown) to keep Q1 off when USB_PRESENT is floating.

Notes:
- If only one USB connector is ever used for laptop connection, USB_PRESENT can be taken from that single VBUS pin and the diode-OR can be omitted.
- If SHDN wiring is long, a small series resistor (100 Ω) close to the SHDN pin can reduce ringing.


## Pin allocation decisions for duty-cycling and regulator gating
### ADXL1005 STB (standby) control
ADXL1005 STB will be driven from the CDK Raspberry Pi header to enable duty-cycling via STB.

- ADXL1005 STB -> CDK J10.24 (labelled 'CS_RPI', maps to module GPIO P0.30)
- STB remains configured with a pull-down resistor to GND to guarantee a deterministic low during reset and GPIO high-impedance intervals. STB high enters standby. STB low enters measurement.

### S7V8F3 SHDN gating control
The external regulator shutdown will be controlled by the CDK using an NPN pull-down driver on SHDN.

- SHDN pull-down driver control -> CDK J10.23 (labelled 'SPI1_CLK', maps to module GPIO P0.31)
- The SHDN pin on S7V8F3 is held high by its internal pull-up to VIN when Q1 is off. Q1 on pulls SHDN low to disable the regulator.

## Reverse-polarity protection (biscuit board battery input)
A Schottky diode will be added in series with the battery pack positive rail on the biscuit board.

- Battery + -> Schottky diode -> S7V8F3 VIN and downstream loads
- Battery - -> GND (direct)

Notes:
- The Schottky forward drop reduces effective VIN. With a 3.3 V regulated output and a 4.5 V nominal battery pack, margin remains acceptable, but the drop should be accounted for in any low-battery cut-off criteria.
- Candidate parts for bench use include SS14 class parts, or smaller low-Vf Schottky devices matched to the expected current.


## Correction: SHDN gating from the CDK is not valid in the current topology
The current bench topology uses the S7V8F3 as the primary 3.3 V supply for the DWM3001C-CDK. If SHDN is asserted low to disable the regulator, the CDK loses power and can no longer hold SHDN low or later deassert it. This creates an inherent self-disable condition and can lead to unintended oscillation (regulator off -> CDK off -> SHDN released by loss of drive -> regulator on -> CDK boots).

Resolution:
- Do not drive S7V8F3 SHDN from a CDK GPIO in the current topology.
- Use ADXL1005 STB duty-cycling (via CDK J10.24) for power savings while keeping the 3.3 V rail enabled.
- If future work requires disabling the battery-fed regulator while USB is present, implement SHDN control using a hardware USB-present detect circuit (diode-OR from J9/J20 VBUS into an NPN pull-down), which does not depend on the CDK remaining powered.

Pin note:
- CDK J10.23 remains a candidate control line for a future SHDN driver only if an always-on controller or a non-CDK power domain exists to maintain control during shutdown.
