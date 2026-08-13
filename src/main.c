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
 
    main.h   										  

    A few assorted routines to help other modules to 
    communicate with the user and similar stuff.

    Also everything else like the actual playing. ;)

 ******************************************************/


#include <stdio.h>
#include "allegro.h"
#include "main.h"
#include "timer.h"
#include "control.h"
#include "options.h"
#include "menu.h"
#include "scroller.h"
#include "intro.h"
#include "map.h"
#include "edit.h"
#include "rally.h"
#include "vehicle.h"
#include "demo.h"
#include "Player.h"

// data
#include "../data/data.h"

#define VERSION_STR		"0.1"


//////////////////////////////////////////////////
// global variable declarations
//////////////////////////////////////////////////

int debug = FALSE;
int window = TRUE;
int got_joystick;

BITMAP *swap_screen = NULL;

Tcontrol ctrl;
Toptions options;

DATAFILE *data = NULL;


// nice bg flag
#define FLAG_W      17
#define FLAG_H      14
int flag_x[FLAG_W][FLAG_H];
int flag_y[FLAG_W][FLAG_H];


Trally race;

// players
Tplayer ply[2];

//////////////////////////////////////////////////
// menu declarations
//////////////////////////////////////////////////

Tmenu_params menu_params;

Tmenu_slider snd_volume_slider = { 0, 0, 250, 25 };
Tmenu_slider msc_volume_slider = { 0, 0, 250, 25 };



Tmenu opt_menu[] = {
	{ "Sound Vol",		0,			IDM_DECREASE,		IDM_INCREASE,		MF_SLIDER,		&snd_volume_slider },
	{ "Music Vol",		0,			IDM_DECREASE,		IDM_INCREASE,		MF_SLIDER,		&msc_volume_slider },
	{ "Fullscreen",		IDM_TOGGLE_CHECKBOX, 0, 0,			MF_CHECKBOX,					&options.full_screen},
	{ "Back",			IDM_BACK,	0,					0,					MF_LAST,		NULL }
};

Tmenu game_menu[] = {
	{ "1 Player",		IDM_NEWGAME1,	0,	0,	0,			NULL },
	{ "2 Players",	    IDM_NEWGAME2,	0,	0,	0,			NULL },
	{ "Back",			IDM_BACK,	    0,  0,  MF_LAST,	NULL }
};

Tmenu main_menu[] = {
	{ "Start Game",		IDM_SUBMENU,		0,	0,	0,			&game_menu },
	//{ "Instructions",	IDM_INSTRUCTIONS,	0,	0,	0,			NULL },
	{ "Options",		IDM_SUBMENU,		0,	0,	0,			&opt_menu },
	{ "Exit",			IDM_EXIT_TO_OS,		0,	0,	MF_LAST,	NULL }
};


//////////////////////////////////////////////////
// scroller
//////////////////////////////////////////////////

char scroller_text[] = "                Welcome to Alex the Allegator 3: Radioactive Racers!                 This is my entry to the Allegro Speedhack 2002.               The Allegro Speedhack is a contest where you are given a set of silly rules and then you must write a game that fits to the rules in less than 72 hours.                          Greets go out to: Madgarden, Spellcaster, Gripen, Lillo, Neo, Bobbe, Ajanes, Supardo, Klykan, Networm and Bob                  Anyway, enough of this ranting... Play the game and I'll see you in next Speedhack!";
Tscroller scroller;

//////////////////////////////////////////////////
// debug stuff 
//////////////////////////////////////////////////

// shows a text message
void my_alert(char *func, char *txt) {
	PALETTE p;

	get_palette(p);
	set_palette(desktop_palette);
	alert("ALERT", func, txt, "Thank you", "Amazing", 13, 27);
	set_palette(p);
}


//////////////////////////////////////////////////
// misc
//////////////////////////////////////////////////
// saves a bitmap to disc
void take_screenshot(BITMAP *bmp, char *fname) {
	PALETTE pal;
	char buf[64];
	static int n = 0;
	
	get_palette(pal);
	sprintf(buf, "%s_%02d.pcx", fname, n++);
	save_pcx(buf, bmp, pal);
}


