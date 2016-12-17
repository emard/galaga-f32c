/* Compositing2 sprite example
 * with some moving vertical color stripes.
 */
#include <Compositing.h>
#include "shapes.h"

#define SPRITE_MAX 160
#define N_SHAPES (sizeof(Shape)/sizeof(Shape[0]))

// max number of objects on the screen
#define SHIPS_MAX 100

// fixed point scale
#define FPSCALE 256

// global speed 1/2/4/8
#define SPEED 4

// alien distance in convoy
#define CONVOY_DISTANCE 24

// alien x-distance in fleet
#define FLEET_DISTANCE 20

Compositing c2;

// starship states
enum 
{ 
  S_NONE,
  S_ALIEN_PREPARE, S_ALIEN_CONVOY, S_ALIEN_HOMING, S_ALIEN_HOME,
  S_ALIEN_ATTACK, S_ALIEN_EXPLODING, S_ALIEN_DEAD
};

struct path_segment
{
  int v; // velocity, v = FPSCALE --> 1 pixel/frame
  uint8_t a; // initial angle 0-255 covers 0-360 degrees, 0->right, 64->up, 128->left, 192->down
  int8_t r; // rotation (angle increment)
  int n; // how many frames to run on this path segment, 0 for last
};

struct path_segment stage1_convoy[] =
{
  {FPSCALE,   0, 0, 50 }, // right 50 frames
  {FPSCALE,  32, 0, 50 }, // right-up 50 frames
  {FPSCALE,  64, 0, 50 }, // up 50 fames
  {FPSCALE,  96, 0, 50 }, // up-left 50 fames
  {FPSCALE, 128, 0, 50 }, // left 50 fames
  {FPSCALE, 160, 0, 50 }, // left-down 50 fames
  {FPSCALE, 192, 0, 50 }, // down 50 fames
  {FPSCALE, 224, 0, 50 }, // down-right 50 fames
  {FPSCALE,   0, 1, 256 }, // left circle 256 frames
  {FPSCALE,   0,-1, 256 }, // right circle 256 frames
  {FPSCALE,   0, 1, 256 }, // left circle 256 frames
  {FPSCALE,   0,-1, 256 }, // right circle 256 frames
  {0,0,0} // end
};

struct path_segment stage2_convoy_left[] =
{
  {SPEED*FPSCALE,   8, 0,     128/SPEED }, // right slightly up 128 frames
  {SPEED*FPSCALE,   0, SPEED, 256/SPEED }, // left circle 256 frames
  {SPEED*FPSCALE,   8, 0,     128/SPEED }, // right slightly up 128 frames
  {SPEED*FPSCALE,   0,-SPEED, 256/SPEED }, // right circle 256 frames
  {SPEED*FPSCALE,   0, SPEED, 256/SPEED }, // left circle 256 frames
  {SPEED*FPSCALE,   0,-SPEED, 256/SPEED }, // right circle 256 frames
  {0,0,0} // end
};

struct path_segment stage2_convoy_right[] =
{
  {SPEED*FPSCALE,  120, 0,     128/SPEED }, // right slightly up 128 frames
  {SPEED*FPSCALE,  128,-SPEED, 256/SPEED }, // left circle 256 frames
  {SPEED*FPSCALE,  120, 0,     128/SPEED }, // right slightly up 128 frames
  {SPEED*FPSCALE,  128, SPEED, 256/SPEED }, // right circle 256 frames
  {SPEED*FPSCALE,  128,-SPEED, 256/SPEED }, // left circle 256 frames
  {SPEED*FPSCALE,  128, SPEED, 256/SPEED }, // right circle 256 frames
  {0,0,0} // end
};

/* fleet formation example
**
** 3.  4       W  W W  W
** 3.  6      w w w w w w
** 2.  8    w w w w w w w w
** 1. 10  v v v v v v v v v v
** 4. 10  v v v v v v v v v v
**
** 1. v v v v v        v v v v v
**           /          \
**           ->        <-
** 2.    <- O <- w w w w w w w w
** 3. w W w W w w W w W w -> O ->
** 4. v v v v v        v v v v v
**           /          \
**           ->        <-
*/

struct path_types
{
  struct path_segment *path;
};

struct path_types Path_types[] =
{
  [0] = {stage1_convoy},
  [1] = {stage2_convoy_left},
  [2] = {stage2_convoy_right},
  {NULL}
};

int *isin; // sine table for angles 0-255

