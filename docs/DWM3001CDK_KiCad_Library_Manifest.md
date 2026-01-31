# DWM3001CDK KiCad Library Manifest
Date: 2026-01-31

## Purpose
A project-local KiCad library manifest for a *DWM3001-CDK* carrier reference in schematic and PCB layout.

Scope:
- One symbol library containing `DWM3001CDK` as a board-level reference device.
- One footprint library containing mating connector footprints for J10 and J1.
- Optional combined footprint placeholder for a single placement object, pending mechanical confirmation.
- 3D STEP placeholders stored under the project, referenced via `${KIPRJMOD}`.

This manifest follows the Thin-Pod style of keeping libraries versioned inside the repository.

---

## Directory layout
Recommended project-local structure:

```
<repo-root>/
  kicad_libs/
    symbols/
      thinpod_dwm3001cdk.kicad_sym
    footprints/
      thinpod_connectors.pretty/
        ThinPod_J10_PinSocket_2x13_P2.54mm_Vertical.kicad_mod
        ThinPod_J1_PinHeader_1x02_P2.54mm_Vertical.kicad_mod
        ThinPod_DWM3001CDK_Mating_Combined.kicad_mod
    3dmodels/
      dwm3001cdk/
        DWM3001-CDK.step
        J10_PinSocket_2x13_P2.54mm_Vertical.step
        J1_PinHeader_1x02_P2.54mm_Vertical.step
    docs/
      DWM3001CDK_KiCad_Library_Manifest.md
```

Notes:
- KiCad resolves `${KIPRJMOD}` to the directory of the `.kicad_pro` file.
- `.pretty/` directories are footprint libraries.
- STEP files listed above are placeholders. They may be replaced later by measured or vendor models.

---

## Symbol library
### File
`kicad_libs/symbols/thinpod_dwm3001cdk.kicad_sym`

### Library name
`thinpod_dwm3001cdk`

### Symbol name
`DWM3001CDK`

### Pin model
The symbol is modelled as a *board connector abstraction*:

- J10 is represented as a 2×13 header (26 pins).
- J1 is represented as a 2-pin power entry (battery loose wires).

Suggested pin naming convention:
- Pin names match the CDK net names, with a short functional description where useful.
- Pin numbers match the physical header numbering used by the CDK documentation.

J10 pin list (26 pins):
1. `NC`
2. `VRPI_5V0`
3. `SDA_RPI`
4. `VRPI_5V0`
5. `SCL_RPI`
6. `GND`
7. `NC`
8. `TXD`
9. `GND`
10. `RXD_RPI`
11. `NC`
12. `RESET`
13. `NC`
14. `GND`
15. `GPIO_RPI`
16. `NC`
17. `NC`
18. `NC`
19. `SPI1_MOSI`
20. `GND`
21. `SPI1_MISO`
22. `NC`
23. `SPI1_CLK`
24. `CS_RPI`
25. `GND`
26. `NC`

J1 pin list (2 pins):
- `J1_+` (VBAT)
- `J1_-` (GND)

Symbol representation guidance:
- A two-part symbol may be used, one unit for J10 and one unit for J1, with a single reference designator.
- Electrical type defaults: `Power input` for power rails, `Passive` for NC pins, and `Bidirectional` or `Input/Output` for GPIO and buses as appropriate.
- Hidden power pins are avoided, explicit nets are preferred.

---

## Footprint library
### Library
`kicad_libs/footprints/thinpod_connectors.pretty`

### Library name
`thinpod_connectors`

### Footprints
1. `ThinPod_J10_PinSocket_2x13_P2.54mm_Vertical`
   - Mating footprint for the CDK J10 interface.
   - Based on KiCad standard footprint geometry for a 2×13 socket at 2.54 mm pitch.
   - Courtyard and fab outlines preserved for KLC-style review.
   - 3D model path placeholder:
     - `${KIPRJMOD}/kicad_libs/3dmodels/dwm3001cdk/J10_PinSocket_2x13_P2.54mm_Vertical.step`

2. `ThinPod_J1_PinHeader_1x02_P2.54mm_Vertical`
   - Mating footprint for J1 (+ and -) if implemented as a 2-pin header.
   - 3D model path placeholder:
     - `${KIPRJMOD}/kicad_libs/3dmodels/dwm3001cdk/J1_PinHeader_1x02_P2.54mm_Vertical.step`

3. `ThinPod_DWM3001CDK_Mating_Combined`
   - Optional combined footprint containing both connector pad arrays, plus a board outline and keep-outs.
   - Status: placeholder until connector offsets and board outline are mechanically confirmed.
   - 3D model path placeholder:
     - `${KIPRJMOD}/kicad_libs/3dmodels/dwm3001cdk/DWM3001-CDK.step`

Implementation guidance:
- The default workflow uses separate connector footprints (items 1 and 2).
- The combined footprint is used only after the relative coordinates for J10 and J1 have been verified.

---

## 3D models
### Storage
`kicad_libs/3dmodels/dwm3001cdk/`

### Placeholders
- `DWM3001-CDK.step`  
  A single assembly model representing the CDK outline and major components.
- `J10_PinSocket_2x13_P2.54mm_Vertical.step`  
  Optional connector model for visual clearance checks.
- `J1_PinHeader_1x02_P2.54mm_Vertical.step`  
  Optional connector model.

KiCad footprint 3D model linkage:
- The footprint `.kicad_mod` references should use `${KIPRJMOD}` paths.
- Rotation and offsets remain at default unless a model origin mismatch is observed.

---

## KiCad project integration
The project library tables are updated so the repository-local libraries resolve without global installation.

### `sym-lib-table` entry
Add:

```
(lib
  (name thinpod_dwm3001cdk)
  (type KiCad)
  (uri ${KIPRJMOD}/kicad_libs/symbols/thinpod_dwm3001cdk.kicad_sym)
  (options "")
  (descr "Thin-Pod local symbols: DWM3001CDK")
)
```

### `fp-lib-table` entry
Add:

```
(lib
  (name thinpod_connectors)
  (type KiCad)
  (uri ${KIPRJMOD}/kicad_libs/footprints/thinpod_connectors.pretty)
  (options "")
  (descr "Thin-Pod local footprints: CDK mating connectors and placeholders")
)
```

---

## Version control hygiene
Recommended repository artefacts:
- Commit `kicad_libs/symbols/*.kicad_sym`
- Commit `kicad_libs/footprints/**/*.kicad_mod`
- Commit `kicad_libs/3dmodels/**/*.step` (placeholders are valid, even if empty stubs)
- Commit `sym-lib-table` and `fp-lib-table` if they are stored locally with the project

Suggested naming and tagging:
- Prefix `ThinPod_` for footprint names.
- Use library names starting `thinpod_` for local tables.

---

## Validation checklist
- Symbol pins for J10 map correctly to the intended nets in the schematic.
- Pin 1 orientation on the J10 footprint aligns with the CDK header orientation.
- 3D model references resolve using `${KIPRJMOD}`.
- Courtyard and fab layers pass KiCad DRC and KLC review expectations.
