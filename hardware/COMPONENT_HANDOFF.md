# SYNTH_01 — Component & PCB handoff

**Updated:** 2026-07-31

**Purchasing source of truth:** Notion database **Composants SYNTH_01**.

**Project page:** Notion **SYNTH_01 — Chord Machine MIDI**.

**Workflow:** guide Arthur in eeschema; never edit `hardware/SYNTH_01.kicad_sch` by script.

## Architecture decision — authoritative

V0 is an **independent USB-MIDI controller**:

- controller: Raspberry Pi Pico 2 H;
- interface: 21 MX switches, 21 diodes, 7 bare push encoders, OLED;
- expansion: one MCP23017;
- output: USB MIDI through the Pico onboard micro-USB;
- excluded from V0: Raspberry Pi 4, Linux audio, DAC, oscillators and standalone audio.

The future standalone V2 will be reconstructed separately around Raspberry Pi 4/Linux. Do not preserve Pi 4 HAT compatibility in the V0 schematic.

## Current repository state

- Main schematic: `hardware/SYNTH_01.kicad_sch`.
- The checked-in schematic still represents the **obsolete Pi 4 HAT design**: J1 Pi header, U1/U2 MCP23017, 21 switches/diodes, 7 encoders and J2 OLED.
- That schematic is an archive/refactor starting point, not fabrication-ready.
- The existing PCB file is effectively empty and has no valid board outline.
- Before work: `git pull --ff-only`, inspect the actual schematic and do not trust an old ERC report.

## Why the refactor is required

- Pi 4 gadget MIDI uses its USB-C device port, which is also the normal power input; this is undesirable for the V0 controller.
- Pico 2 H is a native USB device and can provide power + class-compliant MIDI over one cable.
- Encoder quadrature signals are time-sensitive; their 14 A/B lines should be read directly by the Pico rather than through I²C/Linux.
- MCP23017 `GPA7` and `GPB7` must not be used as inputs. Reserve them for matrix-row outputs.

Official MCP warning: https://support.microchip.com/s/article/GPA7---GPB7-Cannot-Be-Used-as-Inputs-In-MCP23017

## Target V0 signal architecture

### Pico 2 H

Provisional GPIO plan to verify against the installed KiCad symbol and official Pico 2 pinout before Arthur wires it:

- `GP0` → `SDA` (`I2C0`);
- `GP1` → `SCL` (`I2C0`);
- `GP2…GP15` → `E1_A`, `E1_B`, …, `E7_A`, `E7_B` (14 direct inputs);
- `GP16` → `E7_SW` (direct push input);
- remaining exposed GPIOs reserved for debug or revisions;
- USB uses the Pico module's onboard connector, not GPIO pins.

Encoder commons remain:

- rotation `C → GND`;
- push `S2 → GND`;
- `A/B/S1` are active-low inputs with pull-ups configured in hardware/firmware as appropriate.

### U1 — single MCP23017 at 0x20

- `A0/A1/A2 → GND`;
- `RESET → +3V3` through `R1 = 10 kΩ`;
- `VDD → +3V3`, `VSS → GND`;
- `C1 = 100 nF` directly across VDD/VSS;
- `SDA/SCL` share the Pico I²C bus with the OLED;
- `INTA/INTB` are not required for the first polling implementation unless a later verified design explicitly uses them.

Recommended 16-bit allocation:

- outputs `ROW0…ROW4` → `GPA5`, `GPA6`, `GPA7`, `GPB6`, `GPB7`;
- inputs `COL0…COL_SHIFT` → `GPA0…GPA4`;
- inputs `E1_SW…E6_SW` → `GPB0…GPB5`.

This deliberately puts the MCP23017 output-only-sensitive `GPA7/GPB7` on row outputs and keeps all eleven inputs on reliable pins.

### OLED

- shared `SDA/SCL` at 3.3 V;
- provisional logical connector: `GND / +3V3 / SCL / SDA`;
- physical pin order must match the exact received module;
- keep optional 4.7 kΩ I²C pull-up footprints until the module's onboard pull-ups are verified;
- retain local `100 nF` decoupling if the module connection requires it.

## Next-session refactor plan for the agent

