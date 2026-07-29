"""Creates the saved runtime host map for LEVEL 01 HOME.

The level geometry and gameplay actors are built by AKaeganHomeDirector at BeginPlay,
which keeps this map stable and avoids hand-wired binary Blueprint dependencies.
"""
import unreal

MAP_PATH = "/Game/Kaegan/Maps/L_Home_01"
unreal.EditorLevelLibrary.load_level(MAP_PATH)
player_starts = [actor for actor in unreal.EditorLevelLibrary.get_all_level_actors()
                 if isinstance(actor, unreal.PlayerStart)]
if not player_starts:
    player_start = unreal.EditorLevelLibrary.spawn_actor_from_class(
        unreal.PlayerStart, unreal.Vector(0.0, 0.0, 100.0), unreal.Rotator(0.0, 0.0, 0.0))
    player_start.set_actor_label("KaeganSafeEntry")
    unreal.log("KAEGAN: Added safe PlayerStart to L_Home_01")
else:
    unreal.log("KAEGAN: L_Home_01 already has a PlayerStart")

unreal.EditorLevelLibrary.save_current_level()
