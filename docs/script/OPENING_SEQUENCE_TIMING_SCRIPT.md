\# Opening Sequence Timing Script



Back to \[\[../../README|README]]



Related:



\* \[\[../GAME\_GOAL|Game Goal]]

\* \[\[../house/OPENING\_SEQUENCE|Opening Sequence]]

\* \[\[../house/NIGHTMARE\_RULES|Nightmare Rules]]

\* \[\[../house/HOUSE\_BEHAVIOR\_RULES|House Behavior Rules]]

\* \[\[../house/HOUSE\_LAYOUT\_MEMORY\_MAP|House Layout Memory Map]]

\* \[\[../house/Bad\_Dream\_Mommy\_Behavior|Bad Dream Mommy Doorway Behavior]]

\* \[\[../entities/DAD\_MIMIC|Dad Mimic]]



\## Purpose



This document defines the timing script for the opening sequence of Kaegan.exe.



This is not a dialogue script.



This is a playable timing script.



It exists to guide Unreal Blueprint implementation for the first playable nightmare loop.



The goal is to control pacing, player movement, sound triggers, note gates, hallway denial, and false safety.



Timing is essential.



The fear should come from waiting, silence, delayed control, and the house changing rules after the player thinks they understand them.



\## Core Timing Rule



The game should not rush scares.



Each major event should follow this rhythm:



\* sound

\* silence

\* player waiting

\* visual event

\* pause

\* control change

\* false relief

\* route denial

\* escalation



The player should feel that the house is performing the sequence intentionally.



\## Beat 01 - Wake State



\### Player State



Kaegan starts in bed.



The player can look around.



The player cannot move.



The player cannot interact.



The player cannot open the door.



\### Timing



Suggested duration before first event:



```text

3 to 5 seconds

```



\### Purpose



Establish helplessness.



Let the player understand the room before the house does anything.



The room should feel quiet, dim, and wrong.



\## Beat 02 - Dishes Crash



\### Trigger



Automatic after Beat 01.



\### Sound



A loud dishes crash comes from the kitchen side of the house.



The sound should be distant but sharp enough to immediately grab attention.



\### Player State



Movement remains locked.



Camera look remains allowed.



\### Timing After Sound



```text

4 to 7 seconds of silence

```



\### Purpose



The house speaks first through sound.



The player should wait in the dark and wonder what happened.



\## Beat 03 - Listening Silence



\### Trigger



Immediately after dishes crash.



\### Sound



No music.



No footsteps yet.



Only subtle room tone if needed.



\### Player State



Movement remains locked.



\### Purpose



The silence should not feel empty.



The silence should feel like the house is listening.



The player should become uncomfortable before the next sound starts.



\## Beat 04 - Carpet-Drag Footsteps



\### Trigger



After the silence pause.



\### Sound



Fast carpet-drag footsteps approach the bedroom door.



The sound should feel like:



```text

shff shfff shffff shhffff

```



The sound should start farther away and move closer.



\### Player State



Movement remains locked.



Camera look remains allowed.



\### Timing



Suggested sound duration:



```text

3 to 6 seconds

```



Then a pause:



```text

1 to 3 seconds

```



\### Purpose



Create approach dread.



The player should understand something is coming before seeing anything.



\## Beat 05 - Door-Face Appearance



\### Trigger



After footsteps stop outside the bedroom door.



\### Visual



The bedroom door does not open.



The Bad Dream Mommy face appears through the door itself.



For the first build, this can be a placeholder plane, image, shadow, or dark shape.



\### Player State



Movement remains locked.



The player can only look.



\### Timing



Optional micro-delay before face appears:



```text

1 second

```



Face visible duration:



```text

2 to 4 seconds

```



\### Purpose



The player should think:



```text

What is that?

```



Then immediately feel:



```text

That is dangerous.

```



The face should not remain long enough to become fully understandable.



\## Beat 06 - Face Leaves



\### Trigger



After Door-Face Appearance duration ends.



\### Visual



The face leaves, fades, or retracts.



The bedroom door appears normal again.



\### Player State



Movement remains locked briefly.



\### Timing



Pause after face disappears:



```text

2 to 4 seconds

```



\### Purpose



Do not unlock movement instantly.



Let the player sit with the question of whether the danger is gone.



This creates false relief.



\## Beat 07 - Movement Unlock



\### Trigger



After the post-face pause.



\### Player State



Movement unlocks.



Interaction unlocks.



The player can get out of bed.



\### Purpose



The player experiences control as relief.



The relief should not feel like safety.



\## Beat 08 - First Drawing Appears



\### Trigger



After movement unlocks.



\### Visual



A drawing or note appears on the bedroom door where the face was.



\### Interaction



The player must interact with the drawing before the bedroom door can open.



\### Player State



Movement allowed.



Door remains locked until drawing is collected or read.



\### Purpose



Teach the player that drawings and notes are progression keys.



The drawing proves that the event happened.



\## Beat 09 - Bedroom Door Unlock



\### Trigger



Player interacts with the first drawing.



\### Event



Bedroom door unlocks.



Optional sound:



```text

soft click

```



\### Player State



Player can open bedroom door.



\### Purpose



The house rewards interaction by allowing the player to cross the threshold.