// sets the palette correct for displaying explosions
void set_explosion_pal() {
    int i;
    PALETTE pal;

    get_palette(pal);

    for(i=0;i<16;i++) {
        pal[i+16].r = MIN(i*6, 63);
        pal[i+16].g = (i<8 ? 0 : (i-8)*8);
        pal[i+16].b = (i<12 ? 0 : (i-12)*16);
    }

    set_palette(pal);
}


//////////////////////////////////////////////////
// sound stuff
//////////////////////////////////////////////////

// plays a sound with correct volume
void play_sound(SAMPLE *s, int pitch) {
	if (s == NULL || !options.snd_volume) return;
	if (pitch) play_sample(s, options.snd_volume, 128, 1000+rand()%400-100, 0);
	else play_sample(s, options.snd_volume, 128, 1000, 0);
}



//////////////////////////////////////////////////
// init and uninit
//////////////////////////////////////////////////


// inits everything
int init_game() {
	PACKFILE *fp;

	allegro_init();

	// install various parts of allegro
	install_keyboard();
	install_timers();
	install_mouse();

	// sound
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
	
	// check for joystick
	got_joystick = (install_joystick(JOY_TYPE_AUTODETECT) ? 0 : 1);

	// get memory for buffers
	swap_screen = create_bitmap(640, 480);
	if (swap_screen == NULL) {
		allegro_message("Failed to reserve memory screen buffers.");
		return FALSE;
	}


	// load data
	data = load_datafile("data/alex3.dat");
	if (data == NULL) {
		allegro_message("Failed to load datafile.");
		return FALSE;
	}
    // fix pal error
    ((RGB *)data[0].dat)->r =
    ((RGB *)data[0].dat)->g =
    ((RGB *)data[0].dat)->b = 0;

	
	// load config
	fp = pack_fopen("alex3.cfg", "rp");
	if (fp != NULL) {
		load_options(&options, fp);
		pack_fclose(fp);
	}
    else {
        options.full_screen = 1;
        options.msc_volume = 225;
        options.snd_volume = 150;
        options.use_joy = 1;
    }
	snd_volume_slider.value = options.snd_volume;
	msc_volume_slider.value = options.msc_volume;
    set_volume(-1, options.msc_volume);
	
	set_color_depth(8);

	if (!options.full_screen) {
		if (set_gfx_mode(GFX_DIRECTX_WIN, 640, 480, 0, 0) >= 0) {
			window = 1;
			set_palette(data[AAAPAL].dat);
		}
		else options.full_screen = TRUE;
	}
	if (options.full_screen) {
		if (set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0) >= 0) {
			window = 0;
			set_palette(data[AAAPAL].dat);
		}
		else {
			allegro_message("Failed to set graphics mode.");
			return FALSE;
		}
	}

    // get the demo explosions
    generate_explosions();

	// debug?
	if (exists("debug.me")) debug = TRUE;

	// set switch mode
	set_display_switch_mode(SWITCH_PAUSE);

	// misc
	text_mode(-1);

	return TRUE;
}




// uninit everything
void uninit_game() {
	PACKFILE *fp;

	// save config and highscore
	fp = pack_fopen("alex3.cfg", "wp");
	if (fp != NULL) {
		save_options(&options, fp);
		pack_fclose(fp);
	}

	// any destroy_bitmap and unload_datafile goes here
	if (data != NULL) unload_datafile(data);
	if (swap_screen != NULL) destroy_bitmap(swap_screen);

    // loose the demo explosions
    destroy_explosions();

    // leave allegr0
	allegro_exit();
}




//////////////////////////////////////////////////
// general drawing
//////////////////////////////////////////////////

// blit a bmp to screen
void blit_to_screen(BITMAP *bmp) {
	acquire_screen();
	blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);
	release_screen();
}




////////////////////////////////////
// drawing
///////////////////////////////////

