# Bad Dream Mommy Doorway Behavior

Back to [[../../README|README]]

Related: [[../GAME_GOAL|Game Goal]]

## Purpose

This document defines the specific doorway behavior for Bad Dream Mommy in Kaegan.exe.

This is not a normal monster behavior document.

Bad Dream Mommy is most frightening when she exists at the boundary between rooms.

She should not behave like a creature that simply walks into the room and attacks.

Her fear radiates from stillness, partial visibility, jerky motion, threshold pressure, and the feeling that she is getting closer without fully entering the room.

## Core Rule

Bad Dream Mommy does not need to enter the room to be dangerous.

The doorway is her weapon.

The player should feel like the room is being invaded even when she never fully crosses the threshold.

## Doorway Presence

When Bad Dream Mommy appears in an open doorway, she should begin at the edge of the doorframe.

She should not start centered in the doorway.

Sometimes she only shows her face by poking her neck through with jerking, twitching motions.

Sometimes she reaches an arm or leg unnaturally through the doorway and up the wall, almost as if she is trying to climb the doorway.

Sometimes she appears through the door itself. Sometimes only her face appears. Other times, a random leg or foot may show through the door along with the face, twisted and unnatural.

Sometimes the player only sees a silhouette.

The player should feel uncertain about what they are looking at, only knowing that it radiates fear and evil.

The shape should read as human enough to be recognized, but disturbingly twisted and malformed, almost like the house memorized the shape but did not know the meaning of the body parts.

## Stillness

Stillness is required.

Before Bad Dream Mommy moves, she should hold completely still.

The stillness should last long enough for the player to notice her.

The player should have a moment to think:

What is that?

Then the fear should build from the fact that she is not moving normally.

The stillness should feel intentional.

She is not frozen because she is inactive.

She is still because she is watching.

## Leaning Motion

Bad Dream Mommy should lean into the doorway instead of walking into the room.

Her motion should feel broken.

She should move in short, sudden, jerky increments.

The motion should not be smooth.

The movement rhythm should feel like:

- still
- twitch
- lean
- still
- twitch
- lean further
- still

The pauses are as important as the motion.

Each lean should make her feel closer without making her fully enter the room.

She should feel like she is testing the threshold.

She should feel like she is waiting for the room to let her in.

## Threshold Rule

Bad Dream Mommy should remain tied to the doorway during early encounters.

She should not fully enter the bedroom during the opening doorway sequence.

She should not rush the player immediately.

The player should feel trapped because she is close enough to threaten them, but distant enough that the brain keeps waiting for the actual attack.

That waiting is the scare.

## Sound Design

Her movement should create a subtle physical sound.

The sound should feel like skin or body surface brushing against the wooden door frame.

It should not sound like a monster roar.

It should not sound like a loud jump scare sting.

It should be close, dry, and physical.

The sound cue can be described as:

```text
SHHFFFT
```

This is not dialogue or text. It is a physical sound reference.

The player should associate that sound with her movement.

When the player hears that sound later, they should immediately remember the doorway.

## Visual Design

Bad Dream Mommy should remain partially hard to understand.

Her doorway form should use darkness and limited light to hide exact details.

The player should see enough to know she is there, but not enough to fully explain her.

Important traits:

- female-looking silhouette
- wrong posture
- head and neck relationship feels unnatural
- face may be unclear or partially hidden
- movement feels too sudden
- body does not enter the room normally
- she appears to lean closer without walking
- limbs may appear where they should not be
- the body may feel assembled incorrectly by the house

## Face Behavior

When the face is visible, it should not become a clean monster portrait.

The face should be readable enough to scare the player, but not clear enough to feel fully rendered.

Important traits:

- eyes opened unnaturally wide
- naturally colored eyes
- harmful intent in the stare
- mouth shaped wrong
- head sitting wrong on the neck
- curly or permed early-1990s-style hair
- low nightlight illumination
- darkness hiding the full shape

The player should understand danger before they understand anatomy.

## Player State

This behavior is strongest when the player cannot move.

During the opening encounter, Kaegan should be frozen in bed.

The player should be able to look around, but not get up or walk away.

This recreates the feeling of being trapped inside a childhood nightmare.

The player should have to watch the doorway behavior happen.

## Emotional Goal

The player should not think:

A monster entered the room.

The player should think:

Something is wrong with the doorway.

Something is aware of me.

It is getting closer.

It does not have to come inside to reach me.

## Implementation Notes

This behavior can be built as a staged horror event, not full enemy AI.

The first version can use:

- a doorway marker
- a hidden silhouette actor
- timed visibility
- short jerky transform movements
- pause timing
- subtle audio cues
- locked player movement
- locked camera look-only state
- lighting control

The goal is to prove the scare before building a full creature system.

## Do Not Do

Do not make Bad Dream Mommy sprint into the room during the opening doorway event.

Do not reveal the full model too early.

Do not make the sound too loud.

Do not turn the moment into a normal jump scare.

Do not explain what she is.

Do not make her movement smooth.

Do not make her behavior feel like normal enemy AI.

## Guiding Statement

Bad Dream Mommy is terrifying because she respects the doorway and violates it at the same time.

She does not enter.

She leans.

She shifts.

She waits.

The doorway becomes unsafe before the room does.
