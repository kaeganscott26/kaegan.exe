# kaegan.exe

A first-person psychological horror game built as a personal birthday project/present for my son, Kaegan.

## Project Root

This file is the root index for the Kaegan.exe project.

## Core Documents

- [[docs/GAME_GOAL|Game Goal]]

## Script Documents

- [[docs/script/OPENING_SEQUENCE_TIMING_SCRIPT|Opening Sequence Timing Script]]

## House Documents

- [[docs/house/Bad_Dream_Mommy_Behavior|Bad Dream Mommy Doorway Behavior]]
- [[docs/house/NIGHTMARE_RULES|Nightmare Rules]]
- [[docs/house/OPENING_SEQUENCE|Opening Sequence]]
- [[docs/house/HOUSE_BEHAVIOR_RULES|House Behavior Rules]]
- [[docs/house/HOUSE_LAYOUT_MEMORY_MAP|House Layout Memory Map]]

## Entity Documents

- [[docs/entities/DAD_MIMIC|Dad Mimic]]

## Reference Files

- [[references/House Layout/kaegan_house_memory_floorplan.png|House Memory Floor Plan]]

## Current Build Goal

LEVEL 01 HOME is implemented as a runtime-built Unreal vertical slice. See [FIRST_LEVEL_IMPLEMENTATION.md](FIRST_LEVEL_IMPLEMENTATION.md) for the exact map, controls, build status, temporary assets, and launch instructions.

The UE 5.7 editor target, command-line map load, automated runtime progression, and editor map launch were verified on 2026-07-29. A final human visual/audio playthrough is still recommended for the prototype placeholder presentation.

The current playable route includes:

- first-person movement
- a runtime-blocked memory house with living room, kitchen, hallway, bathroom, and bedrooms
- reusable interaction and hinged-door systems
- an inspectable childhood drawing, corruption pass, doorway reveal, father silhouette/crutches, and controlled ending

## Engine

Unreal Engine 5

## Project Layout

- `docs/` - design notes and development documentation
- `planning/` - task lists and build plans
- `references/` - visual, story, and house references
- `assets/` - external source assets before import, such as pictures, floor plans, sound recordings, etc.
- `unreal/` - Unreal project files
- `exports/` - packaged builds and testing exports
- `installer/` - final installer package and download-ready release files

## Build Documents

- [[docs/build/FIRST_PLAYABLE_BUILD_CHECKLIST|First Playable Build Checklist]]
