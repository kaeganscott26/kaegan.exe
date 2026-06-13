# Opening Sequence

Back to [[../../README|README]]

Related:

* [[../GAME_GOAL|Game Goal]]
* [[Bad_Dream_Mommy_Behavior|Bad Dream Mommy Doorway Behavior]]

## Purpose

This document defines the opening playable sequence for Kaegan.exe.

The opening sequence should teach the player the core rule of the game:

The house is the threat.

There should be no combat.

There should be no traditional monster attack.

The house should use sound, darkness, movement restriction, door behavior, notes, drawings, and impossible space to guide and trap the player.

## Opening Design Goal

The player should begin by feeling helpless.

The player should not understand the rules yet.

The player should learn through experience that:

* the house controls movement
* the house controls doors
* the house controls direction
* the house can block familiar routes
* drawings and notes are progression keys
* sounds matter
* the bedroom is not permanently safe

## Starting Location

Kaegan starts in bed inside a dark bedroom at the end of the hall.

The hallway should eventually appear to lead toward Dad's room.

The bedroom should feel like a place that should be safe.

The lighting should be low.

A small nightlight or weak warm light should give the player just enough visibility to see the room, the bedroom door, and the general shape of the space.

The room should not be fully dark.

The player needs to see enough to feel trapped inside the room.

## Player State At Start

At the start of the game, Kaegan cannot move.

The player can only look around.

The player should not be able to:

* get out of bed
* walk
* sprint
* interact
* open the door

The player should be able to:

* move the camera
* look around the bedroom
* notice the door
* hear the house

This creates the feeling of waking up inside a nightmare before control returns.

## Event 1 - Dishes Crash

The first horror event happens automatically.

A loud crash of dishes comes from the kitchen or far side of the house.

The sound should feel distant but loud enough to demand attention.

This should happen before the player can move.

The player should still be trapped in bed.

The sound should not be explained.

The house speaks first through sound.

## Event 2 - Silence

After the dishes crash, there should be a short pause of complete silence.

This silence matters.

The silence should make the player wait.

The player should still be unable to move.

The room should feel like it is listening.

## Event 3 - Carpet-Drag Footsteps

After the silence, the player hears fast footsteps dragging across carpet.

The footsteps should move toward the bedroom door.

The sound should feel like:

```text
shff shfff shffff shhffff
```

The footsteps should not sound like normal walking.

They should sound fast, low, dragged, and wrong.

The player should still be unable to move.

## Event 4 - Door-Face Appearance

The bedroom door does not open.

Instead, the Bad Dream Mommy face appears through the door itself.

Use this visual reference:

```text
references/bad_dream_mommy_opening_scene.png
```

The face should be:

* still
* partially visible
* female-looking
* distorted
* hard for the brain to fully render
* wide-eyed
* naturally colored in the eyes
* lit only enough by the room's weak nightlight
* threatening without being fully explained

The player should only be able to look.

The face should not stay long enough to become fully understandable.

The player should feel:

What is that?

Then:

That is dangerous.

## Event 5 - Face Leaves

The face leaves or fades from the door.

The room becomes quiet again.

The player should not immediately know whether the danger is gone.

There should be a short pause before movement unlocks.

## Event 6 - Movement Unlock

After the face leaves, Kaegan can get out of bed.

The player gains normal first-person movement.

At this point, the game gives the player control for the first time.

This should feel like relief, but not safety.

The player should naturally move toward the bedroom door.

## Event 7 - First Drawing Appears

A drawing or note appears on the bedroom door where the face was.

This is the first required interaction.

The drawing should feel like proof that the event happened.

The player must interact with the drawing before the bedroom door can open.

The drawing acts as:

* progression key
* haunting trigger
* story fragment
* proof of house memory

The player should not be allowed to skip it.

## Event 8 - Bedroom Door Opens

After the player collects or reads the drawing, the bedroom door can open.

The player enters the hallway.

This is the first threshold crossing.

The player is leaving the only space that looked safe.

## Event 9 - Long Hallway

The hallway should look longer than it should be.

The player should see or understand that Dad's room is supposed to be down the hall.

The sprint option becomes available here.

The player should feel encouraged to run toward Dad's room.

The hallway should feel:

* dark
* stretched
* too long
* familiar but wrong
* unsafe behind the player
* falsely hopeful ahead of the player

## Event 10 - Dad's Room Denied

Instead of reaching Dad's room, the player reaches a wall.

The path does not go where it should.

The house has changed the route.

There is another note on the wall.

The note says:

```text
Turn around.
```

This should be simple.

No explanation.

No extra text.

The note should feel like a command from the house.

## Event 11 - Forced Return

After reading the note, the player must turn around and go back toward the bedroom.

The hallway may feel different on the return.

The player should feel like the house has changed its mind.

The path back should not feel safe.

## Event 12 - Bedroom Door Slams Shut

As the player approaches the bedroom, the bedroom door slams shut.

The player cannot open it.

The room is no longer safe.

This teaches the player:

The house can take away safety.

The player should try to interact with the door, but it should not open.

## Event 13 - House Opens Another Path

After the bedroom door becomes unavailable, another route opens.

This route forces Kaegan deeper into the house.

This is the transition from the opening sequence into the main exploration loop.

The player now understands the core rules:

* doors matter
* notes matter
* sounds matter
* the house can change space
* the safest path can be removed
* the bedroom is not a permanent refuge

## First Build Requirements

The first build should prove this sequence, even with placeholder art.

Required:

* first-person camera
* look-only starting state
* locked movement at start
* dishes crash sound
* silence pause
* carpet-drag footsteps sound
* door-face placeholder event
* movement unlock
* first drawing interaction
* bedroom door interaction
* long hallway
* wall with turn-around note
* return path
* bedroom door slam
* locked bedroom door
* alternate path opens

## Not Required Yet

The opening sequence does not need:

* final Bad Dream Mommy model
* final face animation
* final hallway art
* full house art
* full sound mix
* final drawings
* voice acting
* combat
* death system
* save system
* inventory system beyond reading or collecting notes and drawings

## Guiding Statement

The opening sequence should teach the player that the house controls the rules.

The player wakes up helpless.

The house makes sound.

The house shows a face.

The house gives control back.

The house offers a path.

The house removes that path.

The house forces the player deeper.
