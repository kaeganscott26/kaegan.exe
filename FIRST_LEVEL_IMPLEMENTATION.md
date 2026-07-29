# LEVEL 01 HOME - Implementation Record

## Environment assessment

- **Repository:** `C:\Users\North\Documents\Projects\kaegan.exe`
- **Unreal project:** `unreal\KaeganEXE\KaeganEXE.uproject`
- **Detected engine:** Unreal Engine **5.7**, installed through Epic Games Launcher at `C:\Program Files\Epic Games\UE_5.7`.
- **Project architecture before this work:** Blueprint-only UE first-person template. It had no `Source/` directory, no project C++ targets, a template GameMode, Enhanced Input assets, and two maps (`Lvl_FirstPerson` and `MAP_OpeningSequence`).
- **Architecture now:** hybrid. C++ owns the reliable runtime player, interaction, progression, UI, and house blockout. `L_Home_01` is a saved Unreal map host, allowing the entire level to run without hand-wired Blueprint references.
- **Existing content preserved:** all existing template Content, project plugins, reference material, and user changes were retained. The pre-existing modified `.obsidian/graph.json` was not touched.

## Verified toolchain and prerequisite repair

- Windows had no pending restart at validation time.
- Unreal Engine **5.7.4** (`5.7.4-51494982`) is installed at `C:\Program Files\Epic Games\UE_5.7`; `UnrealEditor.exe`, `UnrealEditor-Cmd.exe`, UBT, and the bundled .NET 8.0.412 host were verified.
- Visual Studio 2022 Build Tools **17.14.33** is installed with MSVC **14.44.35207** x64 tools and the C++ workload.
- Windows SDK **10.0.26100.0** is installed.
- Installed during this repair: Microsoft .NET Framework **4.8.1 Developer Pack** using `winget install --exact --id Microsoft.DotNet.Framework.DeveloperPack_4 --silent`. The SDK registry key is `HKLM\SOFTWARE\WOW6432Node\Microsoft\Microsoft SDKs\NETFXSDK\4.8.1`; `C:\Program Files (x86)\Reference Assemblies\Microsoft\Framework\.NETFramework\v4.8.1` exists.
- This resolved Unreal's `SwarmInterface`/`NetFxSDK install dir` error. There is no remaining restart requirement.

## Work completed

### Saved map and startup configuration

- Added `Content/Kaegan/Maps/L_Home_01.umap` as the first-level host map with a saved `KaeganSafeEntry` PlayerStart. The actual playable environment is assembled by the level director during BeginPlay.
- Updated `GameDefaultMap` and `EditorStartupMap` to `/Game/Kaegan/Maps/L_Home_01`.
- Updated `GlobalDefaultGameMode` to `/Script/KaeganEXE.KaeganHomeGameMode`.
- Updated the displayed project name to `KAEGAN.EXE`; the project file and module names were not renamed.

### Systems and C++ classes added

All level code is in `unreal/KaeganEXE/Source/KaeganEXE/`.

- `AKaeganHomeGameMode` - selects the project player/controller and ensures a valid pawn is possessed in an empty map.
- `AKaeganFirstPersonCharacter` - slow first-person walk, mouse look, gravity/collision, optional sprint, movement locks, and a throttled camera interaction trace.
- `AKaeganHomePlayerController` - boot, pause, completion, notices, mouse capture/release, restart, and menu actions.
- `UKaeganHomeScreen` - code-built Unreal UMG screen used for boot, pause, observation text, and completion UI.
- `AKaeganInteractable` - reusable camera-trace interaction base.
- `AKaeganDoorActor` - hinged, collision-aware, smooth-interpolated opening/closing door with locked state.
- `AKaeganMemoryObject` - one-use central drawing interaction that locks control briefly and advances progression.
- `AKaeganHomeDirector` - constructs the house, owns one-way progression state, validates events, schedules horror beats, and safely resets through map reload.
- `AKaeganFatherMimic` - reusable non-AI silhouette with two explicitly named temporary crutches. It is staged by position/visibility instead of unreliable navigation AI.

### House and temporary assets

The director creates a readable low-cost prototype house: entry/living room, kitchen/dining space, main hallway, bathroom, Kaegan's room, parent room, and final hallway. It uses engine basic shapes, restrained movable point lights, floors, ceilings, room boundaries, doors, furniture silhouettes, a television, bed, desk, counters, and a bathroom mirror.

