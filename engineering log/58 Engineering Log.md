# 58 Engineering Log

## High-level overview

Work completed focused on de-risking the transition from the Qorvo DWM3001C development kit to a DWM3001C module mounted on a Thin-Pod-style carrier PCB, with emphasis on assembly yield, solder joint robustness, and RF keep-out discipline.

## Outputs produced

* Defined a practical soldering workflow for mounting the DWM3001C castellated module onto a new carrier PCB:

  * Preferred: stencil + paste + controlled reflow (oven), or hot plate with hot-air assist.
  * Prototype fallback: hand soldering with heavy flux and drag-soldering along castellated edges, followed by wick and reflow touch-up.

* Documented the key equipment required to achieve repeatable, inspection-grade assembly:

  * Hot air rework station and/or reflow oven or hot plate.
  * Fine-tip iron, solder wick, gel flux, tweezers, and good magnification.
  * ESD controls and fume extraction.

* Captured the design and process choices that improve mechanical reliability and solder yield:

  * Sensible stencil thickness and paste strategy with paste reduction if bridging is observed.
  * Conservative solder mask expansion and avoidance of solder wicking paths (for example via placement near castellations).
  * Post-reflow inspection of all castellations and continuity checks prior to first power.
  * Optional mechanical edge staking away from the antenna region when vibration survivability demands it.

* Captured RF robustness constraints relevant to the integrated antenna:

  * Enforced antenna keep-out on the carrier PCB, including no copper or metal in the defined region and avoidance of batteries or large conductors under the antenna.

## KiCad footprint deliverable

* Produced a ready-to-import KiCad footprint for the DWM3001C module:

  * File: 'Qorvo\_DWM3001C.kicad\_mod'
  * Pads numbered to match the module pin numbering scheme.
  * Included conservative defaults for solder mask margin and paste reduction to improve first-build yield, with the expectation of iteration after first assembly photos and inspection.

## Next steps

* Import the footprint into the Thin-Pod KiCad project library and instantiate it in the carrier PCB schematic and layout.
* Add placement constraints and board-level keep-outs for the antenna region and for any nearby conductive objects.
* Run first-build assembly using stencil + reflow (or hot plate + hot air), then capture microscope photos of all four castellated edges.
* Perform continuity checks (power rails and key IO) and a current-limited first power sequence before firmware bring-up.
