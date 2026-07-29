# SYNTH_01 — Component & PCB handoff

**Updated:** 2026-07-29  
**Purchasing source of truth:** Notion database **Composants SYNTH_01**.  
**Workflow reference:** Hermes skill `electronics-coaching`, file `references/synth01-pcb-bom-gate.md`.

## Current electrical state

- Main schematic: `hardware/SYNTH_01.kicad_sch`.
- **Checkout state:** local `main` has been fast-forwarded to `9017408`. The electrical schematic is unchanged since the reviewed checkpoint `2de3658` (the later commit is metadata/images). Before any future work, run `git pull --ff-only` and inspect the resulting schematic rather than relying on this snapshot.
- In the reviewed schematic, `C1`, `C2`, `C3` are 100 nF; `R1`, `R2` are 10 kΩ.
- ERC at that checkpoint: zero electrical errors. Do not commit generated ERC reports by default.
- The schematic represents 21 switches / diodes, 7 EC11 encoders, two MCP23017 I²C expanders, Raspberry Pi connector J1 and OLED connector J2.

## Frozen purchase choices

- Switches: LEOBOG Reaper Linear, MX-compatible, **5-pin PCB mount**. `SW1…SW21`; project qty 21, buy 30. AliExpress `1005007927090867`.
- Encoders: bare EC11, AliExpress `1005007643925677`, exact option **G 20mm Plum handle**; integrated push, 5 terminals, 20 positions, M7×0.75 bushing. `ENC1…ENC7`; project qty 7, buy 10.
- U1/U2: MCP23017-E/SP DIP-28. AliExpress `1005009219211233`; qty 2.
- U1/U2 sockets: DIP-28 narrow / 7.62 mm, AliExpress `1005006919421431`; qty 2.
- C1/C2/C3: 100 nF / 50 V radial ceramic disks (104), AliExpress `1005006378388179`; project qty 3.
- R1/R2: 10 kΩ, 1/4 W, 1 %, axial metal-film; AliExpress `1005003923602966`, select **10K**; project qty 2.
- D1…D21: 1N4148 DO-35; AliExpress `1005009379098965`; project qty 21.
- J1: Pi GPIO female-to-male stacking header, 2×20 / 40 pins, 2.54 mm THT; AliExpress `1005010308099092`; project qty 1.
- OLED: provisional 0.91-inch 128×32 SSD1306 I²C board, AliExpress `1005008640108394`; project qty 1.

## Important electrical distinctions

- EC11 is bare: terminals `A/B/C/S1/S2`, not a HW-040 `CLK/DT/SW/VCC/GND` breakout.
- Current EC11 connections: `A → E*_A`, `B → E*_B`, `S1 → E*_SW`, `C/S2 → GND`.
- OLED logical J2 convention is `GND / +3V3 / SCL / SDA`. The physical pin order of the received OLED must match before finalizing J2.
- No GPIO ribbon cable is part of the final PCB. It is a prototype/debug accessory only.

## Still open — do not guess / do not start PCB layout yet

1. Verify EC11 dimensions/pin layout/mounting lugs against the candidate KiCad footprint.
2. Measure capacitor lead pitch from received parts.
3. Verify OLED pin order, header type/position, dimensions and mounting holes.
4. Decide J2 female 1×4 socket; buy a matching male 1×4 header only if OLED is not supplied with pins.
5. Measure Pi-to-PCB clearance using the actual J1 stacking header.
6. Design faceplate/PCB mounting only after all above: MX cutouts, M7 holes, standoffs, screws and enclosure thickness.

## Mechanical rule

Component reality → verified footprints → PCB outline/holes → placement → routing → DRC/BOM/Gerbers. Never reverse this order.
