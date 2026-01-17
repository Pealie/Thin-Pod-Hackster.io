# Thin-Pod carrier v0.2: THT footprint and drill spec (draft)

Scope: bare PCB only, through-hole patterns for hand assembly of the DWM3001C-CDK, ADXL1005 evaluation board, Pololu S7V8F3 regulator module, and the added RAW Schottky plus ADXL high-side PFET gating.

Units: mm (inch equivalents where standard).

## Global defaults

- Holes: plated through-hole (PTH)
- Annular ring target: >= 0.40 mm
- Solder mask expansion: 0.05 mm to 0.10 mm (leave KiCad default unless a fab requires otherwise)
- For 2.54 mm headers: size holes for typical 0.64 mm square pins (diagonal ~0.91 mm)

## J10: 2 x 13 header (CDK Raspberry Pi style)

Purpose: mechanical and electrical interface to CDK header J10 (carrier side).

### Footprint options (KiCad standard libraries)

- Male pins on carrier: `Connector_PinHeader_2.54mm:PinHeader_2x13_P2.54mm_Vertical`
- Female socket on carrier: `Connector_PinSocket_2.54mm:PinSocket_2x13_P2.54mm_Vertical`

### Pad stack (recommended)

- Pitch: 2.54 mm (rows) x 2.54 mm (columns)
- Finished drill diameter: 1.02 mm
- Pad diameter: 1.85 mm (round)
- Copper annular ring: (1.85 - 1.02) / 2 = 0.415 mm

### Geometry notes

- Total pin span (length): (13 - 1) * 2.54 = 30.48 mm
- Row spacing: 2.54 mm
- Place a Pin 1 marker on silkscreen and match CDK Pin 1 orientation in the schematic symbols.

## J1: 1 x 2 power header

Purpose: mechanical access to CDK J1 (+ / -) power entry holes.

### Footprint (KiCad standard library)

- `Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical`

### Pad stack (recommended)

- Pitch: 2.54 mm
- Finished drill diameter: 1.02 mm
- Pad diameter: 1.85 mm (round)

## ADXL1005 evaluation board: 6-hole pattern (header / via row)

Purpose: provide a through-hole landing for the ADXL1005 eval board header vias. The ADXL1005 eval board provides two alternative rows of 6 vias for a header, so the carrier can support either one row (6 holes) or both rows (12 holes) depending on preferred mounting.

### Minimal pattern (6 holes)

- Footprint: `Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical`
- Pitch: 2.54 mm
- Finished drill diameter: 1.02 mm
- Pad diameter: 1.85 mm

### Optional pattern (support both header rows)

- Footprint: build a small custom footprint that instantiates two `1x06` rows at the same spacing as the eval board provides.
- The spacing between the two rows is board-specific and should be measured on the physical eval board (calipers), then fixed in the footprint.

### Courtyard (recommended)

- Reserve at least 20.5 mm x 20.5 mm for the eval board body.
- Keepout for tall parts on the underside should be added after the board is in hand.

## Pololu S7V8F3 module: 4-hole pattern (VIN, GND, VOUT, SHDN)

Purpose: through-hole landing for the regulator module pins.

The Pololu S7V8F3 dimension diagram indicates:

