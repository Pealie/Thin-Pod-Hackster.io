# Engineering Log addendum: KiCad carrier-board v0.2 planning (2026-01-15)

## Summary

Work this evening consolidated the carrier-board v0.2 electrical additions and translated them into KiCad-ready schematic building blocks, with explicit net naming and connector pin mappings aligned to the current bench bring-up.

## Key outcomes

- Confirmed the raw power entry update:
  - Battery input and an alternative bench supply header both land on `RAW_IN`.
  - A series Schottky diode (1N5817) is placed between `RAW_IN` and `RAW_PROT`.
  - The S7V8F3 `VIN` is fed from `RAW_PROT`, keeping `RAW_IN` available for true source-side measurement and sensing.

- Defined a high-side P-MOSFET power-gating option for near-zero sensor draw:
  - `3V3_A` feeds a P-MOSFET high-side switch to generate a switched rail `ACC_3V3_SW` for ADXL1005 `VDD`.
  - Gate network defined for deterministic default OFF state during CDK reset:
    - 100 k pull-up from `ACC_PFET_G` to `3V3_A`.
    - 100 Ω to 1 k series resistor from `CDK_GPIO_ACC_PWR` to `ACC_PFET_G`.
  - Local decoupling on `ACC_3V3_SW` specified (100 nF plus 1 µF to 10 µF bulk).

- Preserved the STANDBY control line as a separate control mechanism:
  - STANDBY remains available for reduced current without full power removal, with GPIO mapping retained as a defined net (`ACC_STANDBY`).

- Produced a complete KiCad-ready net and pin list for the two additions:
  - Block A: bench header and series Schottky on the raw input.
  - Block B: P-MOSFET high-side sensor rail gating.
  - Recommended test points defined for bring-up: `TP_RAW_IN`, `TP_RAW_PROT`, `TP_ACC_3V3_SW`, and optional `TP_ACC_PFET_G`.

- Saved the source-of-truth block description as a Markdown file for inclusion in the carrier-board v0.2 directory:
  - `thin-pod_kicad_blocks_raw_schottky_pfet.md`.

## Immediate next steps

- Insert the two blocks into the KiCad v0.2 schematic with the specified net labels and connector pin mappings.
- Assign initial footprints (THT DO-41 for 1N5817 if through-hole is intended, plus the chosen P-MOSFET package) and run ERC.
- Add the named test points to the schematic and place them in an accessible region in the first PCB placement pass.
