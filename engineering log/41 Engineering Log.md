## Engineering Log update

### Summary

Work today converted Thin-Pod’s breadboard bench setup into an actionable ‘Option B’ carrier-board plan, with a concrete schematic sheet structure and a settled power and signal strategy for the DWM3001-CDK and ADXL1005.

### Key decisions and outputs

* Selected **Option B carrier board** approach (sensor front-end carrier), focusing on analogue integrity and repeatability while keeping the DWM3001-CDK as the core compute/radio board.
* Defined a **concrete KiCad hierarchical schematic structure** for the carrier board:

  * `power.kicad_sch`
  * `sensor_frontend.kicad_sch`
  * `dwm3001_cdk_header.kicad_sch`
* Locked down the **Thin-Pod analogue signal path** to match the current bench wiring:

  * ADXL1005 `Vout` → RC low-pass (**8 kΩ series**, **680 pF to GND**) → DWM3001-CDK **J10 pin 15** (ADC input). 
* Resolved the **power-entry confusion** and confirmed the practical power plan:

  * The DWM3001-CDK does not expose 3.3 V on ‘Raspberry Pi’ J10 pins 1/17 (NC on this board), so CDK power should not be attempted via those pins.
  * Confirmed the CDK has **bottom ‘+ / −’ battery/external supply pads** and will be powered via a **two-wire flying lead** to those pads.
* Finalised the desired carrier-board power architecture:

  * **JST-PH2 in → S7V8F3 buck → regulated 3.3 V (`3V3_SYS`) → split to:**

    * `3V3_A` → ADXL1005 VDD (with local decoupling and optional isolation jumper)
    * `3V3_CDK` → CDK `+ / −` pads (with an enable/isolation jumper to avoid back-powering during USB flashing)
* Collected supporting documentation for the CDK power paths and interfaces (product brief and quick start guide), and updated the Thin-Pod ‘bench-ready pinouts’ reference file to include Thin-Pod detail.

### Artefacts produced

* Multiple **Markdown ‘schematic skeleton’ packs** were produced and iterated, culminating in the v0.1 pack that implements the final desired architecture:

  * `thin-pod-carrier-v0.1.md`, `power.md`, `sensor_frontend.md`, `dwm3001_cdk_header.md`, plus a short `README.md` describing the power split and caveats.

### Notes and caveats

* Feeding **3.3 V into the CDK battery pads** is treated as a **bench-validated assumption**, since the product brief indicates VBAT feeds an on-board DC-DC but does not publish the full VBAT range. If unstable, the fallback is to power the CDK via its supported 5 V entry (USB or J10 5V0) while keeping S7V8F3 3.3 V for the sensor rail. 