struct starship
{
  int x,y; // current coordinates of this starship (x256)
  uint8_t a; // current angle of movement
  int state; // the state number
  int prepare; // prepare countdown
  int sprite; // sprite number which is used to display this starship
  int shape; // sprite base carrying the shape
  int path_type; // current path type
  int path_state; // state of the current path
  int path_count; // frame countdown until next path state
  int hx, hy; // home position in the fleet
};
struct starship *Starship;

// defines which members of convoy are to enter the stage
// their flight path and their position in the fleet
struct convoy
{
  int x,y;      // x,y entry point on the screen
  int hx,hy;    // x,y coordinates in fleet
  int prepare;  // time delay of the alien to prepare for convoy start
  int8_t path;  // convoy path type
  int8_t alien_type; // alien type 0-3, -1 end
};

struct convoy Convoy1[] =
{
    {160,160,  200+  1*FLEET_DISTANCE,100+3*FLEET_DISTANCE,  1, 1,0 },
    {160,160,  200+  2*FLEET_DISTANCE,100+3*FLEET_DISTANCE,  2, 1,0 },
    {160,160,  200+  3*FLEET_DISTANCE,100+3*FLEET_DISTANCE,  3, 1,0 },
    {160,160,  200+  4*FLEET_DISTANCE,100+3*FLEET_DISTANCE,  4, 1,0 },
    {160,160,  200+  5*FLEET_DISTANCE,100+3*FLEET_DISTANCE,  5, 1,0 },
    {600,160,  200+  6*FLEET_DISTANCE,100+3*FLEET_DISTANCE,  1, 2,0 },
    {600,160,  200+  7*FLEET_DISTANCE,100+3*FLEET_DISTANCE,  2, 2,0 },
    {600,160,  200+  8*FLEET_DISTANCE,100+3*FLEET_DISTANCE,  3, 2,0 },
    {600,160,  200+  9*FLEET_DISTANCE,100+3*FLEET_DISTANCE,  4, 2,0 },
    {600,160,  200+ 10*FLEET_DISTANCE,100+3*FLEET_DISTANCE,  5, 2,0 },

    {600,160,  200+  2*FLEET_DISTANCE,100+2*FLEET_DISTANCE, 50+ 0, 2,1 },
    {600,160,  200+  3*FLEET_DISTANCE,100+2*FLEET_DISTANCE, 50+ 1, 2,1 },
    {600,160,  200+  4*FLEET_DISTANCE,100+2*FLEET_DISTANCE, 50+ 2, 2,1 },
    {600,160,  200+  5*FLEET_DISTANCE,100+2*FLEET_DISTANCE, 50+ 3, 2,1 },
    {600,160,  200+  6*FLEET_DISTANCE,100+2*FLEET_DISTANCE, 50+ 4, 2,1 },
    {600,160,  200+  7*FLEET_DISTANCE,100+2*FLEET_DISTANCE, 50+ 5, 2,1 },
    {600,160,  200+  8*FLEET_DISTANCE,100+2*FLEET_DISTANCE, 50+ 6, 2,1 },
    {600,160,  200+  9*FLEET_DISTANCE,100+2*FLEET_DISTANCE, 50+ 7, 2,1 },

    {160,160,  200+  3*FLEET_DISTANCE,100+1*FLEET_DISTANCE, 100+ 0, 1,1 },
    {160,160,  200+  6*FLEET_DISTANCE/2,100+0*FLEET_DISTANCE, 100+ 1, 1,3 },
    {160,160,  200+  4*FLEET_DISTANCE,100+1*FLEET_DISTANCE, 100+ 2, 1,1 },
    {160,160,  200+  9*FLEET_DISTANCE/2,100+0*FLEET_DISTANCE, 100+ 3, 1,3 },
    {160,160,  200+  5*FLEET_DISTANCE,100+1*FLEET_DISTANCE, 100+ 4, 1,1 },
    {160,160,  200+  6*FLEET_DISTANCE,100+1*FLEET_DISTANCE, 100+ 5, 1,1 },
    {160,160,  200+ 12*FLEET_DISTANCE/2,100+0*FLEET_DISTANCE, 100+ 6, 1,3 },
    {160,160,  200+  7*FLEET_DISTANCE,100+1*FLEET_DISTANCE, 100+ 7, 1,1 },
    {160,160,  200+ 15*FLEET_DISTANCE/2,100+0*FLEET_DISTANCE, 100+ 8, 1,3 },
    {160,160,  200+  8*FLEET_DISTANCE,100+1*FLEET_DISTANCE, 100+ 9, 1,1 },

