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
 
    edit.c   										 

    Stuff for the editor.

 ******************************************************/



#include <string.h>
#include "allegro.h"
#include "main.h"
#include "map.h"
#include "edit.h"

#include "../data/data.h"


/////////////////////////
// globalez
/////////////////////////
BITMAP *buffer;
DATAFILE *data;
Tmap *map;
int current_tile = 1;
int ox = 0, oy = 0; // offset

// draws the screen
void draw_screen(BITMAP *bmp, int mx, int my) {
    clear(bmp);

    map->offset_x = ox << 6;
    map->offset_y = oy << 6;
    draw_map(bmp, map, 0, 0, 640, 480, 1);

    // show start tile/line
    rect(bmp, (-ox + map->start_x) << 6, (-oy + map->start_y) << 6, (-ox + map->start_x + 1) << 6, (-oy + map->start_y + 1) << 6, 15);
    line(bmp, ((-ox + map->start_x) << 6)+32, ((-oy + map->start_y) << 6)+32, 
        ((-ox + map->start_x) << 6) +32+ fixtoi(30*fixcos(itofix(map->start_dir * 64))), ((-oy + map->start_y) << 6) +32+ fixtoi(30*fixsin(itofix(map->start_dir * 64))), 15);

    // print useful text
    textprintf(bmp, data[FONT_SML].dat, 1, 1, 15, "Editing %s       ::   tileset: %s", map->file_name, map->tile_set);
    textprintf(bmp, data[FONT_SML].dat, 1, 9, 15, "Size: %d x %d", map->width, map->height);
    textprintf(bmp, data[FONT_SML].dat, 1, 17, 15, "Pos: %d , %d", (mx >> 6) + ox, (my >> 6) + oy);

    rect(bmp, (mx>>6)<<6, (my>>6)<<6, ((mx>>6)<<6)+63, ((my>>6)<<6)+63, 15);
    stretch_sprite(bmp, map->gfx[current_tile].dat, mx-16, my-16, 32, 32);
    rect(bmp, mx-16, my-16, mx+15, my+15, 1);

    circle(bmp, mx, my, 4, 1);
    circle(bmp, mx, my, 6, 15);
}


