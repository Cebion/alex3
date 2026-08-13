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
 
    scroller.h   										  

    Handles any scrollers.

 ******************************************************/

#ifndef _SCROLLER_H_
#define _SCROLLER_H_

#include "allegro.h"

#define MAX_LINES_IN_SCROLLER	512

typedef struct {
	int horizontal;
	char *text;
	FONT *fnt;
	int font_height;
	int width, height;
	int offset;
	int rows;
	int length;
	char *lines[MAX_LINES_IN_SCROLLER];
} Tscroller;



void init_scroller(Tscroller *sc, FONT *f, char *t, int w, int h, int horiz);
int draw_scroller(Tscroller *sc, BITMAP *bmp, int x, int y);
void scroll_scroller(Tscroller *sc, int step);
void restart_scroller(Tscroller *sc);



#endif