# Thin-Pod Architecture Note
## Option A: 'Split UWB Node + STM32N6 Gateway'
## Option B: 'Smart PSOC Edge Node + Wireless Backhaul'

**Status:** Draft architecture note  
**Date:** 2026-03-12  
**Project stance:** Open-source hardware first

---

## 1. Design intent

Thin-Pod is presently strongest as an open, inspectable, field-oriented sensing system rather than as a closed appliance. The architecture should therefore privilege:

- clear board-level ownership
- reproducible hardware and firmware
- modest node complexity
- a security model that is strong without surrendering too much architectural control
- a clean path from raw vibration data to deployed condition monitoring

Two viable forward paths now exist:

- **Option A:** keep the existing system bias, with a lean UWB-linked sensing node and a more capable STM32N6 gateway
- **Option B:** explore a future redesign in which the node itself becomes far more autonomous, using Infineon PSOC Edge E84 class silicon for local inference and a simpler wireless backhaul

This note treats **Option A as the baseline architecture** and **Option B as the research branch**.

---

## 2. Executive decision

### Recommended baseline

**Adopt Option A as the main Thin-Pod architecture.**

Reason:

Option A is more aligned with the project's open-source hardware character. It keeps the sensing node understandable, auditable and physically focused on vibration capture, timing and radio transport, while assigning heavier AI, update orchestration and network services to a gateway platform better suited to that role.

### Recommended R&D branch

**Develop Option B as a future experimental branch rather than a replacement.**

Reason:

PSOC Edge E84 is an unusually interesting smart-node MCU family for future Thin-Pod work because it brings strong low-power multi-domain design, substantial on-chip AI capability, analogue resources, and a polished embedded ML workflow. However, its security and ownership model is more vendor-mediated than ideal for an open-source hardware flagship, and the evaluation-board path is not a neat drop-in successor to the present ADXL1005 plus DWM3001C direction.

---

## 3. Architectural goals

The architecture should satisfy the following:

1. **Accurate vibration acquisition** from a serious analogue front end, including the present ADXL1005 direction or an equivalent future path.
2. **Open hardware legibility** so that board files, firmware stages, interfaces and power rails remain explainable and publishable.
3. **Predictable field power behaviour** with aggressive duty-cycling and bounded sleep currents.
4. **Secure boot and update discipline** appropriate for unattended industrial nodes.
5. **A clean AI workflow** from captured data to deployable inference artifacts.
6. **Scalable gateway behaviour** for logging, model delivery, fleet policy and network integration.

---

## 4. Option A: 'Split UWB Node + STM32N6 Gateway'

## 4.1 Concept

This option preserves the present Thin-Pod philosophy.

A small, specialised field node performs sensing, timestamping, local buffering, thresholding or lightweight features, and packet transport over UWB. A separate STM32N6 gateway receives data, aggregates multiple nodes, performs richer DSP or ML tasks, manages firmware distribution, and bridges onward to Ethernet, USB or other plant-side interfaces.

## 4.2 System shape

### Field node

- vibration-first design
- ADXL1005 or equivalent high-bandwidth analogue sensing path
- external UWB module such as DWM3001C
- strict power gating and sparse wake behaviour
- small trusted code base
- event capture, buffering, feature extraction if needed, then transmit

### Gateway

- STM32N6-based board as concentration and inference point
- wired backhaul preferred where practical
- Ethernet as the default plant-side network interface
- local model execution, logging, update coordination and fleet control

## 4.3 Why STM32N6 fits the gateway role

STM32N6 is better treated as a gateway-class embedded compute target than as the first choice for a tiny field node.

The family is attractive because it combines:

- Cortex-M55 class application compute
- Neural-ART NPU acceleration
- strong STM32 ecosystem support
- mature evaluation hardware with Ethernet, USB-C, ST morpho and expansion
- a path to secure boot and secure firmware update with owner-provisioned keys

This suits a gateway that can afford more memory, more power draw, more complex boot flow and richer connectivity.

## 4.4 Boot and update model

### Node

- immutable or highly stable stage-0 boot block
- signed application images
- A/B or single-image plus validated recovery, depending on node flash budget
- gateway-distributed updates
- rollback rules based on monotonic versioning and signed manifests

