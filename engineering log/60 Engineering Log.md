# 60 Engineering Log

## Summary

Work completed today progressed the DWM3001C integration from schematic-level intent to practical KiCad library, footprint, and 3D model infrastructure that is suitable for repeatable PCB work. A clean project-relative library pathing approach was established to eliminate ambiguity caused by duplicate folders. A higher-fidelity 3D representation was iterated to provide clear orientation cues, including an antenna-end marker with an oval window.

In parallel, a TinyML direction was captured at a high level, framing how Thin-Pod sensing can evolve from raw acquisition to on-node inference through a minimal, testable feature pipeline and a small classifier suitable for embedded constraints.

## DWM3001C KiCad integration

- Produced and used a KiCad 9 compatible DWM3001C footprint (48 pads, numbering aligned to the datasheet pin diagram).
- Validated footprint orientation variants, including the ‘flipy’ version, for placement alignment with the intended antenna keepout.
- Diagnosed the PCB Editor update error:
  - ‘Updated footprint … *** library footprint not found ***’
  - Root cause: missing or mis-pointed project footprint library nickname and path.
- Established a project-specific footprint library entry:
  - Nickname: `ThinPod`
  - Path: `${KIPRJMOD}/footprints/ThinPod.pretty`
  - Outcome: predictable resolution of `ThinPod:<footprint>` IDs and removal of path ambiguity from duplicate similarly named folders.
- Confirmed footprint internal naming consistency within the `.kicad_mod` file:
  - `(footprint "Qorvo_DWM3001C_kicad9_flipy" …)`
- Linked 3D models via project-relative paths under `${KIPRJMOD}` and corrected scaling strategy:
  - mm-based WRL models use scale `1,1,1`
  - inch-derived scaling factors were identified as the cause of the ‘tiny dot’ effect.

## 3D model realism iterations

The DWM3001C 3D model was iterated to increase visual realism and to remove ambiguity around the antenna end.

- Baseline model: board substrate plus simplified shield-can block and label.
- Patch visibility investigation:
  - Antenna patch not visible in typical viewing angles, likely due to primitive orientation and edge-on presentation.
- Iteration v3:
  - Introduced a high-visibility raised ‘antenna patch’ marker to verify orientation and visibility during placement review.
- Iteration v4:
  - Replaced the high-visibility block with a slightly raised oval window marker, closer to the module’s antenna-window appearance in reference photography.

Key outcome: a 3D model that supports fast visual checks of orientation and antenna-end placement in the absence of a full PCB.

## TinyML progress

- Captured a high-level plan for adding TinyML capability to Thin-Pod as a later stage once the hardware platform is stable.
- Framed an evidence-driven path from sensing to inference:
  - Start with a minimal feature set (time-domain and simple spectral features) derived from accelerometer data.
  - Establish a small labelled dataset aligned to target machine states and mounting conditions.
  - Evaluate small-footprint classifiers suitable for microcontrollers, with an emphasis on reproducibility and measurable uplift over threshold-based rules.
- Identified integration intent:
  - On-node inference for event detection or coarse classification, with UWB backhaul used primarily for summaries, features, and flagged segments rather than continuous raw streaming.

## Notes and next steps

- Confirm the canonical project folder structure under `${KIPRJMOD}` for footprints and 3D models and remove or ignore duplicate external library folders.
- Ensure the DWM3001C footprint in the PCB references the project library ID (`ThinPod:Qorvo_DWM3001C_kicad9_flipy`) and that 3D model links are the intended v4 WRL.
- When PCB layout begins:
  - Apply the antenna copper keepout and mechanical courtyard guidance.
  - Add clear silkscreen orientation marks on the carrier PCB to match the 3D orientation cues.
- For TinyML:
  - Define the first measurable use case and a ‘minimum viable dataset’ to avoid model work drifting ahead of evidence.

