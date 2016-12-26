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

// global speed 1/2/4/8 (more=faster)
#define SPEED 4

// fleet drift (more = SLOWER)
#define FLEET_DRIFT 4

// alien distance in convoy
#define CONVOY_DISTANCE 24

// alien x-distance in fleet
#define FLEET_DISTANCE 20

// numver of different attack formations in the fleet
#define FLEET_MAX_GROUP 10

// showcase all sprites on the left (debug)
#define SHOWCASE_SPRITES 0

Compositing c2;

// starship states
enum 
{ 
  S_NONE,
  S_ALIEN_PREPARE,
  S_ALIEN_CONVOY, S_ALIEN_HOMING, S_ALIEN_HOME, S_ALIEN_ATTACK,
  S_ALIEN_EXPLODING, S_ALIEN_DEAD,
  S_BOMB, S_MISSILE, S_SHIP,
};

int *isin; // sine table for angles 0-255
uint8_t *iatan; // arctan table 0-FPSCALE

struct fleet
{
  int x,y;
  int xmin,xmax;
  int xd; // x-direction
};

struct fleet Fleet =
{
  200*FPSCALE,32*FPSCALE, // initial xy
  160*FPSCALE,400*FPSCALE, // min-max x
  SPEED*FPSCALE/FLEET_DRIFT // initial x-dir
};

// here ship will publish its x/y coordinates
struct ship
{
  int x,y;
};

struct ship Ship =
{
  400*FPSCALE,400*FPSCALE // ship coordinates
};

struct path_segment
{
  int v; // velocity, v = FPSCALE --> 1 pixel/frame
  uint8_t a; // initial angle 0-255 covers 0-360 degrees, 0->right, 64->up, 128->left, 192->down
  int8_t r; // rotation (angle increment)
  int n; // how many frames to run on this path segment, 0 for last
};

struct path_segment stage1_demo[] =
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
  {SPEED*FPSCALE,   0, SPEED, 126/SPEED }, // left helf-circle 128 frames
  {SPEED*FPSCALE, 128,-SPEED, 128/SPEED }, // right helf-circle 128 frames
  {0,0,0} // end
};

struct path_segment stage2_convoy_right[] =
{
  {SPEED*FPSCALE,  120, 0,     128/SPEED }, // right slightly up 128 frames
  {SPEED*FPSCALE,  128,-SPEED, 256/SPEED }, // left circle 256 frames
  {SPEED*FPSCALE,  120, 0,     128/SPEED }, // right slightly up 128 frames
  {SPEED*FPSCALE,  128, SPEED, 256/SPEED }, // right circle 256 frames
  {SPEED*FPSCALE,  128,-SPEED, 128/SPEED }, // left half-circle 128 frames
  {SPEED*FPSCALE,    0, SPEED, 128/SPEED }, // right half-circle 128 frames
  {0,0,0} // end
};

struct path_segment stage1_wave1_left[] =
{
  {SPEED*FPSCALE,  176, 0,     256/SPEED }, // down left 256 frames
  {SPEED*FPSCALE,  176, SPEED,  80/SPEED }, // right circle 80 frames
  {SPEED*FPSCALE,    8, 0,     128/SPEED }, // right up 128 frames
  {SPEED*FPSCALE,    0, SPEED,  64/SPEED }, // right circle 64 frames
  {SPEED*FPSCALE,   64, 0,      96/SPEED }, // up 96 frames
  {0,0,0} // end
};

struct path_segment stage1_wave1_right[] =
{
  {SPEED*FPSCALE,  208, 0,     256/SPEED }, // down right 256 frames
  {SPEED*FPSCALE,  208,-SPEED,  80/SPEED }, // left circle 80 frames
  {SPEED*FPSCALE,  120, 0,     128/SPEED }, // left up 128 frames
  {SPEED*FPSCALE,  128,-SPEED,  64/SPEED }, // left circle 64 frames
  {SPEED*FPSCALE,   64, 0,      96/SPEED }, // up 96 frames
  {0,0,0} // end
};

struct path_segment alien_attack_straight_down[] =
{
  {SPEED*FPSCALE,  192, 0,     512/SPEED }, // straight down 512frames
  {0,0,0} // end
};

