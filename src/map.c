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
 
    map.c   										  

    Handles the track and stuff.

 ******************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "allegro.h"
#include "map.h"


// creates one splendind map 
Tmap *create_map(int w, int h) {
	Tmap *m;
    int x, y;
	
	m = malloc(sizeof(Tmap));
	if (m == NULL) return NULL;

	strcpy(m->name, "noname");
	strcpy(m->tile_set, "none");
	strcpy(m->file_name, "unnamed.map");

	m->num_tiles = 0;
	m->gfx = NULL;

    m->width = w;
    m->height = h;
	
    m->offset_x = m->offset_y = 0;

    m->dat = malloc(sizeof(Tmappos) * m->width * m->height);
    if (m->dat == NULL) {
        free(m);
        return NULL;
    }
	
    // clear map
    for(x=0;x<w;x++)
        for(y=0;y<h;y++) {
            m->dat[x + y * w].tile = 1;
            m->dat[x + y * w].mask = 0;
            m->dat[x + y * w].type = 0;
            m->dat[x + y * w].checkpoint = 0;
        }

    // reset ai paths
    for(x=0;x<100;x++)
        m->ai_path[x] = -1;
    m->num_ai_coords = 0;

    // reset start pos
    m->start_x = 0;
    m->start_y = 0;
    m->start_dir = 0;

	return m;
}




// frees the memory of a map
void destroy_map(Tmap *m) {
	if (m == NULL) return;
	
	// free datafiles
	m->num_tiles = 0;
	if (m->gfx != NULL) {
		unload_datafile(m->gfx);
		m->gfx = NULL;
	}

    // free data
    free(m->dat);

	// free the map itself
	free(m);
}


// loads one splendind map from disk
Tmap *load_map(char *fname) {
	Tmap *m;
	FILE *fp;
    char header[6];
	
	// open file
	fp = fopen(fname, "rb");
	if (fp == NULL) {
		return NULL;
	}

	// does the header match?
	fread(header, 6, 1, fp);
	if (header[0] != 'A' && header[1] != 'X' && header[2] != '3' && header[3] != 'M' && header[4] != 'A' && header[5] != 'P') {
		return NULL;
		fclose(fp);
	}

	// get memory
	m = malloc(sizeof(Tmap));
	if (m == NULL) {
		fclose(fp);
		return NULL;
	}
	
	// read datastruct
	fread(m, sizeof(Tmap), 1, fp);

    // get memory
	m->dat = malloc(sizeof(Tmap) * m->width * m->height);
	if (m->dat == NULL) {
        free(m);
		fclose(fp);
		return NULL;
	}

	// read mappos
    fread(m->dat, sizeof(Tmappos) * m->width * m->height, 1, fp);

	// close file
	fclose(fp);

	m->gfx = NULL;

	return m;
}

// loads the tiles for a map
int load_map_tiles(Tmap *m) {
    char str[80];
    int i;

    sprintf(str, "data/%s", m->tile_set);
    if (m->gfx != NULL) unload_datafile(m->gfx);
    m->gfx = load_datafile(str);
    if (m->gfx == NULL) return 0;

	m->num_tiles = 0;
	for (i=0; m->gfx[i].type != DAT_END; i++) {
		if (!strnicmp(get_datafile_property(m->gfx+i, DAT_ID('N','A','M','E')), "TILE", 4)) m->num_tiles ++;
	}

    set_palette(m->gfx[0].dat);

    return 1;
}


// saves a map to file
int save_map(Tmap *m, char *fname) {
	FILE *fp;
	char header[6] = "AX3MAP";
	
	// open file
	fp = fopen(fname, "wb");
	if (fp == NULL) return FALSE;

	// write header
	fwrite(header, 6, 1, fp);

	// write datastruct
	fwrite(m, sizeof(Tmap), 1, fp);

	// write mappos
    fwrite(m->dat, sizeof(Tmappos) * m->width * m->height, 1, fp);

	// close file
	fclose(fp);
	
	return TRUE;
}


