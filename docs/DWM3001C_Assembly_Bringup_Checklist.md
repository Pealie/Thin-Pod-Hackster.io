# DWM3001C Assembly and Bring-up Checklist
Date: 2026-01-31

An assembly-oriented checklist for integrating the **Qorvo DWM3001C** module onto a custom Thin-Pod PCB.

## Scope
- Footprint and layout verification before fabrication
- Stencil and paste assumptions for first build
- Antenna keep-out and metal-proximity review
- Assembly, inspection, and rework flow for a castellated module
- Bring-up sequence: power, SWD access, UART logging, first RF sanity checks

## Assumptions
- The PCB uses the latest vendor land pattern for DWM3001C and respects the antenna keep-out.
- The first build is a low-volume, bench-assembled prototype.
- A DWM3001-CDK remains available as a golden reference node for RF and firmware comparison.

## Tools and materials
### Assembly
- Hot air rework station or reflow capability (hot plate or small reflow oven)
- Fine tweezers
- Flux (no-clean gel preferred for rework)
- Solder paste (Type 4 baseline; Type 5 preferred where aperture definition is tight)
- Solder wick and fine solder (touch-up)
- Kapton tape (masking and part hold-down for rework)
- Magnification (loupe or microscope)
- IPA and lint-free swabs (cleaning)

### ESD and handling
- ESD mat and wrist strap
- Antistatic storage for modules
- Moisture sensitivity handling controls (sealed bag storage; bake procedure if required)

### Bring-up
- Bench supply with current limit
- DMM
- Oscilloscope (optional but useful)
- SWD debugger and appropriate cable/header
- USB to UART adapter (3.3 V logic)
- Known-good UWB peer (DWM3001-CDK or another proven node)

---

## 1. Footprint checks
### 1.1 Library and land pattern
- DWM3001C footprint verified against vendor land pattern: pad count, pad geometry, pitch, numbering, courtyard, fab outline.
- Pin 1 indicator consistent across: schematic symbol, footprint pad 1, silkscreen marker, assembly drawing.
- Solder mask expansion reviewed for castellated pads. Tight mask openings are avoided due to registration risk.
- Paste layer enabled for each castellated pad. No accidental paste suppression.

### 1.2 Mechanical alignment and placement
- Module outline matches vendor mechanical dimensions in the footprint and fab drawing.
- Courtyard clearance confirmed: adjacent parts do not block hot-air access to pad edges.
- Local fiducial(s) near the module placed (recommended even for hand assembly).
- Reference designator and pin 1 marker remain visible after placement.

### 1.3 Net mapping and access
- VDD and GND pins correctly mapped; power rail naming consistent with Thin-Pod power intent.
- SWD routed to accessible header or test pads: SWDIO, SWCLK, RESET, GND, VREF (target sense).
- UART routed to accessible header or test pads: TX, RX, GND.
- Reset access present (button or pad) for deterministic restart during bring-up.
- Any boot/mode strap pins (if used) are documented and reviewable.

---

## 2. Paste and stencil assumptions
Starting assumptions for first prototypes. Tune after inspection.

### 2.1 Stencil thickness
- 0.10 mm stainless stencil is the default for mixed 0402 plus module work.
- 0.12 mm is considered if the design is dominated by larger passives and connectors.

### 2.2 Aperture sizing for castellations
- Baseline: 1:1 aperture to pad for castellated pads.
- If bridging is observed:
  - Reduce aperture area to ~0.85 to 0.90 of pad area, centred on pad.
  - Consider a small paste pull-back from the PCB edge if pads are edge-adjacent.
- Excess paste is avoided to reduce the chance of wicking under the module and creating hidden shorts.

### 2.3 Paste handling
- Paste storage and warm-up time logged.
- Paste is not used cold directly from refrigeration.
- Paste age within manufacturer limits.

### 2.4 Optional aperture strategy
- For unusually large pads, window-pane apertures are considered to reduce voiding and pump-out.

---

## 3. Antenna keep-out and metal-proximity review
### 3.1 Keep-out geometry
- Antenna keep-out implemented on all copper layers:
  - No copper pour, no tracks, no vias in the keep-out region.
- Silkscreen and text kept clear if required by the vendor keep-out guidance.