struct path_segment alien_attack_small_vibration[] =
{
  {SPEED*FPSCALE,  208, 0,      64/SPEED }, // down right 64 frames
  {SPEED*FPSCALE,  208,-SPEED,  32/SPEED }, // left circle 32 frames
  {SPEED*FPSCALE,  176, SPEED,  32/SPEED }, // right circle 32 frames
  {SPEED*FPSCALE,  208,-SPEED,  32/SPEED }, // left circle 32 frames
  {SPEED*FPSCALE,  176, SPEED,  32/SPEED }, // right circle 32 frames
  {SPEED*FPSCALE,  208,-SPEED,  32/SPEED }, // left circle 32 frames
  {SPEED*FPSCALE,  176, SPEED,  32/SPEED }, // right circle 32 frames
  {SPEED*FPSCALE,  208,-SPEED,  32/SPEED }, // left circle 32 frames
  {SPEED*FPSCALE,  176, SPEED,  32/SPEED }, // right circle 32 frames
  {SPEED*FPSCALE,  208,-SPEED,  32/SPEED }, // left circle 32 frames
  {SPEED*FPSCALE,  176, SPEED,  32/SPEED }, // right circle 32 frames
  {SPEED*FPSCALE,  208,-SPEED,  32/SPEED }, // left circle 32 frames
  {SPEED*FPSCALE,  176, SPEED,  32/SPEED }, // right circle 32 frames
  {0,0,0} // end
};

struct path_segment alien_attack_medium_vibration[] =
{
  {SPEED*FPSCALE,  224, 0,      64/SPEED }, // down right 64 frames
  {SPEED*FPSCALE,  224,-SPEED,  64/SPEED }, // left circle 64 frames
  {SPEED*FPSCALE,  160, SPEED,  64/SPEED }, // right circle 64 frames
  {SPEED*FPSCALE,  224,-SPEED,  64/SPEED }, // left circle 64 frames
  {SPEED*FPSCALE,  160, SPEED,  64/SPEED }, // right circle 64 frames
  {SPEED*FPSCALE,  224,-SPEED,  64/SPEED }, // left circle 64 frames
  {SPEED*FPSCALE,  160, SPEED,  64/SPEED }, // right circle 64 frames
  {SPEED*FPSCALE,  224,-SPEED,  64/SPEED }, // left circle 64 frames
  {SPEED*FPSCALE,  160, SPEED,  64/SPEED }, // right circle 64 frames
  {SPEED*FPSCALE,  224,-SPEED,  64/SPEED }, // left circle 64 frames
  {SPEED*FPSCALE,  160, SPEED,  64/SPEED }, // right circle 64 frames
  {0,0,0} // end
};

struct path_segment alien_attack_zig_zag_return[] =
{
  {SPEED*FPSCALE,  223, 0,     128/SPEED }, // down right 64 frames
  {SPEED*FPSCALE,  161, 0,     128/SPEED }, // down left 64 frames
  {0,0,0} // end
};

struct path_segment alien_attack_zig_zag_thru[] =
{
  {SPEED*FPSCALE,  223, 0,     128/SPEED }, // down right 64 frames
  {SPEED*FPSCALE,  161, 0,     128/SPEED }, // down left 64 frames
  {SPEED*FPSCALE,  223, 0,     128/SPEED }, // down right 64 frames
  {SPEED*FPSCALE,  161, 0,     128/SPEED }, // down left 64 frames
  {SPEED*FPSCALE,  223, 0,     128/SPEED }, // down right 64 frames
  {SPEED*FPSCALE,  161, 0,     128/SPEED }, // down left 64 frames
  {SPEED*FPSCALE,  223, 0,     128/SPEED }, // down right 64 frames
  {SPEED*FPSCALE,  161, 0,     128/SPEED }, // down left 64 frames
  {SPEED*FPSCALE,  223, 0,     128/SPEED }, // down right 64 frames
  {SPEED*FPSCALE,  161, 0,     128/SPEED }, // down left 64 frames
  {0,0,0} // end
};

