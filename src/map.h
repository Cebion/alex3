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
 
    map.h   										  

    Handles the track and stuff.

 ******************************************************/



#ifndef _MAP_H_
#define _MAP_H_

#include <stdio.h>
#include "allegro.h"

#define TYPE_BUMP   1
#define TYPE_GOAL   2
#define TYPE_SLIP   4

#define SM_UP		0x01
#define SM_DOWN		0x02
#define SM_RIGHT	0x04
#define SM_LEFT		0x08



typedef struct {
	unsigned char tile;     	// #id in tile set
	unsigned char mask;			// #id in mask set
	unsigned char type;			// what type(s) has this tile?
	unsigned char checkpoint;	// checkpoint id
} Tmappos;


typedef struct {
	char name[64];
	char file_name[64];
	char tile_set[127];
    unsigned char num_laps;
	DATAFILE *gfx;
	int num_tiles;

    int width, height;
	Tmappos *dat;
	int offset_x;
	int offset_y;

    int start_x, start_y;   // start pos on map
    unsigned char start_dir;          // direction
    unsigned char max_checkpoints;    // direction

    int num_ai_coords;
    int ai_path[100]; // 50 coords
} Tmap;


Tmap *create_map();
void destroy_map(Tmap *m);
Tmap *load_map(char *fname);
int load_map_tiles(Tmap *m);
int save_map(Tmap *m, char *fname);

void draw_map(BITMAP *bmp, Tmap *m, int dx, int dy, int w, int h, int debug);
void change_map_size(Tmap *sm, int dw, int dh, int dir_flags);


//void draw_map_masks(BITMAP *bmp, Tmap *m, Tsubmap* sm, int dx, int dy, int w, int h, DATAFILE *mask_data);

#endif