// prints text with black outline
void text_outline(BITMAP *bmp, char *str, int x, int y) {
    textout(bmp, data[FONT_MAIN].dat, str, x-1, y, 1);
    textout(bmp, data[FONT_MAIN].dat, str, x+1, y, 1);
    textout(bmp, data[FONT_MAIN].dat, str, x, y+1, 1);
    textout(bmp, data[FONT_MAIN].dat, str, x, y+1, 1);
    textout(bmp, data[FONT_MAIN].dat, str, x+1, y+2, 1);
    textout(bmp, data[FONT_MAIN].dat, str, x, y, -1);
}


// draws a frame of the game
void draw_frame(BITMAP *bmp, int plys) {
    int i, c;
    int ox;
    int oy;
    char buf[80];
    
	frame_count ++;

    for(c=0;c<plys;c++) {
        ox = MAX(0, MIN(fixtoi(race.cars[c].pos.x) - (plys == 1 ? 320 : 160), (race.track->width << 6) - (plys == 1 ? 640 : 320)));
        oy = MAX(0, MIN(fixtoi(race.cars[c].pos.y) - 240, (race.track->height << 6) - 480));

        race.track->offset_x = ox;
        race.track->offset_y = oy;
        draw_map(swap_screen, race.track, (c ? 320 : 0), 0, (plys == 1 ? 640 : 320), 480, 0);
        
        set_clip(bmp, (c ? 320 : 0), 0, (plys == 1 ? 639 : 319 + c * 320), 480);
        for(i=0;i<MAX_CARS;i++) {
            draw_vehicle(bmp, &race.cars[i], ox - (c ? 320 : 0), oy);
        }

        if (ply[c].car->in_goal) {
            sprintf(buf, "You placed %d!", ply[c].car->place);
            text_outline(bmp, buf, (c ? 360 : 40), 440);
        }
    }
	set_clip(bmp, 0, 0, SCREEN_W - 1, SCREEN_H - 1);

    sprintf(buf, "Laps: %d", ply[0].car->laps);
    text_outline(bmp, buf, 5, 2);
    if (plys == 2) {
        stretch_sprite(bmp, data[CENTERBAR].dat, 314, 0, 12, 480);
        sprintf(buf, "Laps: %d", ply[1].car->laps);
        text_outline(bmp, buf, 550, 2);
    }


	// some output
	if (debug) textprintf(bmp, font, 0, 0, 15, "%6d, %6d", fps, lps);
}



//////////////////////////////////////////////////
// playing
//////////////////////////////////////////////////

// handles input from player
void handle_player_input(Tplayer *p) {
    if (!p->car->in_goal) poll_control(&p->ctrl);
    else clear_control(&p->ctrl);

    if (is_fire(&p->ctrl)) p->car->throttle = MIN(p->car->throttle + ftofix(0.01), ftofix(0.5));
    else p->car->throttle *= 0.5;
    
    if (is_down(&p->ctrl)) {
        p->car->brake = MIN(p->car->brake + ftofix(0.01), ftofix(0.5));
        p->car->throttle = 0;
    }
    else p->car->brake *= 0.3;
    
    if  (is_right(&p->ctrl)) {
        if (p->car->steer_angle > -itofix(32))
            p->car->steer_angle -= itofix(2);
    }
    else if (is_left(&p->ctrl)) {
        if (p->car->steer_angle < itofix(32))
            p->car->steer_angle += itofix(2);
    } 
    else {
        p->car->steer_angle *= 0.8;
    }
   
}

// handle AI
void handle_AI_input(Tvehicle *v) {
    if (v->in_goal) {
        v->throttle = itofix(1);
    }

    v->throttle = MIN(v->throttle + ftofix(0.03), ftofix(2.0)); 

    v->target_angle = fatan2(itofix(race.track->ai_path[v->next_ai_point * 2 + 1]) - v->pos.y, itofix(race.track->ai_path[v->next_ai_point * 2]) - v->pos.x);

    if (((v->angle - v->target_angle) & 0xFFFFFF) > itofix(16)) {
        if (((v->angle - v->target_angle) & 0xFFFFFF) < itofix(128)) {
            if (v->steer_angle < itofix(32))
                v->steer_angle += itofix(2);
        }
        else {
            if (v->steer_angle > -itofix(32))
                v->steer_angle -= itofix(2);
        }
    }
    else {
        v->steer_angle *= 0.2;
    }

    if (fhypot(v->pos.y - itofix(race.track->ai_path[v->next_ai_point * 2 + 1]), v->pos.x - itofix(race.track->ai_path[v->next_ai_point * 2])) < itofix(128)) {
        v->next_ai_point ++;
        if (v->next_ai_point == race.track->num_ai_coords) v->next_ai_point = 0;
    }



}


