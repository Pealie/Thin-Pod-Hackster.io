# Thin-Pod KiCad Project Library Manifest

## Purpose

This manifest defines a clean, portable KiCad library structure for the Thin-Pod PCB project, covering symbol libraries (`.kicad_sym`) and footprint libraries (`.pretty` containing `.kicad_mod`), plus recommended naming conventions and library table entries.

---

## Repository Layout

Recommended project tree (relative paths):

```
thin-pod/
├─ kicad/
│  ├─ Thin-Pod.kicad_pro
│  ├─ Thin-Pod.kicad_sch
│  ├─ Thin-Pod.kicad_pcb
│  ├─ sym/
│  │  └─ thinpod_custom.kicad_sym
│  ├─ fp/
│  │  ├─ thinpod_custom.pretty/
│  │  │  ├─ ThinPod_Pololu_S7V8F3_Dock.kicad_mod
│  │  │  ├─ ThinPod_DWM3001C_CDK_J10_Mate.kicad_mod
│  │  │  ├─ ThinPod_DWM3001C_CDK_J1_Mate.kicad_mod
│  │  │  ├─ ThinPod_ADXL1005_Breakout_Dock.kicad_mod
│  │  │  ├─ ThinPod_TestPad_1p0mm.kicad_mod
│  │  │  └─ ThinPod_Antenna_Keepout_Polygon.kicad_mod
│  │  └─ (optional additional .pretty libs)
│  ├─ 3d/
│  │  ├─ ThinPod_Pololu_S7V8F3.step
│  │  ├─ ThinPod_DWM3001C_CDK.step
│  │  └─ ThinPod_ADXL1005_Breakout.step
│  ├─ libtables/
│  │  ├─ sym-lib-table
│  │  └─ fp-lib-table
│  └─ README_LIBS.md
├─ docs/
│  └─ Thin-Pod_PCB_Library_Manifest.md
└─ LICENSES/
   └─ (project licensing)
```

Notes:
- `kicad/sym/` contains project-specific symbols.
- `kicad/fp/` contains project-specific footprints.
- `kicad/3d/` contains optional 3D models referenced by the custom footprints.
- `kicad/libtables/` contains project-local `sym-lib-table` and `fp-lib-table` for reproducible opens on new machines.

---

## Library Naming Conventions

### Symbol library
- File: `thinpod_custom.kicad_sym`
- Library nickname: `ThinPod_Custom_Sym`

Symbol naming convention:
- `ThinPod_<PartFamily>_<Variant>`
- Examples:
  - `ThinPod_Pololu_S7V8F3`
  - `ThinPod_DWM3001C_CDK_Dock` (logical docking symbol if preferred)
  - `ThinPod_ADXL1005_Breakout` (logical breakout symbol if the breakout is treated as a module)

### Footprint library
- Folder: `thinpod_custom.pretty`
- Library nickname: `ThinPod_Custom_FP`

Footprint naming convention:
- `ThinPod_<ModuleOrFunction>_<Descriptor>`
- Examples:
  - `ThinPod_Pololu_S7V8F3_Dock`
  - `ThinPod_DWM3001C_CDK_J10_Mate`
  - `ThinPod_ADXL1005_Breakout_Dock`

---

## Custom Symbol Set (Minimum)

Symbols recommended as project-local (custom) entries:

1. `ThinPod_Pololu_S7V8F3`
   - Pins (exact order):
     - 1: `SHDN`
     - 2: `VIN`
     - 3: `GND`
     - 4: `VOUT`
   - Pin types:
     - SHDN: input
     - VIN: power in
     - GND: power in
     - VOUT: power out

2. `ThinPod_DWM3001C_CDK_Headers` (optional convenience symbol)
   - Exposes only the used pins for clarity:
     - J1(+): `3V3+`
     - J1(-): `GND`
     - J10.15: `P0.28_ADC_IN`
     - J10.24: `P0.30_CS_RPI`
   - Intended as a “dock interface” symbol, not a replacement for the CDK schematic.

3. `ThinPod_ADXL1005_Breakout` (optional convenience symbol)
   - Pins: `VDD`, `VOUT`, `GND`, `ORo`, `ST`, `STB`