### 3.2 Proximity to conductive objects
- Clearance maintained between antenna region and metal:
  - Enclosure metal, standoffs, screws, shields, batteries, and nearby cables.
- Any ground pour features, via stitching, or shields remain outside the keep-out envelope.

### 3.3 Board edge and placement
- Module placement consistent with vendor guidance regarding board-edge proximity and multipath sensitivity.
- No tall components placed in front of the antenna region within the keep-out envelope.

---

## 4. Assembly process
### 4.1 Pre-assembly preparation
- PCB inspected: solder mask alignment, pad quality, contamination, and damage.
- ESD controls active during handling and placement.
- Module moisture exposure window tracked; bake procedure applied if required.

### 4.2 Paste application and placement
- Paste applied with stencil and inspected:
  - Complete fill, no smearing, no missing apertures, no paste bridges.
- Module placed under magnification:
  - Outline aligned to fab outline
  - Castellations aligned to pad centres
  - Pin 1 orientation checked before release

### 4.3 Reflow or hot plate
- Reflow executed using a controlled profile aligned to the paste vendor and module guidance.
- Peak temperature and time above liquidus recorded where feasible.
- Module is not disturbed during cool-down.

### 4.4 Post-reflow inspection
- All castellated joints inspected under magnification:
  - Fillet shape, wetting, and evidence of opens or bridges
- Solder balls checked, especially near antenna region.
- Pre-power continuity checks:
  - VDD to GND resistance
  - Selected IO continuity from test pads to module pads (spot checks)

### 4.5 Rework strategy (if required)
- Flux applied and hot air used to reflow the affected edge line.
- Bridges:
  - Wick with flux, then reflow lightly to re-form fillets.
- Opens:
  - Reflow with flux; fine solder touch-up if needed.

---

## 5. Bring-up sequence
A staged bring-up localises failure modes and reduces risk.

### 5.1 Stage 0: Unpowered checks
- VDD to GND resistance measured and compared against known-good expectation.
- Visual check for bridges across castellations and near any fine-pitch passives.

Exit criteria:
- No obvious shorts or solder defects.
- VDD to GND resistance not indicative of a hard short.

### 5.2 Stage 1: First power application
- Bench supply configured with conservative current limit.
- Power applied to the module VDD rail only.
- Observations recorded:
  - Inrush and steady-state current
  - Any local heating at module or regulators
- Rail voltage verified at:
  - Regulator output
  - Module VDD pin (or nearest test pad)

Exit criteria:
- Current draw stable and within expected bounds.
- No thermal anomalies.

### 5.3 Stage 2: Reset and basic life signs
- Reset line toggled and waveform confirmed (scope optional).
- Early-life indicator checked if present:
  - LED, GPIO toggling, or UART banner.

Exit criteria:
- Reset behaves consistently.
- Boot behaviour reproducible.

### 5.4 Stage 3: SWD access validation
- SWD probe connected: SWDIO, SWCLK, RESET, GND, VREF.
- Probe target voltage recognised correctly.
- Non-destructive first action:
  - Read device ID or similar discovery action supported by the toolchain.

If SWD fails:
- SWD pin mapping and header orientation rechecked.
- Ground reference verified.
- Reset strap condition verified.
- SWD line integrity checked (shorts, opens).

Exit criteria:
- Stable SWD session established.
- Device identification succeeds.

### 5.5 Stage 4: Flash a minimal firmware image
- Minimal test image flashed that performs:
  - Safe clock initialisation
  - GPIO or LED toggle at a known rate
  - UART boot message output

Exit criteria:
- Program and verify pass.
- GPIO toggling observable.
- UART output deterministic.

### 5.6 Stage 5: UART logging baseline
- USB to UART adapter connected at 3.3 V logic levels:
  - TX and RX wiring verified (crossed correctly)
  - Common GND confirmed
- Terminal settings logged:
  - Baud rate, parity, stop bits, flow control state
- UART banner and periodic log messages captured.
- Reset issued and UART boot sequence captured again to confirm determinism.

If UART output is absent:
- TX/RX swap verified.
- Correct voltage domain verified.
- Reset observed while capturing logs.
- Alternate baud rates tested if firmware configuration uncertain.