Temporary runtime assets are deliberately tagged and easy to replace:

- `TEMP_KaeganDrawing` - the gold-toned inspectable drawing at Kaegan's desk.
- `TEMP_LivingSofa`, `TEMP_Television`, `TEMP_KitchenTable`, `TEMP_KitchenCounters`, `TEMP_KaeganBed`, `TEMP_KaeganDesk`, `TEMP_BathroomMirror` - prototype environment silhouettes.
- `TEMP_FatherMimic`, `TEMP_LeftCrutch`, `TEMP_RightCrutch` - the final silhouette and crutches.
- `TEMP_Footstep` / `TEMP_CrutchImpact` are named runtime cue hooks. No final personal sound recordings were present in the repository, so these currently use restrained textual/positional timing hooks rather than pretending final audio exists.

To replace the drawing or photos, import approved images under `Content/Kaegan/Temporary` and assign their material to the corresponding director-created component, or replace `TEMP_KaeganDrawing` with a placed Blueprint that subclasses `AKaeganMemoryObject`. To replace the mimic/crutches, make a Blueprint child of `AKaeganFatherMimic`, replace its four mesh components, then change the director's spawned class. Replace audio by adding `USoundBase` references in `SpawnFootstepHint`, door interaction, memory interaction, and `BeginFinalSequence`; attenuation should remain positional and local to the named house points.

### Visibility and lighting pass (2026-07-29)

- Raised the runtime floor, wall, ceiling, wood, furniture, and corruption albedos from near-black to dark readable values. The `BasicShapeMaterial` dynamic `Color` parameter is now explicitly reassigned to each runtime mesh; development validation logs the applied living-room floor tint.
- Added a runtime unbound post-process volume that uses fixed manual exposure (`+1.2` bias, physical-camera exposure disabled), restrained vignette, and ambient occlusion. This prevents editor or inherited map exposure from collapsing the game view to black.
- Added a movable cold-blue Skylight and a non-shadowing directional moon fill. The house also now has 12 zone lights: shadowed cool/warm practicals for contrast, plus low-level shadowless ambient fills in the living room, kitchen, main hall, Kaegan's room, and final hall. Practical intensity/radius were increased to cover their full rooms without using white or red lighting.
- After the drawing interaction, practicals dim only to 76% and ambient fills to 88%, preserving the frightening transition while keeping the critical route readable. The mimic remains blue-black, with a readable human outline and separate crutch silhouettes.
- The enforced runtime spawn moved from `(0, 0, 100)` to `(0, -300, 100)`: a clear living-room lane aimed toward the route, away from the sofa/television cluster. Validation confirms the capsule has no blocking overlap; the nearest blocking geometry is the floor, 100 cm from the pawn origin.
- Development-only `-KaeganValidation` logging now reports the actual spawn, every light's intensity/radius/shadow state, active light count, ambient sources, fixed exposure state, dynamic material result, and nearest spawn obstruction. Add `-KaeganCapture` to request starting-room and post-memory hallway screenshots.

## Controls

- `WASD` - move
- Mouse - look
- `E` - interact
- `Escape` - pause / resume
- `Left Shift` - optional sprint

The existing Enhanced Input template assets remain in the project. This vertical slice also registers reliable legacy fallback bindings in `DefaultInput.ini`; these work with UE's Enhanced Input component without requiring manually created Input Action assets.

## Actual level flow

1. `L_Home_01` opens black with the boot text: `KAEGAN.EXE`, `BUILD STATUS: INCOMPLETE`, `MEMORY SOURCE DETECTED`, and `LOADING: HOME`.
2. The director builds the home at runtime, keeps movement locked during boot, then unlocks the player in the living-room/entry route.
3. The player can inspect the television, open normal interior doors, and explore kitchen, bathroom, rooms, and the main hall. The final-hall door remains locked.
4. In Kaegan's room, the player examines `TEMP_KaeganDrawing`. Control briefly locks, the text `I remember this.` appears, ambient lighting drops, and the final hallway unlocks.
5. Timed footstep/crutch cues begin from selected distant house locations. A doorway silhouette appears in the main hall long enough to read, then disappears and relocates to the final hall.
6. Crossing into the newly unlocked long hallway triggers the corrupted state: alternate furniture appears, lighting is reduced, and the silhouette stages at the far end. Returning toward the house produces a retreat-response observation but cannot break the route.
7. Approaching the final threshold locks movement, kills the practical lights, moves the crutch silhouette closer, plays the final clink/clunk timing cue, and cuts to the completion screen.
8. Completion shows `MEMORY 01 RECOVERED`, `SOURCE INCONSISTENT`, `FATHER INSTANCE DETECTED`, plus Continue, Restart Level, and Main Menu. Continue says `NEXT MEMORY UNAVAILABLE` and returns to boot; Restart and Main Menu both cleanly reload the map/boot sequence.