// draws a map to a bitmap
void draw_map(BITMAP *bmp, Tmap *m, int dx, int dy, int w, int h, int debug) {
	int x, y, i;
	int tox = m->offset_x >> 6;		// Tile Offset X
	int toy = m->offset_y >> 6;		// Tile Offset Y
	int tx, ty;
	int pos;
	int ax = (tox << 6) - m->offset_x; // pixel adjustments in x
	int ay = (toy << 6) - m->offset_y; // pixel adjustments in y
	
	set_clip(bmp, dx, dy, dx+w-1, dy+h-1);
	
	for(x=0;x<11;x++) {
		for(y=0;y<9;y++) {
			tx = x + tox;
			ty = y + toy;
			if (tx < m->width && ty < m->height && tx >= 0 && ty >= 0) {
				pos = tx + ty * m->width;
                if (m->dat[pos].tile)
					blit(m->gfx[m->dat[pos].tile].dat, bmp, 0, 0, dx + (x<<6) + ax, dy + (y<<6) + ay, 64, 64);
				    //draw_sprite(bmp, m->gfx[m->dat[pos].tile].dat, dx + (x<<6) + ax, dy + (y<<6) + ay);
                if (debug) {
                    if (m->dat[pos].checkpoint) {
                        rect(bmp, dx + (x<<6) + ax, dy + (y<<6) + ay, dx + (x<<6) + ax + 63, dy + (y<<6) + ay + 63, 10);
                        textprintf(bmp, font, dx + (x<<6) + ax, dy + (y<<6) + ay, 15, "cp: %d", m->dat[pos].checkpoint);
                    }
                    if (m->dat[pos].type & TYPE_BUMP) { // bump
                        rect(bmp, dx + (x<<6) + ax, dy + (y<<6) + ay, dx + (x<<6) + ax + 63, dy + (y<<6) + ay + 63, 2);
                        rect(bmp, dx + (x<<6) + ax+3, dy + (y<<6) + ay+3, dx + (x<<6) + ax + 63-3, dy + (y<<6) + ay + 63-3, 2);
                    }
                    if (m->dat[pos].type & TYPE_GOAL) { // goal line
                        line(bmp, dx + (x<<6) + ax, dy + (y<<6) + ay, dx + (x<<6) + ax + 63, dy + (y<<6) + ay + 63, 16);
                        line(bmp, dx + (x<<6) + ax, dy + (y<<6) + ay + 63, dx + (x<<6) + ax + 63, dy + (y<<6) + ay, 16);
                    }
                    if (m->dat[pos].type & TYPE_SLIP) { // mud
                        rect(bmp, dx + (x<<6) + ax, dy + (y<<6) + ay + 20, dx + (x<<6) + ax + 63, dy + (y<<6) + ay + 43, 10);
                        textout(bmp, font, "SLIPPY", dx + (x<<6) + ax + 10, dy + (y<<6) + ay + 30, 1);
                    }

                    // paths
                    if (m->ai_path[0] != -1) circle(bmp, m->ai_path[0] - m->offset_x, m->ai_path[1] - m->offset_y, 40, makecol(255, 0 ,0));
                    for(i=2;i<100;i+=2) {
                        if (m->ai_path[i] != -1) {
                            circle(bmp, m->ai_path[i] - m->offset_x, m->ai_path[i+1] - m->offset_y, 40, makecol(255, 0 ,0));
                            line(bmp, m->ai_path[i] - m->offset_x, m->ai_path[i+1] - m->offset_y, m->ai_path[i-2] - m->offset_x, m->ai_path[i-1] - m->offset_y, makecol(255, 0 ,0));
                            textprintf(bmp, font, m->ai_path[i] - m->offset_x, m->ai_path[i+1] - m->offset_y, 15, "%d (%d,%d)", i>>1, m->ai_path[i], m->ai_path[i+1]);
                        }
                    }
                }
			}
		}
	}

	set_clip(bmp, 0, 0, SCREEN_W - 1, SCREEN_H - 1);
	
}

