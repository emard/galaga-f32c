#include <stdlib.h>
#include "shape.h"

// ascii-art of the shapes
struct charcolors std_colors[] = 
{  //               RRGGBB
  {'O', RGB2PIXEL(0xFF7F00)}, // orange
  {'R', RGB2PIXEL(0xFF0000)}, // red
  {'Y', RGB2PIXEL(0xFFFF00)}, // yellow
  {'C', RGB2PIXEL(0x00FFFF)}, // cyan
  {'G', RGB2PIXEL(0x00FF00)}, // green
  {'B', RGB2PIXEL(0x0000FF)}, // blue
  {'V', RGB2PIXEL(0xFF00FF)}, // violett
  {'W', RGB2PIXEL(0xFFFFFF)}, // white
  {' ', RGB2PIXEL(0)}, // transparent
  {0, 0}
};


char *shape_alien1u[] =
{
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"WWWW    WWWW",
"WWWW    WWWW",
"WWWW    WWWW",
"WWWW    WWWW",
NULL
};

char *shape_alien1d[] =
{
"WWWW    WWWW",
"WWWW    WWWW",
"WWWW    WWWW",
"WWWW    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
NULL
};

char *shape_alien1l[] =
{
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"WWWW",
"WWWW",
"WWWW",
"WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
NULL
};

char *shape_alien1r[] =
{
"WWWW",
"WWWW",
"WWWW",
"WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"WWWW",
"WWWW",
"WWWW",
"WWWW",
NULL
};

char *shape_alien2u[] =
{
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"WWWWWWWWWWWW",
"WWWWWWWWWWWW",
"WWWWWWWWWWWW",
"WWWWWWWWWWWW",
NULL
};

char *shape_alien2d[] =
{
"WWWWWWWWWWWW",
"WWWWWWWWWWWW",
"WWWWWWWWWWWW",
"WWWWWWWWWWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
NULL
};

char *shape_alien2l[] =
{
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"WWWWWWWW",
"WWWWWWWW",
"WWWWWWWW",
"WWWWWWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
NULL
};

char *shape_alien2r[] =
{
"WWWW",
"WWWW",
"WWWW",
"WWWW",
"WWWWWWWW",
"WWWWWWWW",
"WWWWWWWW",
"WWWWWWWW",
"WWWW",
"WWWW",
"WWWW",
"WWWW",
NULL
};

char *shape_alien3u[] =
{
"        WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
"    WWWW    WWWW",
"    WWWW    WWWW",
"    WWWW    WWWW",
"    WWWW    WWWW",
"WWWW            WWWW",
"WWWW            WWWW",
"WWWW            WWWW",
"WWWW            WWWW",
NULL
};

char *shape_alien3d[] =
{
"WWWW            WWWW",
"WWWW            WWWW",
"WWWW            WWWW",
"WWWW            WWWW",
"    WWWW    WWWW",
"    WWWW    WWWW",
"    WWWW    WWWW",
"    WWWW    WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
NULL
};

char *shape_alien3l[] =
{
"        WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"WWWW",
"WWWW",
"WWWW",
"WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
NULL
};

char *shape_alien3r[] =
{
"WWWW",
"WWWW",
"WWWW",
"WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"WWWW",
"WWWW",
"WWWW",
"WWWW",
NULL
};

char *shape_alien4u[] =
{
"        WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
"    WWWWWWWWWWWW",
"    WWWWWWWWWWWW",
"    WWWWWWWWWWWW",
"    WWWWWWWWWWWW",
"WWWW            WWWW",
"WWWW            WWWW",
"WWWW            WWWW",
"WWWW            WWWW",
NULL
};

char *shape_alien4d[] =
{
"WWWW            WWWW",
"WWWW            WWWW",
"WWWW            WWWW",
"WWWW            WWWW",
"    WWWWWWWWWWWW",
"    WWWWWWWWWWWW",
"    WWWWWWWWWWWW",
"    WWWWWWWWWWWW",
"        WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
NULL
};

char *shape_alien4l[] =
{
"        WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"WWWWWWWW",
"WWWWWWWW",
"WWWWWWWW",
"WWWWWWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
NULL
};

char *shape_alien4r[] =
{
"WWWW",
"WWWW",
"WWWW",
"WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWWWWWW",
"    WWWWWWWW",
"    WWWWWWWW",
"    WWWWWWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"WWWW",
"WWWW",
"WWWW",
"WWWW",
NULL
};

char *shape_alien_radiate1[] =
{
"WWWW",
"WWWW",
"WWWW",
"WWWW",
NULL
};

char *shape_alien_radiate3[] =
{
"WWWWWWWWWWWW",
"WWWWWWWWWWWW",
"WWWWWWWWWWWW",
"WWWWWWWWWWWW",
NULL
};

char *shape_alien_radiate5[] =
{
"WWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWW",
NULL
};

char *shape_alien_radiate7[] =
{
"WWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWWWWWWWWWW",
NULL
};

char *shape_alien_radiate9[] =
{
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
NULL
};

char *shape_alien_radiate11[] =
{
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
NULL
};

char *shape_explode1[] =
{
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"WWWW    WWWW",
"WWWW    WWWW",
"WWWW    WWWW",
"WWWW    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
NULL
};

