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

Compositing c2;

struct sprite_speed 
{
  int x,y;
};
struct sprite_speed *Sprite_speed;

// starship states
enum 
{ 
  S_ALIEN1_PREPARE, S_ALIEN1_CONVOY, S_ALIEN1_HOMING, S_ALIEN1_HOME,
  S_ALIEN1_ATTACK, S_ALIEN1_EXPLODING, S_ALIEN1_DEAD
};

struct path_segment
{
  int v; // velocity, v = FPSCALE --> 1 pixel/frame
  uint8_t a; // angle 0-255 covers 0-360 degrees, 0->right, 64->up, 128->left, 192->down
  int n; // how many frames to run on this path segment, 0 for last
};

struct path_segment stage1_convoy[] =
{
  {FPSCALE,   0+32, 100 }, // right 100 frames
  {FPSCALE,  64+32, 100 }, // up 100 fames
  {FPSCALE, 128+32, 100 }, // left 100 fames
  {FPSCALE, 192+32, 100 }, // down 100 fames 
  {FPSCALE,   0+32,  50 }, // right 50 frames
  {FPSCALE,  64+32,  50 }, // up 50 fames
  {FPSCALE, 128+32,  50 }, // left 50 fames
  {FPSCALE, 192+32,  50 }, // down 50 fames 
  {0,0,0} // end
};

struct path_types
{
  struct path_segment *path;
};

struct path_types Path_types[] =
{
  stage1_convoy,
  NULL
};

int *isin; // sine table for angles 0-255

struct starship
{
  int x,y; // current coordinates of this starship (x256)
  int state; // the state number
  int sprite; // sprite number which is used to display this starship
  int path_type; // current path type
  int path_state; // state of the current path
  int path_count; // frame countdown until next path state
  int hx, hy; // home position in the fleet
};
struct starship *Starship;

void create_sine_table()
{
  int i;
  isin = (int) malloc(256 * sizeof(int));
  for(i = 0; i < 255; i++)
    isin[i] = (sin(i * 2.0 * M_PI / 256.0) * (1.0*FPSCALE) + 0.5);
}

void create_ships()
{
  Starship = (struct starship *) malloc(SHIPS_MAX * sizeof(struct starship) );
  Starship[0].x = 320*FPSCALE;
  Starship[0].y = 200*FPSCALE;
  Starship[0].sprite = 0;
  Starship[0].path_state = 0;
  Starship[0].path_count = 100;
}

// calculate next frame x y for the starship
void ship_frame(struct starship *s)
{
  int v;
  if( s->path_count )
  {
    s->path_count--;
    s->x += isin[(64 + stage1_convoy[s->path_state].a) & 255]; // cosine
    s->y -= isin[      stage1_convoy[s->path_state].a  & 255]; // sine
    c2.Sprite[s->sprite]->x = s->x / FPSCALE;
    c2.Sprite[s->sprite]->y = s->y / FPSCALE;
  }
  else
  {
    if( stage1_convoy[s->path_state+1].n > 0 )
    {
      s->path_state++;
      s->path_count = stage1_convoy[s->path_state].n;
      s->x += isin[(64 + stage1_convoy[s->path_state].a) & 255]; // cosine
      s->y -= isin[      stage1_convoy[s->path_state].a  & 255]; // sine
      c2.Sprite[s->sprite]->x = s->x / FPSCALE;
      c2.Sprite[s->sprite]->y = s->y / FPSCALE;
    }
  }
}

void setup()
{
  int i;
  c2.init();
  c2.alloc_sprites(SPRITE_MAX);
  Sprite_speed = (struct sprite_speed *) malloc(SPRITE_MAX * sizeof(struct sprite_speed));
  create_sine_table();
  create_ships();

  #if 1
    for(i = 0; i < c2.sprite_max && i < N_SHAPES; i++)
      c2.shape_to_sprite(&Shape[i]);
    for(i = c2.n_sprites; i < c2.sprite_max; i++)
      c2.sprite_clone(i%N_SHAPES);
    for(i = 0; i < c2.n_sprites; i++)
    {
      //shape_to_sprite(1 + (i % 3),i);
      c2.Sprite[i]->x = 20 + (rand() % 600);
      c2.Sprite[i]->y = 20 + (rand() % 400);
      Sprite_speed[i].x = (rand() % 3)-1;
      Sprite_speed[i].y = (rand() % 3)-1;
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

  ship_frame( &(Starship[0]) );

  for(i = 0; i < c2.n_sprites; i++)
  {
    //c2.Sprite[i]->x += Sprite_speed[i].x;
    //c2.Sprite[i]->y += Sprite_speed[i].y;
    if(c2.Sprite[i]->x < -40)
    {
      Sprite_speed[i].x = 1;
      if( (rand()&7) == 0 )
        Sprite_speed[i].y = (rand()%3)-1;
    }
    if(c2.Sprite[i]->x > VGA_X_MAX)
    {
      Sprite_speed[i].x = -1;
    }

    if(c2.Sprite[i]->y < -40)
    {
      Sprite_speed[i].y = 1;
      if( (rand()&7) == 0 )
        Sprite_speed[i].x = (rand()%3)-1;
    }
    if(c2.Sprite[i]->y > VGA_Y_MAX+40)
      Sprite_speed[i].y = -1;
  }
  while((*c2.vblank_reg & 0x80) == 0);
  c2.sprite_refresh();
  while((*c2.vblank_reg & 0x80) != 0);
  //delay(400);
}