### Gateway

- STM32N6 OEMuRoT-based secure boot chain
- owner-provisioned signing keys in OTP
- encrypted external memory regions where required
- signed gateway images and, if useful, signed model bundles
- gateway acts as update authority for nodes

## 4.5 AI and signal-processing path

This option favours a layered workflow:

- raw or lightly processed vibration data captured at node level
- feature extraction can happen either at node or gateway
- richer inference, comparison across fleets, reclassification and model management happen at gateway level
- model training remains host-side, with deployment targeted chiefly at the gateway first

That arrangement is attractive because it avoids forcing early model assumptions into the node hardware.

## 4.6 Open-source hardware alignment

This is the stronger OSH option.

The node remains understandable and publishable as an instrumented open hardware sensing device. The gateway can still be complex, but its complexity is physically separated from the sensor head. That separation matters, because it makes the node easier to document, reproduce and revise without entangling every board spin with a full AI appliance stack.

## 4.7 Principal strengths

- best match to current Thin-Pod direction
- easiest architecture to explain publicly
- preserves UWB design intent
- keeps field-node power and complexity under tighter control
- gateway can evolve independently from nodes
- strongest fit for an OSH-first flagship project

## 4.8 Principal risks

- two-device architecture increases system integration work
- gateway boot and update design around STM32N6 is non-trivial
- some AI use cases may tempt feature creep into the node anyway
- overall BOM and deployment complexity may be higher than a single smart-node concept

---

## 5. Option B: 'Smart PSOC Edge Node + Wireless Backhaul'

## 5.1 Concept

This option rethinks Thin-Pod as a much more autonomous edge node.

The node performs a larger share of feature extraction and classification locally and then reports events, summaries or confidence-ranked states over a simpler wireless path. The radio layer would likely move toward Wi-Fi, Bluetooth or another low-integration-friction backhaul rather than depending on a UWB-first topology.

## 5.2 System shape

### Smart node

- PSOC Edge E84 class MCU on a future custom board
- local analogue acquisition and signal conditioning
- local feature extraction and embedded inference
- aggressive use of low-power and hibernate states
- event-driven reporting rather than streaming-first behaviour

### Backhaul side

- wireless uplink to an access point, bridge or simpler concentrator
- optional thin gateway for aggregation, logging and remote management
- gateway role becomes lighter than in Option A

## 5.3 Why PSOC Edge E84 is interesting here

PSOC Edge E84 is not compelling as a direct replacement dev board for the current Thin-Pod, but it is highly compelling as the basis for a future smart-node custom design.

Reasons include:

- dual-domain architecture with Cortex-M55 plus Ethos-U55 NPU and a lower-power Cortex-M33 side
- integrated analogue resources including ADC, op-amps, PGA and comparators
- published low-power modes that are credible for sparse wake designs
- built-in secure enclave and protected root of trust
- DEEPCRAFT plus ModusToolbox workflow that is unusually well matched to time-series and sensor-led AI development

## 5.4 Boot and update model

### Node

- Infineon Edge Protect style secure chain of trust
- signed images, validated slots and upgrade staging
- overwrite or swap-based upgrade model
- downgrade prevention using versioning and security counters
- OTA-friendly deployment for fielded smart nodes

### Infrastructure side

- lighter concentrator role than Option A
- policy server can remain off-board or cloud-side
- optional thin gateway can cache updates and collect diagnostics

## 5.5 AI workflow

This option is attractive where the node itself becomes the first-class AI target.

The likely flow is:

- capture vibration and environmental traces from the real device
- train and iterate using a time-series oriented workflow
- deploy a compact node-resident classifier or anomaly detector
- transmit states, exceptions, scores and selected waveforms rather than continuous raw streams

This could yield a more elegant field device in the long run, especially where bandwidth and gateway complexity need to be reduced.

## 5.6 Open-source hardware alignment

This is the weaker OSH option in governance terms, even though it can still support open board design around the silicon.

