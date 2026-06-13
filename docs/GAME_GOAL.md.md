
# Game Goal

##Purpose

Kaegan.exe is a first-person psychological horror game built as a personal birthday project for my son, Kaegan.

The game is inspired by childhood nightmare memories, house-based fear, dream logic, and the feeling of being trapped inside a place that should be safe.

This is not a monster attack game.

This is a house-behavior horror game.

The house is the main threat.

# Core Idea

Kaegan wakes up inside a dark bedroom.

At first, he cannot move.

He can only look around.

The house begins performing the nightmare before the player understands what is happening.

Sounds happen before explanations.

Doors become boundaries.

Hallways stretch.

Rooms change.

Notes and drawings act as progression keys and haunting triggers.

The player slowly learns that the house is not just haunted.

The house remmebers.

# Emotional Goal

The game should feel personal, intimate, and wrong.

The horror should come from:

- childhood fear
- darkness
- distorted memory
- being unable to move
- trying to reach safety
- familiar spaces behaving incorrectly
- sounds that should not be happening
- doorway presence 
- false waking
- the feeling thatthe house is guiding the player somewhere.

The player should not feel like they are fighting a monster.

The player should feel like they are trapped inside a nightmare that understands them.

# Main Design Rule

The house performs the story.

The house should use sound, light, movement restriction, impossible space, drawings, notes, door behavior, and repeated nightmare loops to create fear.

The player should experience the story through the environment instead of being told the story directly.

# Interaction Rules
The player should rarely be told what to do directly.

if the house wants the player to move, it should:
- change light
- open or close a door
- place a drawing
- repeat a sound
- block a familiar path
- make the wrong path feel unsafe

That keeps the "house performs the story" rule consistent.

# Opening Sequence

Kaegan begins in bed in a dark bedroom at the end of the hall.

The hallway leads towards his Dad's room, but the house will not allow that path to work normally.

At the beginnin, Kaegan cannot get out of bed. 

The player can only look around.

A loud sound of dishes crashing happens from the kitchen at the other side of the house. The sound must sound distant, but loud enough to grab the attention of the player.

After that after a short pause of complete silence, the player still not being able to move, the sound of fast footsteps dragging on carpet approach the closed bedroom door.

The bedroom door does not open.

Instead, the "Bad Dream Mommy" face appears through the door itself. 
Use the following file path for reference: "C:\Users\North\Documents\Projects\kaegan.exe\references\bad_dream_mommy_opening_scene.png"

The face should be still, partially visible, and difficult for the brain to fully understand.

After the face leaves, Kaegan (the player) can move.

A drawing or note appears on the door where the face was.

The player (Kaegan) must interact with the drawing before opening the door.

After leaving the bedroom, the sprint option appears so Kaegan can sprint dow a long dark hallway towards "Dad's" room.

Instead of reaching Dad's room, he reaches a wall with another note.

The note tells him to turn around.

When Kaegan returns to his bedroom (the room the game starts in), the bedroom door slams shut and cannot be opened. 

The house then opens another path, forcing Kaegan deeper inside.

# Bad Dream Mommy Behavior

Bad Dream Mommy must never be named inside the game...

Bad Dream Mommy should not behave like a normal enemy.

She is most frightening when she exists at thresholds.

She appears near doors, through doors, or at the edge of doorways.

Her movement should be specific and feel intentional:

- Stillness first
- partial silhouette at the doorway edge
- short jerky leaning motion
- sudden pauses
- small twitching movements
- the feeling that she is getting closer without fully entering the room

She should not fully enter the bedroom during any part of the game. 

The doorway IS her weapon.

Her motion should be accompanied by a subtle sound like skin brushing against a wooden door frame.

The sound should feel close, dry, and physical. 

The sound cue should feel like:

print("SHHFFFT", sound_not_text)

# Face Design Rule

The Bad Dream Mommy face should not be fully clear.

It should be visible enough for the player to know something is there, but not clear enough for the brain to fully render it.

The face should feel female, distorted, and wrong.

Important traits:
- wide unnatural eyes
- naturally colored eyes
- harmful intent in the stare
- mouth shaped wrong
- head sitting on the neck incorrectly 
- curly/permed early-1990s-style hair
- partially hidden by darkness
- lit only enough by the rooms's low nightlight

The face should never become a clean monster protrait. 

The player should think:

What is that?

Then immediately feel:

That is dangerous.

# Chase Behavior

There may be moments where Bad Dream Mommy chases kaegan.

The chase should not work like a normal sprint sequence.

As she gets closer, Kaegan should become slower.

The fear comes from losing control of the body.

The closer she gets, the more movement begins to fail.

Right before she catches him, Kaegan wakes back up in the bedroom.

This creates a false waking loop.

The player should feel relief for only a moment before realizing the nightmare has changed.

# Dad Mimic Escalation

After the false wake-loop begins, the Dad Mimic layer starts to appear.

The "Dad_Mimic_Character" should never be directly named in the game.

This should begin through sound and feeling bbefore it becomes visual.

The Dad_Mimic should courrupt the idea of safety.

Dad's room should feel like the place Kaean wants to reach, but the house should make that path unstable.

The Dad Mimic should not be overused early.

It should be treated as an escalation layer after the player already understands the house and Bad Dream Mommy Behavior.

The Dad Mimic begins after the false waking loop and manifests itself as only sounds of the house...crutches, prosthetic leg clicks and clunks from walking with weight and with patience..the crutches clink in a specific rythom that is natural at first but changes to unnatural rythm and lengths.

The Dad Mimic is never physically revealed until the very end as a final jump scare after Kaegan reaches the final note.

After final jump scare, the player awakes in his bed and its day time then the game ends..

# Gameplay Pillars

The game is built around these pillars:

- first-person exploration
- movement restriction
- house-triggered events
- notes and drawing as progression keys
- sound-based horror
- impossible hallway behavior
- doorway scares
- false waking
- enviornmental storytelling
- no combat
- no traditional moster attack loop

# First Build Target

The first playable build should prove the opening nightmare loop.

The player should be able to:

- wake up in bed
- look around while movement is locked
- hear the dishes crash
- see the door-face event 
- regain movement 
- collect the first drawing
- open the bedroom door
- enter the long hallway
- reach the turn-around note 
- reutrn to the bedroom see the bedroom door slam shut
- be forced into the rest of the house

# NOT REQUIRED YET

The first build doen not need:

- final character models
- final house art
- full monster AI
- COMBAT (NEVER COMBAT)
- INVENTORY SYSTEM (Only to read/collect each drawing/notes)
- Save system (You cannot die in the game so no need to save)
- Full voice acting....(The house is the voice)
- final cutscenes
- polished animations
- complete house layout (complete house layout will be provided in "C:\Users\North\Documents\Projects\kaegan.exe\references")

The first build only needs to prove the nightmare logic.

# Guiding Statement 

Kaegan.exe should feel like a childhood nightmare remembered too clearly.

The house should not explain itself. The notes / drawings tell the story and give the player context. Added with the house's response to event triggers it should naturally evolve without being confusing.

The player should learn through fear, repetition, sound, and the terrible feeling of dread. The feeling that the safest path has been taken away.