char *shape_explode2[] =
{
"        WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
"    ",
"    ",
"    ",
"    ",
"WWWW            WWWW",
"WWWW            WWWW",
"WWWW            WWWW",
"WWWW            WWWW",
"    ",
"    ",
"    ",
"    ",
"        WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
NULL
};

char *shape_bomb[] =
{
"WWWW",
"WWWW",
"WWWW",
"WWWW",
NULL
};

char *shape_missile1[] =
{
" WW ",
"  WW",
" WW ",
"WW  ",
" WW ",
"  WW",
" WW ",
"WW  ",
" WW ",
"  WW",
" WW ",
NULL
};

char *shape_missile2[] =
{
"  WW",
" WW ",
"WW  ",
" WW ",
"  WW",
" WW ",
"WW  ",
" WW ",
"  WW",
" WW ",
"WW  ",
NULL
};

char *shape_missile3[] =
{
" WW ",
"WW  ",
" WW ",
"  WW",
" WW ",
"WW  ",
" WW ",
"  WW",
" WW ",
"WW  ",
" WW ",
NULL
};

char *shape_missile4[] =
{
"WW  ",
" WW ",
"  WW",
" WW ",
"WW  ",
" WW ",
"  WW",
" WW ",
"WW  ",
" WW ",
"  WW",
NULL
};

char *shape_ship1u[] =
{
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"WWWW    WWWW",
"WWWW    WWWW",
"WWWW    WWWW",
"WWWW    WWWW",
NULL
};

char *shape_ship1d[] =
{
"WWWW    WWWW",
"WWWW    WWWW",
"WWWW    WWWW",
"WWWW    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
NULL
};

char *shape_ship1l[] =
{
"        WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
"WWWWWWWW",
"WWWWWWWW",
"WWWWWWWW",
"WWWWWWWW",
"        WWWW",
"        WWWW",
"        WWWW",
"        WWWW",
NULL
};

char *shape_ship1r[] =
{
"WWWW",
"WWWW",
"WWWW",
"WWWW",
"    WWWWWWWW",
"    WWWWWWWW",
"    WWWWWWWW",
"    WWWWWWWW",
"WWWW",
"WWWW",
"WWWW",
"WWWW",
NULL
};

char *shape_ship2u[] =
{
"    WWWW    WWWW",
"    WWWW    WWWW",
"    WWWW    WWWW",
"    WWWW    WWWW",
"    WWWW    WWWW",
"    WWWW    WWWW",
"    WWWW    WWWW",
"    WWWW    WWWW",
"WWWW    WWWW    WWWW",
"WWWW    WWWW    WWWW",
"WWWW    WWWW    WWWW",
"WWWW    WWWW    WWWW",
NULL
};

char *shape_placeholder[] =
{
"RRRR",
"OOOO",
"YYYY",
"GGGG",
"CCCC",
"BBBB",
"VVVV",
"RRRR",
"OOOO",
"YYYY",
"GGGG",
"CCCC",
"BBBB",
"VVVV",
"RRRR",
"OOOO",
"YYYY",
"GGGG",
"CCCC",
"BBBB",
"VVVV",
"RRRR",
"OOOO",
"YYYY",
"GGGG",
"CCCC",
"BBBB",
"VVVV",
NULL
};

struct shape Shape[] =
{
  [0] = { std_colors, shape_alien1r },
  [1] = { std_colors, shape_alien1u },
  [2] = { std_colors, shape_alien1l },
  [3] = { std_colors, shape_alien1d },

  [4] = { std_colors, shape_alien2r },
  [5] = { std_colors, shape_alien2u },
  [6] = { std_colors, shape_alien2l },
  [7] = { std_colors, shape_alien2d },

  [8] = { std_colors, shape_alien3r },
  [9] = { std_colors, shape_alien3u },
 [10] = { std_colors, shape_alien3l },
 [11] = { std_colors, shape_alien3d },

 [12] = { std_colors, shape_alien4r },
 [13] = { std_colors, shape_alien4u },
 [14] = { std_colors, shape_alien4l },
 [15] = { std_colors, shape_alien4d },

 [16] = { std_colors, shape_alien_radiate1 },
 [17] = { std_colors, shape_alien_radiate3 },
 [18] = { std_colors, shape_alien_radiate5 },
 [19] = { std_colors, shape_alien_radiate7 },
 [20] = { std_colors, shape_alien_radiate9 },
 [21] = { std_colors, shape_alien_radiate11 },

 [22] = { std_colors, shape_explode1 },
 [23] = { std_colors, shape_explode2 },

 [24] = { std_colors, shape_missile1 },
 [25] = { std_colors, shape_missile2 },
 [26] = { std_colors, shape_missile3 },
 [27] = { std_colors, shape_missile4 },

 [28] = { std_colors, shape_ship1r },
 [29] = { std_colors, shape_ship1u },
 [30] = { std_colors, shape_ship1l },
 [31] = { std_colors, shape_ship1d },
 [32] = { std_colors, shape_ship2u },

 [33] = { std_colors, shape_bomb },
 [34] = { std_colors, shape_placeholder },

//  [99] = { NULL, NULL }
};