// initiates a new game
void new_race(int plys) {
    int i;
    fixed x, y;
    
    // init race
    race.track = load_map("maps/test.map");
    race.track->gfx = load_datafile("data/def.dat");
    load_map_tiles(race.track);
    race.track->num_laps = 5;

    race.track->max_checkpoints = 0;
    for(i=0;i<race.track->width * race.track->height;i++)
        if (race.track->dat[i].checkpoint > race.track->max_checkpoints)
            race.track->max_checkpoints = race.track->dat[i].checkpoint;

    // init cars
    x = itofix((race.track->start_x << 6) + 32);
    y = itofix((race.track->start_y << 6) + 32);

    for(i=0;i<MAX_CARS;i++) {
        init_vehicle(&race.cars[i], data[CAR000 + (i==0 ? 0 : (i>1 ? 2 : (plys == 2 ? 1 : 2)))].dat);
        race.cars[i].front_tire = data[TIRE_SML].dat;
        race.cars[i].rear_tire = data[TIRE_BIG].dat;
        race.cars[i].laps = 0;
        race.cars[i].next_checkpoint = 1;
        race.cars[i].next_ai_point = 0;
        // get positions from map
        // place car
        race.cars[i].pos.x = x;
        race.cars[i].pos.y = y;
        race.cars[i].angle = itofix(race.track->start_dir * 64);
        if (race.track->start_dir == 3) {
            if (i&1) {
                x -= itofix(128);
                y += itofix(128);
            }
            else x += itofix(128);
        }
        if (race.track->start_dir == 0) {
            if (i&1) {
                y -= itofix(128);
                x -= itofix(128);
            }
            else y += itofix(128);
        }
        if (race.track->start_dir == 1) {
            if (i&1) {
                x -= itofix(128);
                y -= itofix(128);
            }
            else x += itofix(128);
        }
        if (race.track->start_dir == 2) {
            if (i&1) {
                y += itofix(128);
                x += itofix(128);
            }
            else y -= itofix(128);
        }
    }

    // init player
    for(i=0;i<plys;i++) {
        init_player(&ply[i], i);
        ply[i].car = &race.cars[i];
        ply[i].car->laps = 0;
        ply[i].car->ai_on = 0;
    }
    set_control(&ply[0].ctrl, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_RCONTROL);
    set_control(&ply[1].ctrl, KEY_W, KEY_S, KEY_A, KEY_D, KEY_LCONTROL);
    ply[0].ctrl.use_joy = TRUE;
    ply[1].ctrl.use_joy = FALSE;
}

// draw lights
void draw_lights(BITMAP *bmp, int step, int ypos) {
    int i;

    for(i=step;i<4;i++)
        draw_sprite(bmp, data[LIGHT_1].dat, 224 + i*49, ypos);

    if (step > 0)
        draw_sprite(bmp, data[LIGHT_2].dat, 224 + 0*49, ypos);
    if (step > 1)
        draw_sprite(bmp, data[LIGHT_2].dat, 224 + 1*49, ypos);
    if (step > 2)
        draw_sprite(bmp, data[LIGHT_3].dat, 224 + 2*49, ypos);
    if (step > 3)
        draw_sprite(bmp, data[LIGHT_4].dat, 224 + 3*49, ypos);
}