The player should feel like the house is permitting progress.



\## Beat 10 - Enter Hallway



\### Trigger



Player opens bedroom door and enters hallway.



\### Environment



The hallway should appear to lead toward Dad's room.



The hallway should feel too long.



\### Player State



Movement allowed.



Sprint may become available here.



\### Purpose



Encourage the player to move toward the safety target.



The player should believe Dad's room is reachable.



\## Beat 11 - Hallway Stretch / Safety Denial Setup



\### Trigger



Player advances down hallway.



\### Environment



The hallway may stretch, feel longer, or visually deny normal distance.



No full scare is required yet.



\### Sound



Optional low room tone or subtle house sound.



\### Purpose



Build doubt.



The player should begin to feel that the hallway is not behaving normally.



\## Beat 12 - Dad's Room Denied



\### Trigger



Player reaches the expected Dad's room destination.



\### Event



Instead of Dad's room, the player reaches a wall.



A note is on the wall.



\### Note Text



```text

Turn around.

```



\### Player State



Movement allowed.



The player must interact with or acknowledge the note.



\### Purpose



This is the first major impossible-space event.



The house denies the expected safety route.



\## Beat 13 - Forced Return



\### Trigger



Player reads or interacts with the Turn Around note.



\### Event



The player must return toward the starting bedroom.



The hallway may feel different on the return.



\### Sound



Optional subtle sound shift.



Optional distant house movement.



\### Purpose



The player should feel that the house has changed its mind.



The return path should not feel safe.



\## Beat 14 - Bedroom Door Slam



\### Trigger



Player approaches the starting bedroom door after returning.



\### Event



The bedroom door slams shut.



The door becomes locked.



\### Sound



Loud door slam.



Optional short silence afterward.



\### Player State



Player can try to interact with the door, but it will not open.



\### Timing After Slam



```text

2 to 4 seconds of silence

```



\### Purpose



Teach the player that the bedroom is no longer safe.



The house can take away refuge.



\## Beat 15 - Doorway Threat Reminder



\### Trigger



After bedroom door slam, optional for first build.



\### Sound



A subtle doorway movement sound:



```text

SHHFFFT

```



\### Visual



Optional placeholder shadow, handprint, or brief doorway presence.



\### Player State



Movement allowed.



\### Purpose



Remind the player that the doorway itself is unsafe.



This beat can be skipped in the earliest graybox if needed.



\## Beat 16 - Alternate Path Opens



\### Trigger



After the bedroom door is locked and the player is denied entry.



\### Event



Another door or route opens.



This route forces Kaegan deeper into the house.



\### Sound



Door creak, latch click, or environmental cue.



\### Player State



Movement allowed.



\### Purpose



Transition from opening sequence into the main exploration loop.



The house has now taught the player:



\* sounds matter

\* notes matter

\* doors matter

\* the bedroom is not permanent safety

\* Dad's room can be denied

\* the house controls the route



\## First Playable Timing Target



The first playable build should prove the following sequence:



```text

BeginPlay

Lock movement

Allow camera look

Delay

Play dishes crash

Delay in silence

Play carpet-drag footsteps

Delay

Show door-face placeholder

Delay

Hide door-face placeholder

Delay

Unlock movement

Enable first drawing

Player collects drawing

Unlock bedroom door

Player enters hallway

Player reaches wall

Player reads Turn Around note

Player returns

Bedroom door slams and locks

Alternate path opens

```



\## Blueprint Implementation Notes



The opening sequence can be controlled by a single sequence manager Blueprint.



Suggested Blueprint:



```text

BP\_OpeningSequenceManager

```



Suggested responsibilities:



\* lock player movement at start

\* allow camera look

\* trigger sound events

\* trigger timed delays

\* show and hide door-face placeholder

\* unlock player movement

\* enable first drawing interaction

\* track note collection

\* unlock bedroom door

\* detect hallway wall note interaction

\* detect player return trigger

\* slam and lock bedroom door

\* open alternate path



\## Placeholder Asset Rules



The first build does not need final assets.



Allowed placeholders:



\* cube for bed

\* cube or plane for door

\* plane for note

\* plane or dark rectangle for door-face

\* placeholder sound for dishes crash

\* placeholder sound for footsteps

\* placeholder sound for door slam

\* placeholder hallway geometry

\* placeholder wall denial



Do not wait for final art before proving the timing.



\## Timing Adjustment Rule



All times in this document are starting suggestions.



The final timing should be adjusted by feel during playtesting.



If a scare feels too fast, add silence.



If a moment feels confusing, add visual guidance.



If a moment feels weak, delay the reveal.



The player should often wait slightly longer than feels comfortable.



\## Do Not Do



Do not rush from dishes crash directly into the face.



Do not unlock movement immediately after the face disappears.



Do not explain the notes too much.



Do not make the hallway denial complicated.



Do not add combat.



Do not add death screens.



Do not polish visuals before the sequence works.



Do not add the Dad Mimic fully in the first graybox.



\## Guiding Statement



The opening sequence should feel like the house is teaching the player its rules.



The player wakes helpless.



The house makes sound.



The house waits.



The house shows a face.



The house gives control back.



The house offers safety.



The house denies safety.



The house forces the player deeper.