struct path_segment alien_attack_zig_zag_small_circle[] =
{
  {SPEED*FPSCALE,  223, 0,     128/SPEED }, // down right 64 frames
  {SPEED*FPSCALE,  161, 0,     128/SPEED }, // down left 64 frames
  {SPEED*FPSCALE,  223, 0,     128/SPEED }, // down right 64 frames
  {SPEED*FPSCALE,  161, 0,     128/SPEED }, // down left 64 frames
  {SPEED*FPSCALE,  223, SPEED, 256/SPEED }, // left circle 64 frames
  {SPEED*FPSCALE,  223, 0,     128/SPEED }, // down right 64 frames
  {SPEED*FPSCALE,  161, 0,     128/SPEED }, // down left 64 frames
  {SPEED*FPSCALE,  223, 0,     128/SPEED }, // down right 64 frames
  {SPEED*FPSCALE,  161, 0,     128/SPEED }, // down left 64 frames
  {SPEED*FPSCALE,  223, 0,     128/SPEED }, // down right 64 frames
  {SPEED*FPSCALE,  161, 0,     128/SPEED }, // down left 64 frames
  {0,0,0} // end
};

struct path_segment alien_attack_zig_zag_big_circle[] =
{
  {SPEED*FPSCALE,  223, 0,      128/SPEED }, // down right 64 frames
  {SPEED*FPSCALE,  161, 0,      128/SPEED }, // down left 64 frames
  {SPEED*FPSCALE,  223, 0,      128/SPEED }, // down right 64 frames
  {SPEED*FPSCALE,  161, 0,      128/SPEED }, // down left 64 frames
  {SPEED*FPSCALE,  223, SPEED/2,512/SPEED }, // left circle 64 frames
  {SPEED*FPSCALE,  223, 0,      128/SPEED }, // down right 64 frames
  {SPEED*FPSCALE,  161, 0,      128/SPEED }, // down left 64 frames
  {SPEED*FPSCALE,  223, 0,      128/SPEED }, // down right 64 frames
  {SPEED*FPSCALE,  161, 0,      128/SPEED }, // down left 64 frames
  {SPEED*FPSCALE,  223, 0,      128/SPEED }, // down right 64 frames
  {SPEED*FPSCALE,  161, 0,      128/SPEED }, // down left 64 frames
  {0,0,0} // end
};

struct path_types
{
  struct path_segment *path;
  int orientation; // should the sprite be reshaped (for angular orientation)
};

struct path_types Path_types[] =
{
  [0] = {stage1_convoy,1},
  [1] = {stage2_convoy_left,1},
  [2] = {stage2_convoy_right,1},
  [3] = {stage1_wave1_left,1},
  [4] = {stage1_wave1_right,1},
  [5] = {alien_attack_straight_down,0}, // go down straight
  [6] = {alien_attack_small_vibration,0}, // go down with small vibration
  [7] = {alien_attack_medium_vibration,0}, // go down with larger vibration
  [8] = {alien_attack_zig_zag_return,1}, // small zig-zag and return
  [9] = {alien_attack_zig_zag_thru,0}, // go down zig-zag way all way thru
 [10] = {alien_attack_zig_zag_small_circle,1}, // go down zig-zag way all way thru
 [11] = {alien_attack_zig_zag_big_circle,1}, // go down zig-zag way all way thru
  {NULL}
};