// plays the game
int play_race(int plys) {
	int playing = TRUE;
    int i,j;
    int light_y = -50;
    int light_step = 0;
    int light_count = 1;
    int go = FALSE;
    int next_place = 1;
    
    set_palette(black_palette);

    play_midi(data[MIDI_GAME].dat, 1);

    // show laps and other start stuff
    draw_frame(swap_screen, plys);
	blit_to_screen(swap_screen);

    fade_in(race.track->gfx[0].dat, 8);

    cycle_count = 0;
	while(playing) {
		while(cycle_count > 0) {  // handle logic
			logic_count ++;

            if (go) {
                // get input
                for(i=0;i<MAX_CARS;i++) {
                    if (i == 0) handle_player_input(&ply[i]);
                    else if (i == 1 && plys == 2) handle_player_input(&ply[i]);
                    else handle_AI_input(&race.cars[i]);
                }

                // raise lights
                if (light_y > - 50) light_y --;
            }
            else {
                if (light_y < 6) light_y ++;
                else light_count++;
                if (light_count % 25 == 0) {
                    light_step ++;
                    if (light_step == 4) go = TRUE;
                }

            }
            
            // handle physics
            for(i=0;i<MAX_CARS;i++) update_vehicle(&race.cars[i]);
            for(i=0;i<MAX_CARS-1;i++) 
                for(j=i+1;j<MAX_CARS;j++) 
                    check_collision(&race.cars[j], &race.cars[i]);

            // check vehicles against map
            for(i=0;i<MAX_CARS;i++) {
                if (check_vehicle_against_map(&race.cars[i], race.track)) {
                    if (!race.cars[i].place) {
                        race.cars[i].place = next_place;
                        next_place ++;
                    }
                }
            }

            cycle_count --;
		}
		
		// draw 
		frame_count ++;
		draw_frame(swap_screen, plys);
        if (light_y != -50) draw_lights(swap_screen, light_step, light_y);
		blit_to_screen(swap_screen);

		// handle special keys
		if (key[KEY_ESC]) playing = FALSE;
    	if (key[KEY_F1]) {
	    	take_screenshot(swap_screen, "a3_g");
		    while(key[KEY_F1]);
	    }

        // are players done?
        if (plys == 1 && ply[0].car->in_goal) playing = FALSE;
        if (plys == 2 && ply[0].car->in_goal && ply[1].car->in_goal) playing = FALSE;

	}

    while(!key[KEY_ESC]) {
    	draw_frame(swap_screen, plys);
        blit_to_screen(swap_screen);
    }

    destroy_map(race.track);

    stop_midi();

	return 0;
}


//////////////////////////////////////////////////
// start game
//////////////////////////////////////////////////

// a new game!
void new_game(int plys) {
    clear(screen);
    new_race(plys);
    play_race(plys);
}



//////////////////////////////////////////////////
// credits
//////////////////////////////////////////////////

// shows the credits
void show_credits() {
    int time = 400;
    int x[100];
    int y[100];
    int c[100];
    int i;
    RLE_SPRITE **rle = get_explosions();

    fade_out(4);
    stop_midi();

    // init explosions
    for(i=0;i<100;i++) c[i] = -1;

    clear(screen);

    draw_sprite(screen, data[PLANT].dat, 320 - ((BITMAP *)data[PLANT].dat)->w/2, 50);
    draw_sprite(screen, data[LOGO].dat, 320 - ((BITMAP *)data[LOGO].dat)->w/2, 110);
    textout_centre(screen, data[FONT_MAIN].dat, "CODE AND GFX: Johan Peitz", 320, 300, -15);
    textout_centre(screen, data[FONT_MAIN].dat, "MUSIC: Bob Bonnell", 320, 340, -15);

    fade_in(data[AAAPAL].dat, 4);
    set_explosion_pal();

    cycle_count = 0;
    while(time > 0 && !key[KEY_ESC]) {
        while(cycle_count > 0) {  // handle logic
            cycle_count --;

            for(i=0;i<100;i++) {
                if (c[i] >= 0) {
                    if (++ c[i] == EXPLODE_FRAMES) c[i] = -1;
                }
                else if (rand()%100 < 2) {
                    x[i] = rand()%640;
                    y[i] = rand()%480;
                    c[i] = 0;
                }
            }


            time --;
        }
        // draw
        clear(swap_screen);
        // explosions
        for(i=0;i<100;i++) 
            if (c[i] >= 0)
                draw_rle_sprite(swap_screen, rle[c[i]], x[i] - EXPLODE_SIZE/2, y[i] - EXPLODE_SIZE/2);

        // stuff
        draw_sprite(swap_screen, data[PLANT].dat, 320 - ((BITMAP *)data[PLANT].dat)->w/2, 50);
        draw_sprite(swap_screen, data[LOGO].dat, 320 - ((BITMAP *)data[LOGO].dat)->w/2, 110);
        textout_centre(swap_screen, data[FONT_MAIN].dat, "CODE AND GFX: Johan Peitz", 320, 300, -15);
        textout_centre(swap_screen, data[FONT_MAIN].dat, "MUSIC: Bob Bonnell", 320, 340, -15);
        blit_to_screen(swap_screen);
    }

    fade_out(4);

}