    {160,160,  200+  1*FLEET_DISTANCE,100+4*FLEET_DISTANCE, 150+ 1, 1,0 },
    {160,160,  200+  2*FLEET_DISTANCE,100+4*FLEET_DISTANCE, 150+ 2, 1,0 },
    {160,160,  200+  3*FLEET_DISTANCE,100+4*FLEET_DISTANCE, 150+ 3, 1,0 },
    {160,160,  200+  4*FLEET_DISTANCE,100+4*FLEET_DISTANCE, 150+ 4, 1,0 },
    {160,160,  200+  5*FLEET_DISTANCE,100+4*FLEET_DISTANCE, 150+ 5, 1,0 },
    {600,160,  200+  6*FLEET_DISTANCE,100+4*FLEET_DISTANCE, 150+ 1, 2,0 },
    {600,160,  200+  7*FLEET_DISTANCE,100+4*FLEET_DISTANCE, 150+ 2, 2,0 },
    {600,160,  200+  8*FLEET_DISTANCE,100+4*FLEET_DISTANCE, 150+ 3, 2,0 },
    {600,160,  200+  9*FLEET_DISTANCE,100+4*FLEET_DISTANCE, 150+ 4, 2,0 },
    {600,160,  200+ 10*FLEET_DISTANCE,100+4*FLEET_DISTANCE, 150+ 5, 2,0 },

    {  0,0,     0,0,   0, 0,-1} // end (alien type -1)
};

struct convoy Convoy_demo[] =
{
    {160,160,  20,20,  1, 1,0 },
    {160,160,  40,20,  2, 1,1 },
    {160,160,  60,20,  3, 1,2 },
    {160,160,  80,20,  4, 1,3 },
    {160,160, 100,20,  5, 1,0 },
    {160,160, 120,20,  6, 1,1 },
    {160,160, 140,20,  7, 1,2 },
    {160,160, 160,20,  8, 1,3 },

    {600,160, 180,20,  1, 2,0 },
    {600,160, 200,20,  2, 2,1 },
    {600,160, 220,20,  3, 2,2 },
    {600,160, 240,20,  4, 2,3 },
    {600,160, 260,20,  5, 2,0 },
    {600,160, 280,20,  6, 2,1 },
    {600,160, 300,20,  7, 2,2 },
    {600,160, 320,20,  8, 2,3 },

    {  0,0,     0,0,   0, 0,-1} // end (alien type -1)
};

void create_sine_table()
{
  int i;
  isin = (int) malloc(256 * sizeof(int));
  for(i = 0; i < 255; i++)
    isin[i] = (sin(i * 2.0 * M_PI / 256.0) * (1.0*FPSCALE) + 0.5);
}

void create_ships()
{
  int i;
  int path_type;
  struct convoy *convoy;
  struct path_segment *path;
  Starship = (struct starship *) malloc(SHIPS_MAX * sizeof(struct starship) );

  for(i = 0; i < SHIPS_MAX; i++)
    Starship[i].state = S_NONE;

  convoy = Convoy1;
  for(i = 0; i < 50; i++)
  {
    if( convoy[i].alien_type == -1)
      return;
      // continue; // abort for-loop
    Starship[i].x = convoy[i].x * FPSCALE; // where it will enter screen
    Starship[i].y = convoy[i].y * FPSCALE;
    Starship[i].hx = convoy[i].hx * FPSCALE; // fleet home position
    Starship[i].hy = convoy[i].hy * FPSCALE;
    Starship[i].state = S_ALIEN_PREPARE;
    Starship[i].prepare = convoy[i].prepare * CONVOY_DISTANCE / SPEED;
    Starship[i].sprite = 40+i;
    Starship[i].shape = (convoy[i].alien_type & 3)*4; // shape base, added rotation 0-3
    Starship[i].path_type = convoy[i].path; // type 0 path
    path = Path_types[Starship[i].path_type].path;
    Starship[i].a = path[0].a;
    Starship[i].path_state = 0;
    Starship[i].path_count = path[0].n;
  }
}

