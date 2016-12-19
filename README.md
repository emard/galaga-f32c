# GALAGA for f32c and its c2 sprites

Galaga rework for FPGArduino f32c
with look and feel of Henrik Wening's legendary
GALAGA with 1/4 cursor block graphics for PET and C64.

This galaga game is currently incomplete, just a tool for
sprite graphics demo and search for bugs (if any) in f32c CPU core, 
c2 sprites, their libraries and new gcc.

Henrik did it by looking at original Galaga.
I did it by looking at Henrik's work.

See C64 video example
[GALAGA on youtube](https://www.youtube.com/watch?v=4c_li-PBJ6g)
or here
[Game #058 Galaga](http://www.theawardian.de/?p=5153)

# Modifications

I had to make some interventions in the game...

Most space shooter games shows aliens as bad and
agressive beings, which is not politically correct...

Here aliens come in peace.
They won't fire a single missile
until first of them is killed...

But as the game is incomplete, the above is not currently
only a TODO. Aliens are shooting harmless bullets, the player
ship can't shoot or move...

# Technical details

On f32c the block graphics is not used, although it could be done with
textmode.

I decided to use c2 sprites with "artwork" that resembles block graphics.

Sprites are chosen primarily to improve playability in situations
when smooth moving of the sprites can make game more consistent and playable.

For example, aliens can shoot missiles at an angle.
Original game renders such missile very jerky, and the missiles
are tiny dots, hard to see. Ship can get destroyed without being able to
properly see the trajctory of incoming missiles. 

Now sprites will smooth render alien fire shots at angle, so player 
will have more chance to see and avoid them.