//////////////////////////////////////////////////
// instructions
//////////////////////////////////////////////////

// shows the instructions
void show_instructions() {
}



//////////////////////////////////////////////////
// menu callbacks
//////////////////////////////////////////////////

// plays the correct sound when the menu cursor is moved
void play_menu_move() {
	play_sound(data[SND_MENU_TICK].dat, 0);
}


// plays the correct sound when a selectio nis made in the menu
void play_menu_select() {
	play_sound(data[SND_MENU_TICK].dat, 0);
}


// draws the rest of the menu
void main_menu_callback(void) {
    static int count = 0;
    int x, y;
    int poly[8];
    int white = 254;
    int black = 253;
    int old_v;

    // calc & draw flag
    count ++;
    for(x=0;x<FLAG_W;x++)
        for(y=0;y<FLAG_H;y++) {
            flag_y[x][y] = fixtoi(20 * (fcos(itofix(count*2 + y*3 + x)) + fsin(itofix(count + y*16 + x*25))));
            flag_x[x][y] = fixtoi(20 * fcos(itofix(count + y*13 + x)));
        }

    for(x=0;x<FLAG_W-1;x++)
        for(y=0;y<FLAG_H-1;y++) {
			poly[0] = x * 45 + flag_x[x][y] - 40;
			poly[1] = y * 45 + flag_y[x][y] - 40;
			poly[2] = (x+1) * 45 + flag_x[x+1][y] - 40;
			poly[3] = y * 45 + flag_y[x+1][y] - 40;
			poly[4] = (x+1) * 45 + flag_x[x+1][y+1] - 40;
			poly[5] = (y+1) * 45 + flag_y[x+1][y+1] - 40;
			poly[6] = x * 45 + flag_x[x][y+1] - 40;
			poly[7] = (y+1) * 45 + flag_y[x][y+1] - 40;

            polygon(swap_screen, 4, poly, (x&2 ? (y&2 ? white : black) : (y&2 ? black : white)));

        }


    // draw logo
    draw_sprite(swap_screen, data[PLANT].dat, 320 - ((BITMAP *)data[PLANT].dat)->w/2, 50);
    draw_sprite(swap_screen, data[LOGO].dat, 320 - ((BITMAP *)data[LOGO].dat)->w/2, 110);
//    stretch_sprite(swap_screen, data[LOGO].dat, 320 - w/2, 160 - h/2, w, h);

    textout_centre(swap_screen, data[FONT_MAIN].dat, "ALEX THE ALLEGATOR 3:", 321, 10, 1);
    textout_centre(swap_screen, data[FONT_MAIN].dat, "ALEX THE ALLEGATOR 3:", 319, 10, 1);
    textout_centre(swap_screen, data[FONT_MAIN].dat, "ALEX THE ALLEGATOR 3:", 320, 11, 1);
    textout_centre(swap_screen, data[FONT_MAIN].dat, "ALEX THE ALLEGATOR 3:", 320, 9, 1);
    textout_centre(swap_screen, data[FONT_MAIN].dat, "ALEX THE ALLEGATOR 3:", 320, 10, -1);

    // draw sun
    circlefill(swap_screen, 160 + fixtoi(100 * fcos(ftofix(count/4.0))), 480 + fixtoi(80 * fsin(ftofix(count/4.0))), 12, 244);

    // draw landscape
    draw_sprite(swap_screen, data[MENU_BG].dat, -((count>>2)%640), 428);
    draw_sprite(swap_screen, data[MENU_BG].dat, 640-((count>>2)%640), 428);
    draw_sprite(swap_screen, data[MENU_FG].dat, -((count>>1)%640), 419);
    draw_sprite(swap_screen, data[MENU_FG].dat, 640-((count>>1)%640), 419);

    // draw scroller
	scroll_scroller(&scroller, -1);
	if (!draw_scroller(&scroller, swap_screen,  0, 470)) restart_scroller(&scroller);

    // draw info        
	textprintf_right(swap_screen, data[FONT_SML].dat, 638, 3, 1,   "v%s%s", VERSION_STR, (debug ? " DEBUG ON" : ""));
	textprintf_right(swap_screen, data[FONT_SML].dat, 637, 2, 15, "v%s%s", VERSION_STR, (debug ? " DEBUG ON" : ""));

	if (window && options.full_screen) {
		set_gfx_mode(GFX_AUTODETECT, 640, 480, 0, 0);
		window = 0;
		set_palette(data[AAAPAL].dat);
	}
	if (!window && !options.full_screen) {
		set_gfx_mode(GFX_DIRECTX_WIN, 640, 480, 0, 0);
		window = 1;
		set_palette(data[AAAPAL].dat);
	}

	// update sliders
    old_v = options.msc_volume;
	options.snd_volume = get_slider_value(&snd_volume_slider);
	options.msc_volume = get_slider_value(&msc_volume_slider);
    if (old_v != options.msc_volume) {
        set_volume(-1, options.msc_volume);
    }


    if (key[KEY_E] && key[KEY_TAB]) {
        edit(swap_screen, data);
        set_palette(data[AAAPAL].dat);
    }

}