## Level states

`Boot -> ExploreHouse -> FootstepsActive -> MimicRevealed -> HouseCorrupted -> FinalSequence -> LevelComplete`

Transitions are guarded against repeat calls and are logged under `LogKaeganHome`. The periodic progression check uses a 0.25-second timer; interaction traces run only every 0.08 seconds. No world searches run every frame.

## Build and validation results

Run from `unreal\KaeganEXE`:

```powershell
$engineDotnet = 'C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\ThirdParty\DotNet\8.0.412\win-x64\dotnet.exe'
& $engineDotnet 'C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.dll' -projectfiles -project="$PWD\KaeganEXE.uproject" -game -engine
& $engineDotnet 'C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.dll' KaeganEXEEditor Win64 Development -Project="$PWD\KaeganEXE.uproject" -WaitMutex -NoHotReload
& 'C:\Program Files\Epic Games\UE_5.7\Engine\Binaries\Win64\UnrealEditor-Cmd.exe' "$PWD\KaeganEXE.uproject" /Game/Kaegan/Maps/L_Home_01 -game -windowed -ResX=1280 -ResY=720 -KaeganValidation -KaeganCapture -NoSplash
```

Results on 2026-07-29:

- Project-file generation: passed.
- Unreal Header Tool: passed with warnings-as-errors.
- `KaeganEXEEditor Win64 Development`: passed. Build log: `unreal/KaeganEXE/Saved/Logs/CodexBuild.log`.
- Rendered command-line map load: passed; `/Game/Kaegan/Maps/L_Home_01` loaded with `KaeganHomeGameMode` and the custom pawn at `(0, -300, 100)`. The visibility validation reported 12 active point lights, movable ambient sky/moon fills, manual exposure at `+1.20`, correct dynamic material tint, and `spawn_blocked=false`.
- Automated game-world validation: passed boot, possession, pause/resume screen lifecycle, memory interaction, hallway corruption, mimic staging, final movement lock, and completion screen/mouse release. It uses the opt-in `-KaeganValidation` flag and exits after success.
- Visual validation: passed. A clean rendered capture of the untouched start position shows a readable floor, wall edges, door/hall opening, television/sofa silhouettes, and safe sightline. The rendered post-memory hallway capture shows its floor and wall planes, a readable threatening human/crutch silhouette, and no black-screen failure. Captures: `unreal/KaeganEXE/Saved/Screenshots/KaeganHome_StartingRoom00001.png` and `unreal/KaeganEXE/Saved/Screenshots/KaeganHome_MainHall_AfterMemory00001.png`.
- Full Unreal Editor launch: passed. UE 5.7 opened the project and loaded `L_Home_01` after this lighting change.

Repairs made during validation: installed the missing .NET Framework SDK; fixed UE 5.7 point-light API usage and warnings-as-errors member shadowing; ordered runtime mesh mobility before mesh assignment; added a saved PlayerStart plus enforced safe spawn; and prevented stale notice timers from removing a later completion screen.

The rendered command-line run and saved screenshots visually confirm the required navigation readability. A short human PIE playthrough remains useful for subjective door feel, final UI-button clicking, and audio polish.

## Exact launch instructions

1. Open `C:\Users\North\Documents\Projects\kaegan.exe\unreal\KaeganEXE\KaeganEXE.uproject` in Unreal Engine 5.7.
2. No compilation should be required unless source files have changed after this validated build.
3. Confirm `/Game/Kaegan/Maps/L_Home_01` is open (it is both the editor startup map and game default map).
4. Press Play and wait for the boot text to clear, then use the controls above.

No known fatal startup or runtime blocker remains. The expected limitation is prototype visuals/audio: the house, drawing, mimic, crutches, and cues currently use tagged runtime placeholder assets.

## Best next task

Replace the tagged temporary drawing, silhouette/crutch meshes, and positional cue hooks with approved personal art and recorded household/crutch audio, then perform a visual lighting pass inside PIE on the ASUS laptop.
