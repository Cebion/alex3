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
#include "demo.h"



RLE_SPRITE *explosion[EXPLODE_FRAMES];


/* the explosion graphics are pregenerated, using a simple particle system */
void generate_explosions(void)
{
    BITMAP *bmp = create_bitmap(EXPLODE_SIZE, EXPLODE_SIZE);
    unsigned char *p;
    int c, c2;
    int x, y;
    int xx, yy;
    int color;
    
#define HOTSPOTS  64
    
    struct HOTSPOT
    {
        int x, y;
        int xc, yc;
    } hot[HOTSPOTS];
    
    for (c=0; c<HOTSPOTS; c++) {
        hot[c].x = hot[c].y = (EXPLODE_SIZE/2)<<16;
        hot[c].xc = (rand()&0xFFFF)-0x7FFF;
        hot[c].yc = (rand()&0xFFFF)-0x7FFF;
    }
    
    for (c=0; c<EXPLODE_FRAMES; c++) {
        clear_bitmap(bmp);
        
        color = ((c<16) ? c*4 : (80-c)) >> 2;
        
        for (c2=0; c2<HOTSPOTS; c2++) {
            for (x=-6; x<=6; x++) {
                for (y=-6; y<=6; y++) {
                    xx = (hot[c2].x>>16) + x;
                    yy = (hot[c2].y>>16) + y;
                    if ((xx>0) && (yy>0) && (xx<EXPLODE_SIZE) && (yy<EXPLODE_SIZE)) {
                        p = bmp->line[yy] + xx;
                        *p += (color >> ((ABS(x)+ABS(y))/3));
                        if (*p > 63)
                            *p = 63;
                    }
                }
            }
            hot[c2].x += hot[c2].xc;
            hot[c2].y += hot[c2].yc;
        }
        
        for (x=0; x<EXPLODE_SIZE; x++) {
            for (y=0; y<EXPLODE_SIZE; y++) {
                c2 = bmp->line[y][x];
                if (c2 < 8)
                    bmp->line[y][x] = 0;
                else
                    bmp->line[y][x] = 16+c2/4;
            }
        }
        
        explosion[c] = get_rle_sprite(bmp);
    }
    
    destroy_bitmap(bmp);
}


// destroys the exomplsions
void destroy_explosions() {
    int c;
    for (c=0; c<EXPLODE_FRAMES; c++) {
        destroy_rle_sprite(explosion[c]);
    }
}   

// returns the explosion array
RLE_SPRITE **get_explosions() {
    return explosion;
}
