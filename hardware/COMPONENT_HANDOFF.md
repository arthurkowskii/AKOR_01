# AKOR_01 — PCB handoff


## Archive note (2026-08-08)

The RGB LED chain (SK6812MINI-E, 74AHCT1G125, 470 µF reservoir, 10 kΩ/470 Ω 0805, 21× 100 nF 0805) is **archived and out of V0 scope**.
- Git: preserved in branch `archive/rgb-led` (2026-08-08).
- Notion: those 6 rows are now status `Plus tard`.
- `hardware/BOM_AKOR_01.csv` lists the NO_LED build only.
Do not treat any LED chain as build context for the V0 PCB.
**Updated:** 2026-07-31

This file is the local handoff for the next Codex session. The live Notion page
`AKOR_01` and its database `Composants AKOR_01` remain the purchasing and
decision source of truth.

## Authoritative V0 scope

V0 is an independent USB-MIDI controller built around a Raspberry Pi Pico 2 H:

- 1 Raspberry Pi Pico 2 H;
- 1 MCP23017-E/SP at address `0x20`;
- 21 MX switches and 21 1N4148 diodes;
- 7 EC11-style push encoders;
- 1 SSD1306 I2C OLED, 0.91 inch, 128 x 32;
- 1 reset resistor and 2 local 100 nF decoupling capacitors.

Raspberry Pi 4, Linux audio, DAC, amplifier and standalone synthesis are outside
V0. They belong to a later V2 and must not constrain this PCB.

## Verified electrical checkpoint

The electrical schematic is complete in `hardware/AKOR_01.kicad_sch`.

- Pico: `GP0=SDA`, `GP1=SCL`, `GP2...GP15=E1_A/B...E7_A/B`,
  `GP16=E7_SW`.
- U1: matrix rows on `GPA5/GPA6/GPA7/GPB6/GPB7`, columns on
  `GPA0...GPA4`, encoder pushes `E1_SW...E6_SW` on `GPB0...GPB5`.
- OLED connector: `J2.1=GND`, `J2.2=+3V3`, `J2.3=SCL`, `J2.4=SDA`.
- The last electrical checkpoint is commit `8ba3745`.
- The schematic title block currently says `AKOR_01_SCH`; the repository and
  project remain named `AKOR_01`.

Arthur performs schematic and PCB edits in KiCad. Do not rewrite `.kicad_sch`
or `.kicad_pcb` by script.

## Footprint checkpoint

Footprint assignment is complete at commit `3c35341`:

- `A1` — `Module:RaspberryPi_Pico_Common_THT`;
- `U1` — `Package_DIP:DIP-28_W7.62mm_Socket`;
- `D1...D21` —
  `Diode_THT:D_DO-35_SOD27_P10.16mm_Horizontal`;
- `R1` —
  `Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P10.16mm_Horizontal`;
- `SW1...SW21` —
  `Button_Switch_Keyboard:SW_Cherry_MX_1.00u_PCB`;
- `ENC1...ENC7` —
  `Rotary_Encoder:RotaryEncoder_Alps_EC11E-Switch_Vertical_H20mm`;
- `J2` —
  `Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Vertical`;
- `C1`, `C3` —
  `Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P5.00mm`.

Audit result: 55 physical components, 0 missing footprints.

Validated footprints: U1 socket, D1...D21 and R1.

Provisional until physical measurement: MX switches, EC11 encoders, Pico socket
stack height, OLED header/body clearance and capacitor body dimensions. The
capacitor listing explicitly states a 5 mm lead pitch, so `P5.00mm` is
intentional; measure the received body before fabrication.

## Current PCB state

`hardware/AKOR_01.kicad_pcb` is still effectively empty:

- no imported footprints;
- no board outline;
- no mounting holes;
- no placement;
- no copper tracks or zones.

Do not mistake completed footprint assignment in the schematic for completed PCB
layout.

## Exact next lesson

1. Run `git pull --ff-only` and confirm `main` matches `origin/main`.
2. Read this handoff and the top `État actuel` block in Notion.
3. Open the schematic and use **Tools → Update PCB from Schematic**.
4. Import the 55 footprints into PCB Editor. This command imports the netlist;
   it does not route tracks.
5. Save and review the generated PCB diff before committing.
6. Begin mechanical placement only:
   - MX grid on a 19.05 mm pitch;
   - seven EC11 encoders;
   - OLED body and readable area;
   - Pico with micro-USB accessible at a board edge;
   - board outline, mounting holes and silkscreen zones.
7. Review placement and 3D view before any routing.

## Fabrication gate

Do not route definitively and do not order a PCB until the physical parts have
confirmed:

- LEOBOG Reaper underside geometry and five-pin MX fit;
- EC11 pin/lug geometry and shaft height;
- Pico socket height and retention;
- OLED pin order, header orientation, 38 x 12 mm body clearance and support at
  its free end;
- 100 nF capacitor body and lead diameter;
- board outline, mounting holes, underside protection and USB clearance.

The PCB is the visible instrument face: black solder mask, white silkscreen,
exposed components and a protected underside. Ergonomics and mechanical
placement come before routing.

## Git hygiene

KiCad modifies `hardware/.history` while editing. Do not stage or commit that
submodule unless Arthur explicitly asks. Stage only the reviewed project files.