Arthur performs every schematic edit in KiCad. The agent explains the electrical role first, gives one coherent block, and reviews only after Arthur pushes.

### Task 1 — establish the checkpoint

1. Run `git pull --ff-only` in `~/GIT/SYNTH_01`.
2. Read this handoff, the live Notion project page and the components database.
3. Load `electronics-coaching`.
4. Inspect `hardware/SYNTH_01.kicad_sch` and export its current netlist.
5. Confirm that no uncommitted schematic edit would be lost.

### Task 2 — validate the Pico symbol and pin budget

1. Verify the official Pico 2 H pinout and the exact KiCad symbol/footprint available locally.
2. Confirm the provisional `GP0…GP16` allocation above is legal.
3. Confirm USB, SWD, RUN, 3V3_OUT, VSYS and GND behavior.
4. Explain the full mapping to Arthur before asking him to edit.

Checkpoint: no schematic edit yet if the mapping is not fully validated.

### Task 3 — replace the host/power block in eeschema

Guide Arthur to:

1. remove the obsolete Pi 4 connector `J1` from the V0 schematic;
2. place the validated Pico 2 H module symbol;
3. connect its 3V3 output and GND to the V0 rails;
4. preserve the OLED and U1 I²C net names;
5. remove Pi-specific interrupt and power labels that no longer have a consumer.

Do not assign final footprints or touch PCB layout in this task.

### Task 4 — reduce two expanders to one

Guide Arthur to:

1. delete obsolete `U2`, its reset resistor and its decoupling capacitor;
2. retain U1 at I²C address `0x20`;
3. reassign U1 exactly as specified above;
4. ensure `GPA7/GPB7` are only outputs;
5. connect `E1_SW…E6_SW` to U1 and `E7_SW` directly to the Pico.

### Task 5 — connect all encoder rotations directly

Guide Arthur to connect:

- `E1_A/B…E7_A/B` to the fourteen validated Pico GPIOs;
- all encoder `C` rotation commons to GND;
- all encoder `S2` push contacts to GND;
- preserve unique labels and avoid accidental shared A/B nets.

### Task 6 — finish power, I²C and values

1. U1 reset pull-up: `10 kΩ`.
2. U1 decoupling: `100 nF` across 3V3/GND.
3. OLED power and decoupling.
4. Optional I²C pull-up footprints, populated only after checking the OLED module.
5. Normalize every diode value to `1N4148` and every capacitor unit to `nF`.
6. Add No Connect markers to genuinely unused Pico/MCP pins.

### Task 7 — review gate after Arthur's push

1. Load `kicad-pushed-schematic-review`.
2. Fetch/pull the pushed commit.
3. Export the netlist and BOM.
4. Verify all 21 switches/diodes and all 7 encoders.
5. Verify no A/B signal passes through MCP23017.
6. Verify `GPA7/GPB7` are outputs only.
7. Run ERC and explain every remaining warning.
8. Reconcile Notion quantities with the new netlist.

Only after this gate may the project move to exact footprints and mechanical measurements. PCB placement/routing remains blocked.

## Active component quantities for the target schematic

- 1 × Raspberry Pi Pico 2 H;
- 1 × MCP23017-E/SP and 1 × DIP-28 socket;
- 1 × 10 kΩ MCP reset resistor;
- 2 × 100 nF local decoupling capacitors provisionally (U1 + OLED connection);
- 21 × MX-compatible switches;
- 21 × 1N4148;
- 7 × bare EC11-style push encoders;
- 1 × I²C OLED;
- 2 × 1×20 female sockets for removable Pico 2 H, exact stack height to confirm;
- optional 2 × 4.7 kΩ I²C pull-ups, pending measurement of the OLED board.

## Mechanical/PCB gate still blocked

Do not route or order the PCB before verifying:

1. exact EC11 body, lugs, shaft and footprint;
2. exact MX switch footprint and plate geometry;
3. OLED pin order, dimensions, header and mounting holes;
4. Pico 2 H socket geometry, USB access and clearance;
5. capacitor lead pitch;
6. faceplate, standoffs and enclosure clearances;
7. complete netlist/BOM/footprint reconciliation.