- Board outline: 11.43 mm x 16.51 mm (0.45" x 0.65")
- Pin holes: 4 holes on 2.54 mm pitch (0.1")
- Hole diameter: 1.02 mm (0.040")
- Hole row offset from pin-side long edge: 1.27 mm (0.050")
- End offsets: 1.91 mm (0.075") from each short edge to the first and last hole centres

### Practical footprint choice

- If the module can be soldered directly to the carrier (module pins through carrier): a simple `PinHeader_1x04_P2.54mm_Vertical` pad pattern is sufficient.
- If the module is intended to be removable: use a 1x4 socket header footprint and match pin orientation.

### Custom footprint coordinates (recommended, for mechanical certainty)

Define the S7V8F3 module outline as a courtyard and place 4 pads using these coordinates:

- Coordinate origin: module corner at (0, 0)
- X axis: along the 11.43 mm edge (0.45")
- Y axis: along the 16.51 mm edge (0.65")
- Hole row located at y = 1.27 mm from the pin-side long edge

Pad centres:

- Pad 1: (1.905, 1.270)
- Pad 2: (4.445, 1.270)
- Pad 3: (6.985, 1.270)
- Pad 4: (9.525, 1.270)

Pad stack:

- Finished drill diameter: 1.02 mm
- Pad diameter: 1.85 mm

## D1: RAW input Schottky diode (1N5817)

Purpose: series reverse polarity protection from `RAW_IN` to `RAW_PROT`.

### Recommended footprint

- `Diode_THT:D_DO-41_SOD81_P10.16mm_Horizontal`

### Pad stack

- Lead pitch: 10.16 mm (0.4")
- Finished drill diameter: 1.10 mm
- Pad diameter: 2.20 mm

## Q1: ADXL high-side P-MOSFET (THT)

Purpose: high-side gating of `3V3_A` to `ACC_3V3_SW`.

Two practical THT package paths exist. The default below is TO-92 for small current loads.

### Option A (default): TO-92 inline (small signal)

- Example class of parts: P-channel MOSFET in TO-92 (for example, BS250 family class)
- Footprint: `Package_TO_SOT_THT:TO-92_Inline`
- Lead pitch: 1.27 mm between adjacent leads (overall 2.54 mm from lead 1 to lead 3)

Pad stack:

- Finished drill diameter: 0.80 mm
- Pad diameter: 1.60 mm

### Option B: TO-220-3 (overkill but easy to source)

- Example class of parts: P-channel MOSFET in TO-220 with low RDS(on) at VGS = -2.5 V
- Footprint: `Package_TO_SOT_THT:TO-220-3_Vertical`

Pad stack:

- Finished drill diameter: 1.30 mm
- Pad diameter: 2.50 mm
- Lead pitch: 2.54 mm

## C1 and C2: ADXL decoupling capacitors (THT lead pitch)

Purpose: local decoupling on `ACC_3V3_SW` to ground, per the PFET gating block.

### C1 (100 nF, local HF decoupling)

- Recommended package: ceramic disc capacitor
- Footprint: `Capacitor_THT:C_Disc_D3.0mm_W1.6mm_P2.50mm`

Pad stack:

- Lead pitch: 2.50 mm (0.1")
- Finished drill diameter: 0.80 mm
- Pad diameter: 1.60 mm

### C2 (1 uF to 10 uF, local bulk)

Two common stocking-friendly radial pitches exist. Select one after selecting an actual part.

- Option 1 (smaller radial electrolytic): 2.50 mm lead pitch
  - Example footprint class: `Capacitor_THT:CP_Radial_*_P2.50mm`
- Option 2 (larger radial electrolytic or film): 5.00 mm lead pitch
  - Example footprint class: `Capacitor_THT:CP_Radial_*_P5.00mm` or `Capacitor_THT:C_Rect_L*_*_P5.00mm`

Pad stack (both options):

- Finished drill diameter: 0.80 mm
- Pad diameter: 1.60 mm

## Axial resistors (R_SER1, R1, R2 and related)

Recommended default for hand assembly and broad part compatibility:

- Package: DIN0207, 0.25 W, axial
- Footprint: `Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal`

Pad stack:

- Lead pitch: 7.62 mm (0.3")
- Finished drill diameter: 0.80 mm
- Pad diameter: 1.60 mm

## General-purpose 'dead-bug style' pads

For optional bodge parts and quick bench rework on the carrier:

- Single pad: `Connector_Wire:SolderWirePad_1x01_Drill1mm`
- Two pad jumper: `Connector_Wire:SolderWirePad_1x02_P2.54mm_Drill1mm`

These pads form convenient anchor points for DNP parts, scope hooks, and experimental RC values.

## Notes on verification

- Header drill diameters above are conservative for common 2.54 mm header pins. If tight fit is observed on an existing board, move from 1.02 mm to 1.10 mm for the header drills and increase pad diameter to 2.00 mm.
- For the S7V8F3 and ADXL1005 module footprints, final mechanical placement should be verified against the physical modules on first printout (1:1 plot) before ordering.
