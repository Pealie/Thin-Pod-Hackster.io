# 68 Engineering Log.md

**Log date:** 02 May 2026

## High-level summary

Today’s work moved the Thin-Pod Gateway rev 0.1 KiCad design from loose board-module planning into a more coherent schematic/PCB integration state. The main achievement was consolidating the gateway around a clearer carrier-board architecture: the NUCLEO-N657X0-Q, the Seeed Studio XIAO ESP32-C6 DIP/header module, and the DWM3001C-CDK are now represented in the KiCad project with project-local symbols/footprints and placed on the developing PCB layout.

A key design clean-up was the decision to move the NUCLEO-side wiring away from the Arduino/Zio headers and onto the ST morpho headers. The SPI and control connections previously described through CN14, CN13 and CN5 were consolidated onto CN15 and CN3 only. CN15 now carries the shared SPI5 bus and most control signals, while CN3 carries power, ground and the XIAO ESP32-C6 enable/reset signal. This produces a cleaner carrier-board wiring strategy and avoids mixing Arduino-style and morpho-header connection schemes on the same test PCB.

The Seeed Studio XIAO ESP32-C6 DIP/header footprint and symbol were sourced/generated and added to the KiCad project. The DIP/header variant remains the preferred choice for this first gateway PCB because it supports removable bring-up, easier probing, and lower rework risk than soldering the XIAO module directly with a castellated SMD footprint.

A NUCLEO-N657X0-Q symbol was also added to the schematic. The first version is physically large, as expected for a Nucleo-144-style connector symbol, but it is now usable in the schematic as a reference point for the gateway carrier design. The schematic now includes the NUCLEO symbol, the XIAO ESP32-C6 DIP/header symbol, and a documentation block summarising the revised one-page CN3/CN15 wiring plan.

On the PCB side, the major footprints are now present and positioned inside the board outline. The 3D Viewer now renders the board successfully, showing the carrier board, module footprints, header holes, mounting holes, outline graphics, and visible front silkscreen labelling. Earlier silkscreen issues were resolved: the board text was moved to the correct silkscreen layer, un-mirrored, enlarged, and confirmed visible in 3D View.

The board currently functions as a mechanical and connectivity planning layout rather than a completed routed PCB. The key value of today’s work is that the gateway design now has a coherent physical structure, local library assets, visible schematic documentation, and a cleaner NUCLEO header strategy to route from.

## Main achievements

* Established a clearer Thin-Pod Gateway rev 0.1 KiCad structure using project-local assets.
* Added and verified a Seeed Studio XIAO ESP32-C6 DIP/header footprint for removable module bring-up.
* Added a corresponding XIAO ESP32-C6 DIP/header schematic symbol.
* Added a NUCLEO-N657X0-Q schematic symbol and associated Nucleo-144-style footprint approach.
* Consolidated the NUCLEO wiring strategy onto CN3 and CN15 only.
* Moved SPI/control planning away from CN14 and power planning away from CN5.
* Assigned CN15 as the main signal header for SPI5 and device-control nets.
* Assigned CN3 as the main power/ground/auxiliary header.
* Added a one-page schematic documentation block describing the revised CN3/CN15 wiring.
* Corrected the board silkscreen label so it is no longer mirrored and now appears correctly in the PCB editor and 3D Viewer.
* Confirmed that the 3D board render now shows the placed modules and board label cleanly.

## Revised gateway connection intent

The preferred NUCLEO-side connection strategy is now:

|Purpose|NUCLEO header|
|-|-|
|Shared SPI5 SCK/MISO/MOSI|CN15|
|DWM3001C-CDK chip-select, IRQ and reset|CN15|
|XIAO ESP32-C6 chip-select and interrupt|CN15|
|XIAO ESP32-C6 enable/reset|CN3|
|DWM3001C-CDK 5 V power|CN3|
|XIAO ESP32-C6 3.3 V power|CN3|
|Ground returns|CN3 and/or CN15|

The practical routing intent is therefore:

* CN15 carries the shared SPI5 bus and almost all logic/control signals.
* CN3 carries 5 V, 3.3 V, ground and XIAO enable/reset.
* CN5, CN13 and CN14 are no longer part of the preferred gateway carrier wiring plan.

## Notes and caveats

The current schematic documentation block is a design note, not a substitute for electrical connectivity. The next schematic step is to convert the documented net plan into real KiCad wires or net labels between the NUCLEO, DWM3001C-CDK and XIAO ESP32-C6 symbols.

The XIAO ESP32-C6 DIP/header approach remains best for rev 0.1 because it keeps the communications module removable during bring-up. A later rev 0.2 board may reconsider the castellated SMD footprint once the pinout, firmware workflow and power behaviour have been validated.

Care is still required around XIAO power. During bring-up, avoid unintentionally powering the XIAO from both USB-C and the NUCLEO 3.3 V rail unless the back-feed behaviour has been checked.

DWM reset should remain on an ordinary controllable GPIO, not on the NUCLEO NRST line. This preserves independent firmware control of the DWM3001C-CDK reset signal. Also, consider weak pull-ups, around 10 kΩ to 3V3, on both CS lines so neither peripheral is accidentally selected during reset.

## Next steps

1. Convert the CN3/CN15 one-page wiring note into actual schematic connectivity using named nets:

   * SPI5\_SCK
   * SPI5\_MISO
   * SPI5\_MOSI
   * DWM\_CS
   * DWM\_IRQ
   * DWM\_RESET
   * C6\_CS
   * C6\_INT
   * C6\_EN\_RST
   * 5V\_DWM
   * 3V3\_C6
   * GND
2. Verify every NUCLEO symbol pin against the NUCLEO-N657X0-Q/MB1940 connector tables before routing.
3. Assign and confirm footprints for all schematic symbols, especially:

   * NUCLEO-N657X0-Q
   * XIAO-ESP32-C6-DIP
   * DWM3001C-CDK or equivalent header/module representation
4. Run **Update PCB from Schematic** after the schematic nets are real.
5. Inspect the PCB ratsnest and confirm that the expected airwires appear for the shared SPI bus, chip-select lines, reset/interrupt lines, power and ground.
6. Add pull-ups to the two chip-select nets if required, probably around 10 kΩ to 3.3 V, so neither SPI peripheral is accidentally selected during reset.
7. Check power-entry assumptions:

   * DWM3001C-CDK powered from 5 V
   * XIAO ESP32-C6 powered from 3.3 V
   * no accidental XIAO USB/back-feed conflict during programming
8. Route the PCB in stages:

   * power and ground first
   * shared SPI bus next
   * chip-select, reset and interrupt lines last
   * keep labels and test points accessible for bench probing
9. Run ERC and DRC after routing.
10. Re-open the PCB in 3D Viewer and confirm:

    * board outline is valid
    * modules fit mechanically
    * silkscreen is readable
    * USB access and header clearance are practical
    * no obvious mechanical clashes exist
11. Once schematic and PCB checks are clean, regenerate the manufacturing outputs:

    * Gerbers
    * Excellon drill files
    * board outline
    * optional position/BOM files if needed
12. Add the final CN3/CN15 pinout and board screenshots to the project documentation before fabrication.