Exit criteria:
- Repeatable UART boot log captured across multiple resets.

### 5.7 Stage 6: Peripheral sanity and timing baseline
- GPIO sanity:
  - Select a spare GPIO as a heartbeat pin and confirm toggling under scope or DMM (if low frequency).
- Timebase sanity:
  - If the firmware exposes a tick counter over UART, verify monotonic increment.
- Power stability:
  - Monitor VDD under activity for droop or ripple (scope optional).

Exit criteria:
- GPIO and timebase behave as expected.
- No rail instability under basic load.

### 5.8 Stage 7: First RF sanity checks
The goal is a minimal UWB link test and a quick isolation of RF versus firmware issues.

#### 5.8.1 Controlled test environment
- Initial tests kept simple:
  - Short range (1 to 3 m) line-of-sight
  - Minimal metal nearby
  - Both nodes at similar height
- DWM3001-CDK used as the reference peer running a known-good configuration.

#### 5.8.2 Configuration parity
- Configuration parameters matched between custom PCB node and CDK node:
  - Channel
  - PRF and preamble settings
  - Data rate
  - Antenna delays or calibration values if applicable in the stack
- Full configuration logged on UART at boot for traceability.

Exit criteria:
- Both ends report consistent RF configuration values.

#### 5.8.3 Link establishment test
- Simple message exchange or ping executed:
  - Transmission from node A, acknowledgement from node B
- Outcomes logged:
  - Packet counts sent and received
  - Error counts
  - Any reported diagnostics (RSSI-like metrics, CIR quality, timestamps)

Exit criteria:
- Repeatable bidirectional exchange with low error rate at short range.

#### 5.8.4 Range sanity check (coarse)
- If the stack supports ranging, a coarse two-point sanity check performed:
  - Close distance: ~1 m
  - Further distance: ~3 m
- Measured distances compared against tape-measured ground truth.
- Behaviour and bias compared against the CDK baseline.

Exit criteria:
- Measured distance increases with physical separation.
- Large discontinuities or instability are absent.

#### 5.8.5 Orientation and keep-out sensitivity
- Node rotated through multiple orientations while distance is held fixed.
- RSSI-like metrics, packet error rate, and range stability observed.
- If performance collapses in specific orientations, re-check:
  - Antenna keep-out
  - Nearby copper features
  - Nearby cables, battery placement, or metal fasteners

Exit criteria:
- Performance varies within expected bounds, without catastrophic nulls.

---

## 6. Fault isolation guide
### 6.1 Power faults
- Symptom: high current draw at Stage 1
  - Likely causes: solder bridge under module, incorrect regulator, reversed rail, paste smear
  - Action: remove power, inspect under magnification, reflow suspect edges with flux, re-check VDD to GND resistance

### 6.2 SWD faults
- Symptom: no target detection
  - Likely causes: header orientation error, mis-mapped pins, missing VREF sense, RESET held asserted, solder open on SWD pads
  - Action: continuity check from header to module pads, verify VREF, verify reset, verify SWDIO and SWCLK idle levels

### 6.3 UART faults
- Symptom: silence or garbled output
  - Likely causes: TX/RX swapped, wrong baud, ground missing, voltage level mismatch, firmware not initialising UART
  - Action: verify wiring, confirm 3.3 V logic, capture during reset, verify firmware UART init path

### 6.4 RF faults
- Symptom: stable firmware but no link
  - Likely causes: antenna keep-out violated, metal too close, configuration mismatch, incorrect antenna delay calibration, damaged RF path
  - Action: revert to shortest-range line-of-sight test, remove nearby metal, match config with CDK, compare diagnostic counters to CDK

---

## 7. Evidence artefacts to capture for the Engineering Log
- Photos:
  - Pre-reflow paste print
  - Post-reflow module edges (all sides under magnification)
  - Bring-up wiring (SWD and UART)
- Measurements:
  - VDD to GND resistance pre-power
  - First power current trace (inrush and steady-state)
  - Rail voltage at module VDD during idle and activity
- Logs:
  - UART boot banner and configuration dump
  - SWD identification success screenshot or text output
  - RF link test counters and any quality metrics
- Comparison:
  - Side-by-side CDK versus custom PCB performance at 1 m and 3 m line-of-sight