/* fleet formation example
**
** 3.  4       W  W W  W
** 3.  6      w w w w w w
** 2.  8    w w w w w w w w
** 1. 10  v v v v v v v v v v
** 4. 10  v v v v v v v v v v
**
** the attack groups
**
** 3.  4       1  2 3  4
** 3.  6      1 1 2 3 4 4
** 2.  8    5 5 6 6 7 7 8 8
** 1. 10  5 5 5 6 6 7 7 8 8 8
** 4. 10  5 5 5 6 6 7 7 8 8 8
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


struct starship
{
  int x,y; // current coordinates of this starship (x256)
  uint8_t a; // current angle of movement
  int v; // the speed (usually SPEED*FPSCALE). 1*FPSCALE -> 1 pixel per frame
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
  int group;    // group membership
  int prepare;  // time delay of the alien to prepare for convoy start
  int8_t path;  // convoy path type
  int8_t alien_type; // alien type 0-3, -1 end
};

struct convoy Convoy1[] =
{
    {380,  0,   1*FLEET_DISTANCE,3*FLEET_DISTANCE,5,  1, 3,0 },
    {380,  0,   2*FLEET_DISTANCE,3*FLEET_DISTANCE,5,  2, 3,0 },
    {380,  0,   3*FLEET_DISTANCE,3*FLEET_DISTANCE,5,  3, 3,0 },
    {380,  0,   4*FLEET_DISTANCE,3*FLEET_DISTANCE,6,  4, 3,0 },
    {380,  0,   5*FLEET_DISTANCE,3*FLEET_DISTANCE,6,  5, 3,0 },
    {420,  0,   6*FLEET_DISTANCE,3*FLEET_DISTANCE,7,  1, 4,0 },
    {420,  0,   7*FLEET_DISTANCE,3*FLEET_DISTANCE,7,  2, 4,0 },
    {420,  0,   8*FLEET_DISTANCE,3*FLEET_DISTANCE,8,  3, 4,0 },
    {420,  0,   9*FLEET_DISTANCE,3*FLEET_DISTANCE,8,  4, 4,0 },
    {420,  0,  10*FLEET_DISTANCE,3*FLEET_DISTANCE,8,  5, 4,0 },

    {640,290,   2*FLEET_DISTANCE,2*FLEET_DISTANCE,5, 40+ 0, 2,1 },
    {640,290,   3*FLEET_DISTANCE,2*FLEET_DISTANCE,5, 40+ 1, 2,1 },
    {640,290,   4*FLEET_DISTANCE,2*FLEET_DISTANCE,6, 40+ 2, 2,1 },
    {640,290,   5*FLEET_DISTANCE,2*FLEET_DISTANCE,6, 40+ 3, 2,1 },
    {640,290,   6*FLEET_DISTANCE,2*FLEET_DISTANCE,7, 40+ 4, 2,1 },
    {640,290,   7*FLEET_DISTANCE,2*FLEET_DISTANCE,7, 40+ 5, 2,1 },
    {640,290,   8*FLEET_DISTANCE,2*FLEET_DISTANCE,8, 40+ 6, 2,1 },
    {640,290,   9*FLEET_DISTANCE,2*FLEET_DISTANCE,8, 40+ 7, 2,1 },

    {160,260,   3*FLEET_DISTANCE,  1*FLEET_DISTANCE,1,  90+ 0, 1,1 },
    {160,260,   6*FLEET_DISTANCE/2,0*FLEET_DISTANCE,1,  90+ 1, 1,3 },
    {160,260,   4*FLEET_DISTANCE,  1*FLEET_DISTANCE,1,  90+ 2, 1,1 },
    {160,260,   9*FLEET_DISTANCE/2,0*FLEET_DISTANCE,2,  90+ 3, 1,3 },
    {160,260,   5*FLEET_DISTANCE,  1*FLEET_DISTANCE,2,  90+ 4, 1,1 },
    {160,260,   6*FLEET_DISTANCE,  1*FLEET_DISTANCE,3,  90+ 5, 1,1 },
    {160,260,  12*FLEET_DISTANCE/2,0*FLEET_DISTANCE,3,  90+ 6, 1,3 },
    {160,260,   7*FLEET_DISTANCE,  1*FLEET_DISTANCE,4,  90+ 7, 1,1 },
    {160,260,  15*FLEET_DISTANCE/2,0*FLEET_DISTANCE,4,  90+ 8, 1,3 },
    {160,260,   8*FLEET_DISTANCE,  1*FLEET_DISTANCE,4,  90+ 9, 1,1 },

    {380,  0,   1*FLEET_DISTANCE,4*FLEET_DISTANCE,5, 140+ 1, 3,0 },
    {380,  0,   2*FLEET_DISTANCE,4*FLEET_DISTANCE,5, 140+ 2, 3,0 },
    {380,  0,   3*FLEET_DISTANCE,4*FLEET_DISTANCE,5, 140+ 3, 3,0 },
    {380,  0,   4*FLEET_DISTANCE,4*FLEET_DISTANCE,6, 140+ 4, 3,0 },
    {380,  0,   5*FLEET_DISTANCE,4*FLEET_DISTANCE,6, 140+ 5, 3,0 },
    {420,  0,   6*FLEET_DISTANCE,4*FLEET_DISTANCE,7, 140+ 1, 4,0 },
    {420,  0,   7*FLEET_DISTANCE,4*FLEET_DISTANCE,7, 140+ 2, 4,0 },
    {420,  0,   8*FLEET_DISTANCE,4*FLEET_DISTANCE,8, 140+ 3, 4,0 },
    {420,  0,   9*FLEET_DISTANCE,4*FLEET_DISTANCE,8, 140+ 4, 4,0 },
    {420,  0,  10*FLEET_DISTANCE,4*FLEET_DISTANCE,8, 140+ 5, 4,0 },

    {  0,0,     0,0,0,   0, 0,-1} // end (alien type -1)
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
  for(i = 0; i < 256; i++)
    isin[i] = sin(i * 2.0 * M_PI / 256.0) * (1.0*FPSCALE) + 0.5;
  // sine calculation is suspected to have some
  // probably C math library problem
  // like some values are sometimes calculated wrong.
  // printing sine table seems it is OK
  // probably we have bugs in another place.
  // bug manifests as sometimes angular values for X are wrong
  // so convoy gets a bit messed up and attack
  // paths sometimes drift too much to the left 
  #if 0
  for(i = 0; i < 256; i++)
    printf("isin[%d] = %d\n", i, isin[i]);
  #endif
}

void create_atan_table()
{
  int i;
  iatan = (int) malloc(FPSCALE * sizeof(int));
  for(i = 0; i < FPSCALE; i++)
    iatan[i] = atan(i * (1.0 / FPSCALE)) * 256.0 / (2 * M_PI) + 0.5;
}

struct starship *find_free()
{
  int i;
  for(i = 0; i < SHIPS_MAX; i++)
  {
    if( Starship[i].state == S_NONE )
      return &(Starship[i]);
  }
  return NULL;
}

void create_aliens()
{
  int i;
  int path_type;
  struct convoy *convoy;
  struct path_segment *path;
  Starship = (struct starship *) malloc(SHIPS_MAX * sizeof(struct starship) );

  for(i = 0; i < SHIPS_MAX; i++)
  {
    Starship[i].state = S_NONE;
    Starship[i].sprite = 40+i;
  }

  convoy = Convoy1;
  for(i = 0; i < 50; i++)
  {
    if( convoy[i].alien_type == -1)
      return; // abort for-loop --- todo this must be done better
    Starship[i].x = convoy[i].x * FPSCALE; // where it will enter screen
    Starship[i].y = convoy[i].y * FPSCALE;
    Starship[i].hx = convoy[i].hx * FPSCALE; // fleet home position
    Starship[i].hy = convoy[i].hy * FPSCALE;
    Starship[i].state = S_ALIEN_PREPARE;
    Starship[i].prepare = convoy[i].prepare * CONVOY_DISTANCE / SPEED;
    Starship[i].sprite = 40+i;
    Starship[i].shape = (convoy[i].alien_type & 3)*4; // shape base, added rotation 0-3
    Starship[i].path_type = convoy[i].path; // path to follow
    path = Path_types[Starship[i].path_type].path;
    Starship[i].a = path[0].a;
    Starship[i].v = path[0].v;
    Starship[i].path_state = 0;
    Starship[i].path_count = path[0].n;
  }
}

// angular move the ship with velocity v
void object_angular_move(struct starship *s)
{
  uint8_t xa = 64 + s->a;
  s->x += isin[xa] * s->v / FPSCALE; // cos
  s->y -= isin[s->a] * s->v / FPSCALE; // sin
  c2.Sprite[s->sprite]->x = s->x / FPSCALE;
  c2.Sprite[s->sprite]->y = s->y / FPSCALE;
}

// bomb starting from x,y, fly at angle a
void bomb_create(int x, int y, uint8_t a)
{
  struct starship *s;
  s = find_free();
  if(s == NULL)
    return;
  s->x = x;
  s->y = y;
  s->a = a;
  s->v = SPEED*FPSCALE*5/4;
  s->state = S_BOMB;
  c2.sprite_link_content(33, s->sprite);
  c2.Sprite[s->sprite]->x = s->x / FPSCALE;
  c2.Sprite[s->sprite]->y = s->y / FPSCALE;
}

void bomb_move(struct starship *s)
{
  if(s->x < 10*FPSCALE || s->x > 640*FPSCALE || s->y > 480*FPSCALE || s->y < 10*FPSCALE)
  {
    s->state = S_NONE;
    c2.Sprite[s->sprite]->y = 640; // off-screen, invisible
    return;
  }
  object_angular_move(s);
}

// missile starting from x,y
void missile_create(int x, int y)
{
  struct starship *s;
  s = find_free();
  if(s == NULL)
    return;
  s->x = x;
  s->y = y;
  s->a = 64; // fly up
  s->v = 3*SPEED*FPSCALE;
  s->state = S_MISSILE;
  c2.sprite_link_content(24, s->sprite);
  c2.Sprite[s->sprite]->x = s->x / FPSCALE;
  c2.Sprite[s->sprite]->y = s->y / FPSCALE;
}

// alien collision
// returns non-null pointer 
// to the alien which is hit by the missile s
struct starship *alien_hit(struct starship *s)
{
  int i;
  struct starship *as;
  struct convoy *convoy = Convoy1;
  int xr = 4*FPSCALE, yr = 8*FPSCALE; // collision range
  for(i = 0; i < SHIPS_MAX; i++)
  {
    if( convoy[i].alien_type == -1)
      return NULL; // abort for-loop --- todo this must be done better
    as = &(Starship[i]);
    // is this ship alien alive?
    if(as->state >= S_ALIEN_CONVOY && as->state <= S_ALIEN_ATTACK)
    {
      if(as->x - xr < s->x && as->x + xr > s->x
      && as->y - yr < s->y && as->y + yr > s->y)
        return as;
    }
  }
  return NULL; // no alien is hit
}

void missile_move(struct starship *s)
{
  struct starship *ah = alien_hit(s);
  if(ah != NULL)
  {
    ah->state = S_NONE;
    // todo: create alien explosion
    c2.Sprite[ah->sprite]->y = 640; // alien off-screen, invisible
  }
  if(s->x < 10*FPSCALE || s->x > 640*FPSCALE || s->y > 480*FPSCALE || s->y < 10*FPSCALE || ah != NULL)
  {
    s->state = S_NONE;
    c2.Sprite[s->sprite]->y = 640; // off-screen, invisible
    return;
  }
  object_angular_move(s);
}

// calculate next frame x y for the starship
// reshape=0 -> do not change shape on direction change
void alien_convoy(struct starship *s)
{
  int v;
  uint8_t xa;
  struct path_segment *path;
  path = Path_types[s->path_type].path;
  int reshape = Path_types[s->path_type].orientation;
  s->v = path[s->path_state].v;
  if( s->path_count > 0 )
  {
    s->path_count--;
    if(reshape != 0)
      c2.sprite_link_content(s->shape + (((s->a+32)/64)&3), s->sprite);
    object_angular_move(s);
    s->a += path[s->path_state].r; // rotate
  }
  else
  {
    if( path[s->path_state+1].n > 0 )
    {
      s->path_state++;
      s->path_count = path[s->path_state].n;
      s->a = path[s->path_state].a;
      if(reshape != 0)
        c2.sprite_link_content(s->shape + (((s->a+32)/64)&3), s->sprite);
      object_angular_move(s);
      s->a += path[s->path_state].r; // rotate
    }
    else
    {
      s->state = S_ALIEN_HOMING;
    }
  }
}

void alien_prepare(struct starship *s)
{
  if(s->prepare > 0)
    s->prepare--;
  else
    s->state = S_ALIEN_CONVOY;
}

void alien_homing(struct starship *s)
{
  int dir;
  int xd, yd;
  xd = Fleet.x + s->hx - s->x;
  yd = Fleet.y + s->hy - s->y;
  if(xd > SPEED * FPSCALE)
  {
    xd = SPEED * FPSCALE;
    dir = 0; // right
  }
  else
  {
    if(xd < -(SPEED * FPSCALE))
    {
      xd = -(SPEED * FPSCALE);
      dir = 2; // left
    }
  }

  if(yd < -(SPEED * FPSCALE))
  {
    yd = -(SPEED * FPSCALE);
    dir = 1; // up
  }
  else
  {
    if(yd > SPEED * FPSCALE)
    {
      yd = SPEED * FPSCALE;
      dir = 3; // down
    }
  }
  s->x += xd;
  s->y += yd;
  if(abs(xd) < 2*SPEED*FPSCALE/FLEET_DRIFT && abs(yd) < 2*SPEED*FPSCALE/FLEET_DRIFT)
  // if( xd == 0 && yd == 0 )
  {
    s->state = S_ALIEN_HOME;
    s->x = Fleet.x + s->hx;
    s->y = Fleet.y + s->hy;
    dir = 3; // down
  }
  s->a = dir * 64; // orient alien down
  // c2.sprite_link_content(s->shape + (((s->a+32)/64)&3), s->sprite);
  c2.sprite_link_content(s->shape + dir, s->sprite); // orient alien down
  c2.Sprite[s->sprite]->x = s->x / FPSCALE;
  c2.Sprite[s->sprite]->y = s->y / FPSCALE;
}

// calculate bomb angle from alien starship to the player's ship 
// possible angles are within 45 degrees
// if ship is out of shooting range - return 0
uint8_t aim_bomb_angle(struct starship *s)
{
  int rev = 1;
  int dx, dy;
  uint8_t a;
  int xadjust = 4*FPSCALE, yadjust = 10*FPSCALE; // x,y-adjustment for uncentered sprites
  int tangent; // tangent value
  // if ship is above alien, can't shoot
  if(Ship.y < s->y)
    return 0;
  dx = Ship.x + xadjust - s->x;
  dy = Ship.y + yadjust - s->y;
  // if ship is left, convert to the right
  if(dx < 0)
  {
    dx = -dx;
    rev = -1;
  }
  // both dx and dy should be positive now
  // angles > 45 are out of reach
  if(dx >= dy)
    return 0;
  // avoid eventual division by zero
  if(dy == 0)
    return 0;
  // calculate angle (arc-tan)
  tangent = (dx*FPSCALE)/dy;
  // sanity check
  if(tangent < 0 || tangent >= FPSCALE)
    return 0;
  a = iatan[tangent];
  // reverse sign
  if(rev > 0)
    return 192 + a;
  else
    return 192 - a;
}

// fly the ship in the fleet
void alien_fleet(struct starship *s)
{
  uint16_t rng;
  uint8_t a;

  s->x = Fleet.x + s->hx;
  s->y = Fleet.y + s->hy;

  c2.Sprite[s->sprite]->x = (Fleet.x + s->hx) / FPSCALE;
  c2.Sprite[s->sprite]->y = (Fleet.y + s->hy) / FPSCALE;

  rng = rand();

  if(rng < 50)
  {
    a = aim_bomb_angle(s);
    if(a != 0)
      bomb_create(s->x, s->y, a);
  }

  if(0)
  if(rng > 50 && rng < 100)
  {
    struct path_segment *path;
    s->path_type = 5+(rng % 7); // 5 is attack path
    // s->path_type = 11; // force type for testing
    path = Path_types[s->path_type].path;
    s->state = S_ALIEN_ATTACK;
    s->path_state = 0; // 0 resets path to the first segment of the path
    s->path_count = path[0].n; // contdown of the path segment
    s->a = path[0].a; // initial angle
  }
}

// select a group of alien ships in the fleet
// that will perform an attack, all flying
// the same path
void fleet_select_attack()
{
  uint32_t rng = rand();
  int group;
  int i;
  if(rng < 60000000)
  {
    struct convoy *convoy = Convoy1;
    group = 1 + (rng % 10); // select which group will attack
    // group = 1;
    // todo: if no ships are in this group in the fleet,
    // choose next group
    // search for all ships, find those which are members of
    // selected group and are in FLEET HOME state
    for(i = 0; i < SHIPS_MAX; i++)
    {
      if(convoy[i].alien_type == -1)
        return; // no more ships --- todo this must be done better

      if(Starship[i].state == S_ALIEN_HOME && convoy[i].group == group)
      {
        // found the candidate for the group attack
        struct starship *s = &(Starship[i]);
        struct path_segment *path;
        s->path_type = 5+(rng % 7); // 5 is attack path
        // s->path_type = 11; // force type for testing
        path = Path_types[s->path_type].path;
        s->state = S_ALIEN_ATTACK;
        s->path_state = 0; // 0 resets path to the first segment of the path
        s->path_count = path[0].n; // contdown of the path segment
        s->a = path[0].a; // initial angle
      }
    }
  }

}

void fleet_move()
{
  if(Fleet.x <= Fleet.xmin)
    Fleet.xd = (FPSCALE*SPEED/FLEET_DRIFT);

  if( Fleet.x >= Fleet.xmax)
    Fleet.xd = -(FPSCALE*SPEED/FLEET_DRIFT);

  Fleet.x += Fleet.xd;
}

// attack flight path steering of the alien ship
void alien_attack(struct starship *s)
{
  uint16_t rng = rand();
  uint8_t a;

  if(rng < 2000)
  {
    a = aim_bomb_angle(s);
    if(a != 0)
      bomb_create(s->x, s->y, a);
  }

  s->v = SPEED*FPSCALE;
  if(s->y < 480*FPSCALE)
  {
    alien_convoy(s); // should be (s,0) but there's some bug in angle?
  }
  else
  {
    s->y = 0; // jump to top of the screen
    s->a = 192; // angle down
    object_angular_move(s); // initial move
    c2.sprite_link_content(s->shape + (((s->a+32)/64)&3), s->sprite);
    s->state = S_ALIEN_HOMING;
  }
}

// ship at x,y
void ship_create(int x, int y)
{
  struct starship *s;
  s = find_free();
  if(s == NULL)
    return;
  s->x = x;
  s->y = y;
  s->a = 64; // fly up
  s->v = 0;
  s->state = S_SHIP;
  c2.sprite_link_content(29, s->sprite);
  c2.Sprite[s->sprite]->x = s->x / FPSCALE;
  c2.Sprite[s->sprite]->y = s->y / FPSCALE;
}

void ship_move(struct starship *s)
{
  uint16_t rng = rand();
  static int xdir = SPEED*FPSCALE/2; // x-direction that ship moves
  if(rng < 2000)
  {
    missile_create(Ship.x+4*FPSCALE, Ship.y);
  }
  if(s->x > 600*FPSCALE && xdir > 0)
    xdir = -SPEED*FPSCALE/2;
  if(s->x < 100*FPSCALE && xdir < 0)
    xdir =  SPEED*FPSCALE/2;
  s->x += xdir;
  Ship.x = s->x; // publish ship's new x coordinate (y stays the same)
  c2.Sprite[s->sprite]->x = s->x / FPSCALE;
  c2.Sprite[s->sprite]->y = s->y / FPSCALE;
}

void everything_move(struct starship *s)
{
  switch(s->state)
  {
    case S_NONE:
      return;
    case S_ALIEN_PREPARE:
      alien_prepare(s);
      break;
    case S_ALIEN_CONVOY:
      alien_convoy(s);
      break;
    case S_ALIEN_HOMING:
      alien_homing(s);
      break;
    case S_ALIEN_HOME:
      alien_fleet(s);
      break;
    case S_ALIEN_ATTACK:
      alien_attack(s);
      break;
    case S_BOMB:
      bomb_move(s);
      break;
    case S_MISSILE:
      missile_move(s);
      break;
    case S_SHIP:
      ship_move(s);
      break;
  }
}

void setup()
{
  int i;
  c2.init();
  c2.alloc_sprites(SPRITE_MAX);
  create_sine_table();
  create_atan_table();
  create_aliens();

  #if 1
    for(i = 0; i < c2.sprite_max && i < N_SHAPES; i++)
      c2.shape_to_sprite(&Shape[i]);
    for(i = c2.n_sprites; i < c2.sprite_max; i++)
      c2.sprite_clone(34); // sprite 34 is big enough to allow reshaping with smaller ones
    for(i = 0; i < c2.n_sprites; i++)
    {
      c2.Sprite[i]->x = 0 + 32*(i&3);
      #if SHOWCASE_SPRITES
      c2.Sprite[i]->y = 0 + 12*i;
      #else
      c2.Sprite[i]->y = 640; // off screen (invisible)
      #endif
    }
  #endif

  // create the ship, just to display something
  ship_create(Ship.x, Ship.y);

  // experimental bomb
  // bomb_create(300*FPSCALE,50*FPSCALE,191);

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

  fleet_move();
  fleet_select_attack();
  for(i = 0; i < SHIPS_MAX; i++)
    everything_move( &(Starship[i]) );

  while((*c2.vblank_reg & 0x80) == 0);
  c2.sprite_refresh();
  while((*c2.vblank_reg & 0x80) != 0);
  //delay(400);
}

