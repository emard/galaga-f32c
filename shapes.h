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
  {'W', RGB2PIXEL(0xFFFFFF)}, // white
  {' ', RGB2PIXEL(0)}, // transparent
  {0, 0}
};

char *shape_ship1[] =
{
"    WWWW",
"    WWWW",
"    WWWW",
"    WWWW",
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

char *shape_ship2[] =
{
"    WWWW    WWWW",
"    WWWW    WWWW",
"    WWWW    WWWW",
"    WWWW    WWWW",
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
"                        WWWW",
"                        WWWW",
"                        WWWW",
"                        WWWW",
"    ",
"    ",
"    ",
"    ",
"                    WWWWWWWWWWWW",
"                    WWWWWWWWWWWW",
"                    WWWWWWWWWWWW",
"                    WWWWWWWWWWWW",
"    ",
"    ",
"    ",
"    ",
"                 WWWWWWWWWWWWWWWWWWW",
"                 WWWWWWWWWWWWWWWWWWW",
"                 WWWWWWWWWWWWWWWWWWW",
"                 WWWWWWWWWWWWWWWWWWW",
"    ",
"    ",
"    ",
"    ",
"            WWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"            WWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"            WWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"            WWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"    ",
"    ",
"    ",
"    ",
"        WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"        WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"        WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"        WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
NULL
};

char *shape_alien_radiate2[] =
{
"                    WWWWWWWWWWWW",
"                    WWWWWWWWWWWW",
"                    WWWWWWWWWWWW",
"                    WWWWWWWWWWWW",
"    ",
"    ",
"    ",
"    ",
"                 WWWWWWWWWWWWWWWWWWW",
"                 WWWWWWWWWWWWWWWWWWW",
"                 WWWWWWWWWWWWWWWWWWW",
"                 WWWWWWWWWWWWWWWWWWW",
"    ",
"    ",
"    ",
"    ",
"            WWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"            WWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"            WWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"            WWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"    ",
"    ",
"    ",
"    ",
"        WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"        WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"        WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"        WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"    ",
"    ",
"    ",
"    ",
"    WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"    WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"    WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
"    WWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWWW",
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

char *shape_missile[] =
{
"WWWW",
"WWWW",
"WWWW",
"WWWW",
"WWWW",
"WWWW",
"WWWW",
"WWWW",
NULL
};

struct shape Shape[] =
{
  [0] = { std_colors, shape_alien1u },
  [1] = { std_colors, shape_alien1d },
  [2] = { std_colors, shape_alien1l },
  [3] = { std_colors, shape_alien1r },
  [4] = { std_colors, shape_alien2u },
  [5] = { std_colors, shape_alien2d },
  [6] = { std_colors, shape_alien2l },
  [7] = { std_colors, shape_alien2r },
  [8] = { std_colors, shape_alien3u },
  [9] = { std_colors, shape_alien3d },
 [10] = { std_colors, shape_alien3l },
 [11] = { std_colors, shape_alien3r },
 [12] = { std_colors, shape_alien4u },
 [13] = { std_colors, shape_alien4d },
 [14] = { std_colors, shape_alien4l },
 [15] = { std_colors, shape_alien4r },
 [16] = { std_colors, shape_alien_radiate1 },
 [17] = { std_colors, shape_alien_radiate2 },
 [18] = { std_colors, shape_bomb },
 [19] = { std_colors, shape_missile },
 [20] = { std_colors, shape_ship1 },
 [21] = { std_colors, shape_ship2 },
//  [99] = { NULL, NULL }
};