//////////////////////////////////////////////////
// main
//////////////////////////////////////////////////

// main 
// inits the game, sets some initial parameters 
// and starts the menu
int main() {
	int ret = 0;
    int fade = 1;

	// init allegro & the game
	if (!init_game()) {
		allegro_message("Failed to initialize the game.");
		allegro_exit();
		return 1;
	}

	set_palette(black_palette);

    show_intro(swap_screen, data);

	set_palette(black_palette);

	menu_params.font = data[FONT_MAIN].dat;
	menu_params.bullet = NULL;  // bmp
	menu_params.pos = 0;
	init_control(&menu_params.ctrl);
	init_control(&ctrl);
	if (got_joystick) ctrl.use_joy = TRUE;
	reset_menu(main_menu, &menu_params, 0);

    // reset scroller
    init_scroller(&scroller, data[FONT_SML].dat, scroller_text, 640, 10, TRUE);
    restart_scroller(&scroller);

	main_menu_callback();
	draw_menu(swap_screen, main_menu, &menu_params, 50, 270);
    blit_to_screen(swap_screen);

	while (ret != IDM_EXIT_TO_OS) {
		main_menu_callback();
		draw_menu(swap_screen, main_menu, &menu_params, 50, 270);
		blit_to_screen(swap_screen);
    
        if (fade) {
            fade_in(data[AAAPAL].dat, 8);
            fade = 0;
        }


		// let user play with menu
		ret = handle_menu(main_menu, &menu_params, &ctrl, swap_screen, main_menu_callback, 50, 270);
		
		// check if somthing should be launched
		if (ret == IDM_NEWGAME1 || ret == IDM_NEWGAME2) {
            fade_out(8);
            stop_midi();
            new_game((ret == IDM_NEWGAME1 ? 1 : 2));
			menu_params.pos = 0;
            play_midi(data[MIDI_MENU].dat, TRUE);
            set_palette(black_palette);
            fade = 1;
		}
		if (ret == IDM_INSTRUCTIONS) {
			show_instructions();
			menu_params.pos = 1;
		}
	}

	show_credits();
	
	// uninit and exit
	uninit_game();
	return 0;

} END_OF_MAIN();

