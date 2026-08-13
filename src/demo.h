/*         ______   ___    ___ 
 *        /\  _  \ /\_ \  /\_ \ 
 *        \ \ \L\ \\//\ \ \//\ \      __     __   _ __   ___ 
 *         \ \  __ \ \ \ \  \ \ \   /'__`\ /'_ `\/\`'__\/ __`\
 *          \ \ \/\ \ \_\ \_ \_\ \_/\  __//\ \L\ \ \ \//\ \L\ \
 *           \ \_\ \_\/\____\/\____\ \____\ \____ \ \_\\ \____/
 *            \/_/\/_/\/____/\/____/\/____/\/___L\ \/_/ \/___/
 *                                           /\____/
 *                                           \_/__/
 *
 *      A simple game demonstrating the use of the Allegro library.
 *
 *      By Shawn Hargreaves.
 *      Hacked by Johan Peitz for Speedhack 2002
 *
 */


#include <string.h>
#include <stdio.h>

#include "allegro.h"

#define EXPLODE_FLAG    10
#define EXPLODE_FRAMES  80
#define EXPLODE_SIZE    80


void generate_explosions(void);
RLE_SPRITE **get_explosions();
void destroy_explosions();
