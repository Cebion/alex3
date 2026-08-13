/******************************************************
 *     _____   .__                       ________     * 
 *    /  _  \  |  |    ____  ___  ___    \_____  \    *
 *   /  /_\  \ |  |  _/ __ \ \  \/  /      _(__  <    *
 *  /    |    \|  |__\  ___/  >    <      /       \   *
 *  \____|__  /|____/ \___  >/__/\_ \    /______  /   *
 *          \/            \/       \/           \/    *
 *													  *
 ******************************************************
 *   Alex 3, written for the Allegro Speedhack 2002   *
 *   by Johan Peitz, http://www.freelunchdesign.com   *
 ******************************************************
 
    hisc.h   										  

    All high score routines. 

 ******************************************************/


#ifndef _HISC_H_
#define _HISC_H_

#include "allegro.h"

#define MAX_SCORES      10

typedef struct {
    char name[16];
    unsigned int score;
    unsigned int floor;
} Thisc;

Thisc* make_hisc_table();
void destroy_hisc_table(Thisc *);
int qualify_hisc_table(Thisc *table, Thisc post);
void sort_hisc_table(Thisc *table);
void enter_hisc_table(Thisc *table, Thisc post);
void reset_hisc_table(Thisc *table, char *name, int hi, int lo);
int load_hisc_table(Thisc *table, PACKFILE *fp);
void save_hisc_table(Thisc *table, PACKFILE *fp);

void draw_hisc_post(Thisc *table, BITMAP *bmp, FONT *fnt, int x, int y);
void draw_hisc_table(Thisc *table, BITMAP *bmp, FONT *fnt, int x, int y);

#endif