Where official KiCad symbols exist (e.g., generic connectors, generic PFET, diode, capacitors, resistors), the official libraries remain preferred.

---

## Custom Footprint Set (Minimum)

Footprints recommended as project-local entries:

1. `ThinPod_Pololu_S7V8F3_Dock`
   - Base: 1×4, 2.54 mm THT header pattern
   - Pin 1 marking: square pad + silkscreen “1”
   - Silkscreen labels: `SHDN VIN GND VOUT`
   - Optional courtyard for the module outline

2. `ThinPod_DWM3001C_CDK_J10_Mate`
   - Base: 2×N, 2.54 mm header footprint matching CDK mechanical drawing
   - Must match exact row spacing, pin count, and offsets used by the CDK
   - Silkscreen alignment marks to support assembly

3. `ThinPod_DWM3001C_CDK_J1_Mate`
   - Base: 1×2, 2.54 mm header footprint matching CDK J1 power header

4. `ThinPod_ADXL1005_Breakout_Dock`
   - Base: 1×6 header + four mounting holes pattern matched to the breakout PCB

5. `ThinPod_TestPad_1p0mm`
   - Round SMT pad footprint used for RAW_IN, RAW_PROT, 3V3+, ADC_NODE, etc.

6. `ThinPod_Antenna_Keepout_Polygon` (optional)
   - A helper footprint containing a keep-out and/or documentation outline
   - Alternative approach: keep-out drawn directly in the PCB editor

---

## Project Library Tables (Portable Setup)

To ensure the project opens correctly on any machine, use project-local library tables. Two approaches exist:

- Approach A: Commit `kicad/libtables/sym-lib-table` and `kicad/libtables/fp-lib-table` and point KiCad to them as project tables.
- Approach B: Keep tables beside the `.kicad_pro` file (KiCad project-local defaults).

The content below assumes the tables live beside the project file (recommended for minimal friction). If tables are stored in `kicad/libtables/`, adjust the paths accordingly.

### `sym-lib-table` (project-local)

Example entry:

```
(sym_lib_table
  (lib (name ThinPod_Custom_Sym)
    (type kicad)
    (uri ${KIPRJMOD}/sym/thinpod_custom.kicad_sym)
    (options "")
    (descr "Thin-Pod project custom symbols"))
)
```

### `fp-lib-table` (project-local)

Example entry:

```
(fp_lib_table
  (lib (name ThinPod_Custom_FP)
    (type KiCad)
    (uri ${KIPRJMOD}/fp/thinpod_custom.pretty)
    (options "")
    (descr "Thin-Pod project custom footprints"))
)
```

Notes:
- `${KIPRJMOD}` resolves to the directory containing the `.kicad_pro`.
- Avoid absolute paths to preserve portability.

---

## 3D Model Linking (Optional)

Where 3D models are available, link them using project-relative paths:

- 3D model directory: `${KIPRJMOD}/3d/`
- Assign STEP models in each custom footprint:
  - `ThinPod_Pololu_S7V8F3.step`
  - `ThinPod_DWM3001C_CDK.step`
  - `ThinPod_ADXL1005_Breakout.step`

If models are not committed, the footprints remain valid; the 3D view will simply omit the model.

---

## File Ownership and Change Control

- Custom library files are treated as project artefacts and version-controlled.
- One symbol file: `sym/thinpod_custom.kicad_sym`
- One footprint library folder: `fp/thinpod_custom.pretty/`
- Every custom footprint stored as a separate `.kicad_mod` file.
- Footprints and symbols carry a brief description field including:
  - source reference in project documentation
  - revision date
  - mechanical assumptions (pin numbering, orientation, keep-outs)

---

## Minimal Checklist for Implementation

1. Create directories: `kicad/sym`, `kicad/fp/thinpod_custom.pretty`, `kicad/3d`.
2. Create `thinpod_custom.kicad_sym` and add custom symbols.
3. Create required `.kicad_mod` footprints in `thinpod_custom.pretty`.
4. Add project-local entries to `sym-lib-table` and `fp-lib-table` using `${KIPRJMOD}`.
5. Assign footprints to symbols and run ERC/DRC.
6. Commit library files to version control.
