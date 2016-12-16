# GALAGA c2

Galaga rework for FPGArduino f32c
with look and feel of Henrik Wening's legendary
GALAGA with 1/4 cursor block graphics for PET and C64.

Henrik did it by looking at original Galaga.
I did it by looking at Henrik's work.

See C64 video example at

    https://www.youtube.com/watch?v=4c_li-PBJ6g
    Game #058 Galaga
    http://www.theawardian.de/?p=5153

# Modifications

I had to make some interventions in the game...

Most space shooter games shows aliens as bad and
agressive beings, which is not politically correct...

Here aliens come in peace.
They won't fire a single missile
until first of them is killed...

# Technical details

On f32c the block graphics is not used, although it could be done with
textmode.

I decided to use c2 sprites with "artwork" that resembles block graphics.

Sprites are chosen primarily to improve playability in some occasions
when smooth moving of the sprites can make game more consistent and playable.

For example, aliens can shoot missiles at an angle.
Original game renders such missile very jerky, and the missiles
itself are hard to see, so ship gets destroyed without being able to
properly see the trajctory of the missile. Here sprites will smooth render
alien fire shot at angle and player can have at least some fair chance to see
and avoid them.
