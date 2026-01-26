# Thin-Pod Engineering Log — Evening Summary (2026-01-26)

## Summary of progress
- Duty-cycling approach for the ADXL1005 supply was clarified and implemented using a high-side PFET (ZVP2106A) to create the switched rail `ACC_3V3_SW`.
- Default power-up behaviour for the ADXL1005 was changed to **default ON** by configuring the PFET gate bias as a **pull-down** (R2 to GND), while preserving firmware control to force **OFF** by driving the gate high.
- KiCad net highlighting was used to validate correct separation of the PFET **Source**, **Gate**, and **Drain** nets, confirming correct switch topology and removing earlier ambiguity.
- A CDK constraint was confirmed: **J10 pin 16 is NC on the DWM3001C‑CDK**, so it cannot be used for PFET gate control.
- The schematic was updated to use a confirmed connected CDK header pin for PFET control:
  - **J10 pin 24 (`CS_RPI`) mapped to module P0.30** selected as the PFET gate drive control.
  - **J10 pin 15 (`GPIO_RPI`) mapped to module P0.28** retained as the ADXL1005 analogue output ADC input.
- Net naming and intent were tightened to reflect physical mapping and firmware responsibilities (e.g., `J10.15_ADC_input_P0.28`, `J10.24_CS_RPI_P0.30`).

## Intended behaviour
- PFET switch defaults **ON** at reset (gate pulled low via R2).
- Firmware control uses **P0.30**:
  - Drive **HIGH** to force PFET **OFF** for duty-cycling.
  - Drive **LOW** (or allow Hi‑Z) for PFET **ON**.

## Next actions
- Confirm P0.30 firmware configuration as a push-pull output for guaranteed OFF state.
- Document active-high ‘OFF’ logic in schematic notes and re-run ERC.
- Continue PCB Editor stage with test points for `ACC_3V3_SW`, the PFET gate control net, and the ADC node.
