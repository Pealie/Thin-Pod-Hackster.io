# Engineering Log Addendum: CDK docking and module solder-down plan

Date: 2026-01-20 (21:30)

## Summary

* Clarified the practical integration paths for Qorvo UWB hardware:

  * Bring-up and iteration via docking the DWM3001CDK to a custom Thin-Pod PCB using headers and standoffs.
  * Next PCB revision via soldering down the DWM3001C castellated module using the vendor land pattern, antenna keep-out guidance, and the recommended solder profile.

* Confirmed that ADXL1005 dev-board control pins can be left unused for early bring-up:

  * ST (self test), STB (standby), and ORo (overrange) may be left not connected when not required.

* Identified alignment practices that minimise mechanical stress and assembly risk when docking the CDK:

  * Use the CDK as a temporary assembly jig when soldering the Thin-Pod J10 header footprint.
  * Fit standoffs so the header carries signal connectivity rather than structural load.
  * Prefer a harnessed power feed during early docking unless connector geometry is fully locked.

## Decisions captured

* CDK docking selected as the lowest-friction bring-up method.
* DWM3001C module solder-down deferred to the next Thin-Pod PCB spin after docked bring-up validates the end-to-end data path.
* Calipers identified as the preferred tool for capturing hole centres, connector offsets, and standoff geometry.

## Notes on header alignment techniques

* Jig soldering approach:

  * Insert unsoldered header into the Thin-Pod PCB footprint.
  * Mate the CDK onto the header to enforce correct pitch and orthogonality.
  * Tack corner pins, unmated inspection, then complete soldering.

* Mechanical support:

  * Standoffs used to set board-to-board spacing and preserve parallelism.
  * Header pins treated as electrical interconnect rather than mechanical structure.

* Geometry capture options for KiCad placement:

  * Caliper-driven datum measurements from a chosen J10 corner reference.
  * 1:1 print check of the Thin-Pod outline and hole pattern before fabrication.
  * Scan or photo overlay method as a supplementary cross-check where useful.

## Next actions

* Purchase calipers.
* Measure and record:

  * J10 location reference, any required board-to-board spacing, and candidate standoff positions.
  * Any relevant power-header geometry intended for the docking arrangement.

* Convert measurements into KiCad mechanical layers:

  * Header footprints, mounting holes, keep-outs for tall CDK components, and clearance notes.

## References

* Qorvo DWM3001C datasheet (module land pattern, solder profile, antenna keep-out guidance).
* Qorvo DWM3001CDK product brief (dockable CDK interfaces and bring-up context).
* Analog Devices ADXL1005 datasheet (control pin roles and optional usage during bring-up).