// editor
// starts and runs the editor
void edit(BITMAP *bmp, DATAFILE *df) {
    int quit = 0;
    int mx, my, mb;
    int tx, ty;
    int kp;
    int path_mode = 0;
    int ai_node = 0;

    buffer = bmp;
    data = df;

    map = create_map(20, 20);
    if (map == NULL) return;
    strcpy(map->tile_set, "def.dat");
    load_map_tiles(map);

    // fix gui colors
    gui_fg_color = makecol(0, 0, 0);
    gui_bg_color = makecol(255, 255, 255);


    clear_keybuf();
    while(!quit) {
        // check mouse
        mx = mouse_x;
        my = mouse_y;
        mb = mouse_b;
        tx = ox + (mx >> 6);
        ty = oy + (my >> 6);
        
        // check keys
        kp = 0;
        if (keypressed()) kp = readkey() >> 8;
        
        if (path_mode == 0) {

            if (kp == KEY_P) path_mode = 1;
            
            if (tx >= 0 && tx < map->width && ty >= 0 && ty < map->height) { 
                if (mb == 1) map->dat[tx + ty * map->width].tile = current_tile;
                if (mb == 2) current_tile = map->dat[tx + ty * map->width].tile;
            }
            
            if (kp == KEY_Z) current_tile++;
            if (kp == KEY_X) current_tile--;
            if (current_tile > map->num_tiles) current_tile = 1;
            if (current_tile == 0) current_tile = map->num_tiles;
            if (key[KEY_LCONTROL]) {
                // save map
                if (kp == KEY_S) {
                    char path[1024];
                    sprintf(path, "maps/%s", map->file_name);
                    if (file_select_ex("Save map as...", path, "MAP", 1024, 320, 400)) {
                        strcpy(map->file_name, get_filename(path));
                        save_map(map, path);
                    }
                }
                // load map
                if (kp == KEY_L) {
                    char path[1024];
                    strcpy(path, "maps/");
                    if (file_select_ex("Load map...", path, "MAP", 1024, 320, 400)) {
                        destroy_map(map);
                        map = load_map(path);
                        load_map_tiles(map);
                        strcpy(map->file_name, get_filename(path));
                    }
                }
            }
            else {
                if (key[KEY_S]) {
                    map->start_x = tx;
                    map->start_y = ty;
                    while(key[KEY_S]) {
                        if (key[KEY_UP]) map->start_dir = 3;
                        if (key[KEY_RIGHT]) map->start_dir = 0;
                        if (key[KEY_DOWN]) map->start_dir = 1;
                        if (key[KEY_LEFT]) map->start_dir = 2;
                        clear_keybuf();
                    }
                }
            }
            
            if (key[KEY_1]) map->dat[tx + ty * map->width].checkpoint = 1;
            if (key[KEY_2]) map->dat[tx + ty * map->width].checkpoint = 2;
            if (key[KEY_3]) map->dat[tx + ty * map->width].checkpoint = 3;
            if (key[KEY_4]) map->dat[tx + ty * map->width].checkpoint = 4;
            if (key[KEY_5]) map->dat[tx + ty * map->width].checkpoint = 5;
            if (key[KEY_6]) map->dat[tx + ty * map->width].checkpoint = 6;
            if (key[KEY_7]) map->dat[tx + ty * map->width].checkpoint = 7;
            if (key[KEY_8]) map->dat[tx + ty * map->width].checkpoint = 8;
            if (key[KEY_9]) map->dat[tx + ty * map->width].checkpoint = 9;
            if (key[KEY_0]) map->dat[tx + ty * map->width].checkpoint = 0;
            
            if (key[KEY_B]) {   // BUMP
                if (key[KEY_LSHIFT]) map->dat[tx + ty * map->width].type &= ~TYPE_BUMP;
                else map->dat[tx + ty * map->width].type |= TYPE_BUMP;
            }
            
            if (key[KEY_G]) {   // GOAL
                if (key[KEY_LSHIFT]) map->dat[tx + ty * map->width].type &= ~TYPE_GOAL;          
                else map->dat[tx + ty * map->width].type |= TYPE_GOAL;
            }
            
            if (key[KEY_M]) {   // MUD / SLIP
                if (key[KEY_LSHIFT]) map->dat[tx + ty * map->width].type &= ~TYPE_SLIP;          
                else map->dat[tx + ty * map->width].type |= TYPE_SLIP;
            }


            // inc/dec map?
			if (key[KEY_INSERT]) {
				if (key[KEY_UP]) change_map_size(map, 0, 1, SM_UP);
				if (key[KEY_DOWN]) change_map_size(map, 0, 1, SM_DOWN);
				if (key[KEY_LEFT]) change_map_size(map, 1, 0, SM_LEFT);
				if (key[KEY_RIGHT]) change_map_size(map, 1, 0, SM_RIGHT);
				while(key[KEY_RIGHT] || key[KEY_LEFT] || key[KEY_UP] || key[KEY_DOWN]);
			}
			if (key[KEY_DEL]) {
				if (key[KEY_UP]) change_map_size(map, 0, -1, SM_UP);
				if (key[KEY_DOWN]) change_map_size(map, 0, -1, SM_DOWN);
				if (key[KEY_LEFT]) change_map_size(map, -1, 0, SM_LEFT);
				if (key[KEY_RIGHT]) change_map_size(map, -1, 0, SM_RIGHT);
				while(key[KEY_RIGHT] || key[KEY_LEFT] || key[KEY_UP] || key[KEY_DOWN]);
			}


        }
        else {  // PATH MODE
            if (kp == KEY_P) path_mode = 0;
            
            if (kp == KEY_I && ai_node < 49) {  // insert ai node
                map->ai_path[ai_node * 2] = mx + (ox << 6);
                map->ai_path[ai_node * 2 + 1] = my + (oy << 6);
                ai_node ++;
            }

            if (kp == KEY_U && ai_node >= 0) {  // delete ai node
                map->ai_path[ai_node * 2] = -1;
                map->ai_path[ai_node * 2 + 1] = -1;
                if (ai_node > 0) ai_node --;
            }

            map->num_ai_coords = ai_node;

        }


        if (kp == KEY_UP) oy --;
        if (kp == KEY_DOWN) oy ++;
        if (kp == KEY_LEFT) ox --;
        if (kp == KEY_RIGHT) ox ++;
        
        if (key[KEY_F1]) {
            take_screenshot(buffer, "a3_e");
		    while(key[KEY_F1]);
	    }

        if (key[KEY_ESC]) quit = 1;

        // draw all
        draw_screen(buffer, mx, my);
        if (path_mode) textout(buffer, font, "PATH MODE", 1, 470, 15);
        blit_to_screen(buffer);

    }

    destroy_map(map);
}
