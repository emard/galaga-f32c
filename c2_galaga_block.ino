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

Compositing c2;

// starship states
enum 
{ 
  S_NONE,
  S_ALIEN1_PREPARE, S_ALIEN1_CONVOY, S_ALIEN1_HOMING, S_ALIEN1_HOME,
  S_ALIEN1_ATTACK, S_ALIEN1_EXPLODING, S_ALIEN1_DEAD
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
  int delay;    // time delay of the convoy start
  int8_t path;  // convoy path type
  int8_t alien_type; // alien type 0-3, -1 end
};

struct convoy Convoy1[] =
{
    {160,160,  10,20,  0, 1,0 },
    {160,160,  20,20,  0, 1,0 },
    {160,160,  30,20,  0, 1,0 },
    {160,160,  40,20,  0, 1,0 },
    {160,160,  50,20,  0, 1,0 },
    {600,160,  60,20,  0, 2,0 },
    {600,160,  70,20,  0, 2,0 },
    {600,160,  80,20,  0, 2,0 },
    {600,160,  60,20,  0, 2,0 },
    {600,160,  70,20,  0, 2,0 },
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
  struct path_segment *path;
  Starship = (struct starship *) malloc(SHIPS_MAX * sizeof(struct starship) );

  for(i = 0; i < SHIPS_MAX; i++)
    Starship[i].state = S_NONE;

  path_type=1;
  path = Path_types[path_type].path;
  for(i = 0; i < 8; i++)
  {
    Starship[i].x = (160+0*i)*FPSCALE; // left
    Starship[i].y = 240*FPSCALE;
    Starship[i].state = S_ALIEN1_PREPARE;
    Starship[i].prepare = 24*i/SPEED;
    Starship[i].sprite = 40+i;
    Starship[i].shape = (i&3)*4; // shape base, added rotation 0-3
    Starship[i].path_type = path_type; // type 0 path
    Starship[i].a = path[0].a;
    Starship[i].path_state = 0;
    Starship[i].path_count = path[0].n;
  }

  path_type=2;
  path = Path_types[path_type].path;
  for(i = 8; i < 16; i++)
  {
    Starship[i].x = (600+0*i)*FPSCALE; // right
    Starship[i].y = 240*FPSCALE;
    Starship[i].state = S_ALIEN1_PREPARE;
    Starship[i].prepare = 24*(i-8)/SPEED;
    Starship[i].sprite = 40+i;
    Starship[i].shape = (i&3)*4; // shape base, added rotation 0-3
    Starship[i].path_type = path_type; // type 0 path
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
  }
}

void ship_prepare(struct starship *s)
{
  if(s->prepare > 0)
    s->prepare--;
  else
    s->state = S_ALIEN1_CONVOY;
}

void ship_frame(struct starship *s)
{
  if(s->state == S_NONE)
    return;
  if(s->state == S_ALIEN1_PREPARE)
    ship_prepare(s);
  if(s->state == S_ALIEN1_CONVOY)
    ship_convoy(s);
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

