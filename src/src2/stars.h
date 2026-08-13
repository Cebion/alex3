//////////////////////////////////////////////////
//
// GAME FRAMEWORK
//
// Johan Peitz, 2001
//
//////////////////////////////////////////////////

#ifndef _STARS_H_
#define _STARS_H_

#include "allegro.h"

#define MAX_STARS	1024

typedef struct {
	double x;
	double y;
	int z;
} Tstar;

typedef struct {
	int clear_color;
	int stars;
	int width;
	int height;
	int depth;
	int col1 , col_step;
	Tstar star[MAX_STARS];
} Tstar_field;


void init_star_field(Tstar_field *sf, int w, int h, int num, int first_col, int last_col, int depth, int cc);
void draw_star_field(Tstar_field *sf, BITMAP *bmp, int x, int y);
void scroll_star_field(Tstar_field *sf, double xstep, double ystep);

#endif