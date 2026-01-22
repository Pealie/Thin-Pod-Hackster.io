# Engineering Log: evening summary

Date: 22 January 2026

## Summary
This session consolidated the ‘compute and radio’ backbone of Thin-Pod by documenting two key platforms and clarifying how each fits the system architecture, with a focus on TinyML feasibility, data-path roles, and practical integration patterns.

## Outputs produced
### 1) STM32 NUCLEO-N657X0-Q (STM32N657, STM32N6 series)
- Captured a specification-led overview of the NUCLEO-N657X0-Q platform, focusing on MCU compute, memory, I/O, and board-level features that affect real deployments.
- Mapped suitability for TinyML and neural networks, emphasising the Cortex-M55 plus Helium DSP path and the on-device NPU acceleration path for inference workflows.
- Positioned the NUCLEO-N657X0-Q as a natural **Gateway-class** Thin-Pod compute node, suitable for heavier DSP, inference, logging, and upstream connectivity.

Artefact: `STM32_NUCLEO_N657X0_Q_Specs_TinyML_ThinPod.md`

### 2) Qorvo DWM3001-CDK (DWM3001C module)
- Documented the DWM3001-CDK as a low-friction prototyping platform for UWB ranging and packet transport.
- Identified best-fit Thin-Pod roles: early node prototyping and UWB endpoint duties, with an emphasis on power profiling, timing discipline, and RF-risk reduction via a certified module.
- Clarified where the platform is strong (ranging, link bring-up, power iteration) and where constraints sit (on-node compute ceiling for heavier models, burst current requirements).
- Outlined practical Thin-Pod integration patterns, including ‘feature-first node’, ‘triggered capture node’, and ‘localisation plus telemetry’, alongside a bench bring-up sequence that supports iterative validation.

Artefact: `Qorvo_DWM3001_CDK_Suitability_ThinPod.md`

## Architectural decisions clarified
- Gateway compute is the appropriate locus for heavier inference and fleet-level decision logic, while the UWB node benefits from a feature-first telemetry strategy.
- Raw streaming over UWB remains a conditional mode best used for triggered windows or sparse captures, with routine operation favouring on-node feature extraction plus event-driven snippets.

## Files generated (paths in this workspace)
- `sandbox:/mnt/data/STM32_NUCLEO_N657X0_Q_Specs_TinyML_ThinPod.md`
- `sandbox:/mnt/data/Qorvo_DWM3001_CDK_Suitability_ThinPod.md`