// calculate next frame x y for the starship
void ship_convoy(struct starship *s)
{
  int v;
  struct path_segment *path;
  path = Path_types[s->path_type].path;
  v = path[s->path_state].v;
  if( s->path_count )
  {
    s->path_count--;
    c2.sprite_link_content(s->shape + (((s->a+32)/64)&3), s->sprite);
    s->x += isin[(64 + s->a) & 255] * v / FPSCALE; // cos
    s->y -= isin[      s->a  & 255] * v / FPSCALE; // sin
    s->a += path[s->path_state].r; // rotate
    c2.Sprite[s->sprite]->x = s->x / FPSCALE;
    c2.Sprite[s->sprite]->y = s->y / FPSCALE;
  }
  else
  {
    if( path[s->path_state+1].n > 0 )
    {
      s->path_state++;
      s->path_count = path[s->path_state].n;
      s->a  = path[s->path_state].a;
      c2.sprite_link_content(s->shape + (((s->a+32)/64)&3), s->sprite);
      s->x += isin[(64 + s->a) & 255] * v / FPSCALE; // cos
      s->y -= isin[      s->a  & 255] * v / FPSCALE; // sin
      s->a += path[s->path_state].r; // rotate
      c2.Sprite[s->sprite]->x = s->x / FPSCALE;
      c2.Sprite[s->sprite]->y = s->y / FPSCALE;
    }
    else
    {
#if 0
      // alien directly to home
      // FIXME: this is to early, "HOMING" state is skipped
      s->a = 192; // orient alien down
      // c2.sprite_link_content(s->shape + (((s->a+32)/64)&3), s->sprite);
      c2.sprite_link_content(s->shape + 3, s->sprite); // orient alien down
      s->state = S_ALIEN_HOME;
#endif
      s->state = S_ALIEN_HOMING;
    }
  }
}

void ship_prepare(struct starship *s)
{
  if(s->prepare > 0)
    s->prepare--;
  else
    s->state = S_ALIEN_CONVOY;
}

void ship_homing(struct starship *s)
{
  if(s->x > s->hx)
    s->x -= SPEED * FPSCALE;
  if(s->x < s->hx)
    s->x += SPEED * FPSCALE;
  if(s->y > s->hy)
    s->y -= SPEED * FPSCALE;
  if(s->y < s->hy)
    s->y += SPEED * FPSCALE;
  if(s->x == s->hx && s->y == s->hy)
    s->state = S_ALIEN_HOME;
  s->a = 192; // orient alien down
  // c2.sprite_link_content(s->shape + (((s->a+32)/64)&3), s->sprite);
  c2.sprite_link_content(s->shape + 3, s->sprite); // orient alien down
  c2.Sprite[s->sprite]->x = s->x / FPSCALE;
  c2.Sprite[s->sprite]->y = s->y / FPSCALE;
}

// fly the ship in the fleet
void ship_fleet(struct starship *s)
{
   c2.Sprite[s->sprite]->x = s->hx / FPSCALE;
   c2.Sprite[s->sprite]->y = s->hy / FPSCALE;
}

void ship_frame(struct starship *s)
{
  switch(s->state)
  {
    case S_NONE:
      return;
    case S_ALIEN_PREPARE:
      ship_prepare(s);
      break;
    case S_ALIEN_CONVOY:
      ship_convoy(s);
      break;
    case S_ALIEN_HOMING:
      ship_homing(s);
      break;
    case S_ALIEN_HOME:
      ship_fleet(s);
      break;
  }
}

void setup()
{
  int i;
  c2.init();
  c2.alloc_sprites(SPRITE_MAX);
  create_sine_table();
  create_ships();

  #if 1
    for(i = 0; i < c2.sprite_max && i < N_SHAPES; i++)
      c2.shape_to_sprite(&Shape[i]);
    for(i = c2.n_sprites; i < c2.sprite_max; i++)
      c2.sprite_clone(34); // sprite 34 is big enough to allow reshaping with smaller ones
    for(i = 0; i < c2.n_sprites; i++)
    {
      c2.Sprite[i]->x = 0 + 32*(i&3);
      c2.Sprite[i]->y = 0 + 12*i;
    }
  #endif

  if(1)
  {
  // enable video fetching after all the
  // pointers have been correctly sat.
    c2.sprite_refresh();
  }
  // prevents random RAM content from
  // causing extensive fetching, and slowing
  // down CPU
  //videodisplay_reg = &(scanlines[0][0]);
  // this is needed for vgatext
  // to disable textmode and enable bitmap
  *c2.cntrl_reg = 0b11000000; // enable video, yes bitmap, no text mode, no cursor
  // try it with text to "see" what's going
  // on with linked list :)
  //*c2.cntrl_reg = 0b11100000; // enable video, yes bitmap, yes text mode, no cursor
}

void loop()
{
  int i;

  for(i = 0; i < SHIPS_MAX; i++)
    ship_frame( &(Starship[i]) );

  while((*c2.vblank_reg & 0x80) == 0);
  c2.sprite_refresh();
  while((*c2.vblank_reg & 0x80) != 0);
  //delay(400);
}