The issue is not that the silicon is unusable for open hardware. The issue is that the deepest trust machinery is more vendor-mediated. Ownership transfer, OEM certificates, Infineon-signed elements within the secure architecture, and the special status of Secure Enclave RAM applications all make the root-of-trust story less self-determined than an ideal OSH flagship might prefer.

That does not disqualify the option. It simply changes its character. It becomes an open board around a more opinionated vendor security substrate.

## 5.7 Principal strengths

- strongest future smart-node candidate presently seen
- good fit for always-on local classification
- better low-power posture for autonomous node work than STM32N6 gateway silicon
- polished ML workflow for time-series and sensor-led development
- can reduce dependence on a heavy gateway over time

## 5.8 Principal risks

- not a direct successor to the present UWB node concept
- current evaluation-board path is not a neat fit for ADXL1005 plus DWM3001C style work
- security ownership model is less aligned with OSH purity
- radio strategy would need rethinking
- larger node responsibility increases pressure on field firmware correctness

---

## 6. Side-by-side judgement

| Dimension | Option A: Split UWB Node + STM32N6 Gateway | Option B: Smart PSOC Edge Node + Wireless Backhaul |
|---|---|---|
| Fit to current Thin-Pod | Strong | Moderate |
| Fit to OSH-first posture | Stronger | Moderate |
| UWB compatibility | Native to concept | Weak unless added externally |
| Gateway strength | Strong | Moderate |
| Smart-node potential | Moderate | Strong |
| Power posture for node | Strong if node kept lean | Strong if custom board is disciplined |
| Vendor control over root of trust | Lower in system framing | Higher in security framing |
| AI at node | Limited to moderate | Strong |
| Near-term execution risk | Lower | Higher |
| Long-term research interest | High | Very high |

---

## 7. Recommendation

## 7.1 Project baseline

**Proceed with Option A as the official Thin-Pod architecture.**

This should be the branch used for:

- public documentation
- OSH-oriented board releases
- engineering logs
- gateway software definition
- UWB and vibration acquisition integration
- first fieldable demonstrator

## 7.2 Research branch

**Maintain Option B as a named R&D path: 'Thin-Pod Smart Node Branch'.**

This branch should be used for:

- evaluating what percentage of DSP or ML should migrate from gateway to node
- testing low-power autonomous classification
- examining whether future Thin-Pod variants should move from streaming to event-led reporting
- exploring whether a more autonomous product line could coexist alongside the flagship OSH architecture

---

## 8. Immediate engineering actions

### For Option A

1. Freeze the node versus gateway responsibility split in writing.
2. Define the node packet contract for raw samples, features, timestamps, health and power telemetry.
3. Write the STM32N6 gateway boot and update note with named stages, memory regions, signing flow and rollback rules.
4. Decide how much inference is mandatory at gateway v1.
5. Keep node-side software deliberately small.

### For Option B

1. Treat PSOC Edge as a future custom-board study, not as a direct evaluation-board substitution.
2. Draft a smart-node power budget around sparse sampling and hibernate behaviour.
3. Define a local-inference use case narrow enough to test honestly, for example anomaly detection or state classification on a motor class already in scope.
4. Write down the open-hardware implications of the Infineon trust chain before any major commitment.

---

## 9. Final position

At this stage the cleanest architectural stance is simple:

**Thin-Pod should remain a split system.**

The **lean UWB sensing node plus STM32N6 gateway** model is the best fit for the current project identity, especially because the project is strongly slanted toward open-source hardware, explicit design ownership and publishable engineering.

At the same time, **PSOC Edge E84 deserves serious attention as a future smart-node MCU family**. It is not the right replacement for the present architecture, but it is a credible foundation for a later branch in which Thin-Pod becomes more autonomous, more event-driven and more inference-capable at the edge.

That is therefore the recommended strategic split:

- **Option A:** product architecture and flagship OSH path
- **Option B:** advanced R&D path for future smart-node evolution

---

## 10. Source basis

This note is grounded in recent official vendor documentation for:

- Infineon PSOC Edge E84 family and security material
- Infineon Edge Protect Bootloader and DEEPCRAFT Studio material
- ST STM32N6 family, NUCLEO-N657X0-Q board material, and STM32N6 security and AI tooling material