/*
// draws the masks of a submap to a bitmap
void draw_sub_map_masks(BITMAP *bmp, Tmap *m, Tsubmap* sm, int dx, int dy, int w, int h, DATAFILE *mask_data) {
	int x, y;
	int tox = sm->offset_x >> 5;
	int toy = sm->offset_y >> 5;
	int tx, ty;
	int pos;
	
	rect(bmp, dx, dy, dx+w-1, dy+h-1, 251);
	
	if (sm == NULL || m == NULL) return;
	
	for(x=0;x<20;x++) {
		for(y=0;y<15;y++) {
			tx = x + tox;
			ty = y + toy;
			if (tx < sm->width && ty < sm->height && tx >= 0 && ty >= 0) {
				pos = tx + ty * sm->width;
				if (sm->dat[pos].mask) {
					draw_sprite(bmp, mask_data[sm->dat[pos].mask].dat, dx + x*32, dy + y*32);
				}
			}
		}
	}
}
*/

// copies a part of a map to another part
void copy_map(Tmap *src, Tmap *dst, int sx, int sy, int dx, int dy, int w, int h) {
	int x, y;
	for(x=0;x<w;x++)
		for(y=0;y<h;y++)
			dst->dat[dx + x + (dy + y) * dst->width] = src->dat[sx + x + (sy + y) * src->width];
}

// changes the size of the sub map according to dw and dh
// dir_flags hold on which sides to shrink and/or expand
void change_map_size(Tmap *sm, int dw, int dh, int dir_flags) {
	Tmap tsm;
	int i;

	if (sm == NULL) return;
	if (!dir_flags) return;

	if (dh) {
		// save old map and create new
		tsm.dat = sm->dat;
		tsm.width = sm->width;
		tsm.height = sm->height;
		sm->height += dh;
		sm->dat = malloc(sm->width * sm->height * sizeof(Tmappos));
		for(i=0;i<sm->width*sm->height;i++) {
			sm->dat[i].tile = 1;
			sm->dat[i].type = 0;
		}
	
		// copy relevant parts
		if (dh > 0) {
			if      (dir_flags & SM_UP  ) copy_map(&tsm, sm, 0,  0,  0, dh, tsm.width, tsm.height);
			else if (dir_flags & SM_DOWN) copy_map(&tsm, sm, 0,  0,  0,  0, tsm.width, tsm.height);
		}
		else if (dh < 0) {
			if      (dir_flags & SM_UP  ) copy_map(&tsm, sm,  0,-dh, 0, 0, tsm.width, tsm.height + dh);
			else if (dir_flags & SM_DOWN) copy_map(&tsm, sm,  0,  0, 0, 0, tsm.width, tsm.height + dh);
		}

		free(tsm.dat);
	}

	if (dw) {
		// save old map and create new
		tsm.dat = sm->dat;
		tsm.width = sm->width;
		tsm.height = sm->height;
		sm->width += dw;
		sm->dat = malloc(sm->width * sm->height * sizeof(Tmappos));
		for(i=0;i<sm->width*sm->height;i++) {
			sm->dat[i].tile = 1;
			sm->dat[i].type = 0;
		}
	
		// copy relevant parts
		if (dw > 0) {
			if      (dir_flags & SM_LEFT ) copy_map(&tsm, sm, 0,  0, dw, 0, tsm.width, tsm.height);
			else if (dir_flags & SM_RIGHT) copy_map(&tsm, sm, 0,  0,  0, 0, tsm.width, tsm.height);
		}
		else if (dw < 0) {
			if      (dir_flags & SM_LEFT ) copy_map(&tsm, sm,-dw, 0, 0, 0, tsm.width + dw, tsm.height);
			else if (dir_flags & SM_RIGHT) copy_map(&tsm, sm,  0, 0, 0, 0, tsm.width + dw, tsm.height);
		}

		free(tsm.dat);
	}

}
