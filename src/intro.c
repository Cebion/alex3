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
 
    intro.c  										  

    Simply to keep the intro stuff in a separate file.
    This file also parses script files.
    
 ******************************************************/


#include <stdio.h>
#include <string.h>
#include "allegro.h"
#include "main.h"
#include "timer.h"
#include "token.h"
#include "../data/intro.h"
#include "../data/data.h"


// globalez
DATAFILE *intro_data;
DATAFILE *data;
BITMAP *buffer;
int done;
int midi = 0;


// waits for x millis or ESC
int esc_rest(int millis) {
	int clicks = millis/20;
	cycle_count=0;
	while(cycle_count<clicks && !key[KEY_ESC]);
    if (key[KEY_ESC]) {
        done = TRUE;
        return 1;
    }
	return 0;
}

// script commands
// used by the script interpretor to launch various events

void cmd_reset() {
    clear(buffer);
    blit_to_screen(buffer);
    set_palette(black_palette);
}

void cmd_showbmp(int id) {
    BITMAP *bmp = intro_data[PIC000 + id].dat;

    draw_sprite(buffer, bmp, 320 - bmp->w/2, 240 - bmp->h/2);
    blit_to_screen(buffer);
}

void cmd_fadein(int speed, int id) {
    ((RGB *)intro_data[PAL000 + id].dat)->r =
    ((RGB *)intro_data[PAL000 + id].dat)->g =
    ((RGB *)intro_data[PAL000 + id].dat)->b = 0;
    fade_in(intro_data[PAL000 + id].dat, speed);
}

void cmd_fadeout(int speed) {
    fade_out(speed);
}

void cmd_showtext(int ypos, Ttoken *t) {
    char str[2024];
    PALETTE pal;
    
    get_palette(pal);
           
    strcpy(str, "");
    // concat token into one string
    while(t->next != NULL) {
        strcat(str, t->word);
        strcat(str, " ");
        t = (Ttoken *)t->next;
    }
    strcat(str, t->word);

    // clear space for text
    rectfill(buffer, 0, ypos, 639, ypos + 40, 0);

    textout_centre(buffer, data[FONT_MAIN].dat, str, 320, ypos, -1);
    blit_to_screen(buffer);

    fade_from_range(pal, intro_data[PAL000].dat, 8, 0, 16);
}

int cmd_wait(int msec) {
    return esc_rest(msec);
}

void cmd_end() {
    done = TRUE;
}

void cmd_playsong() {
    play_midi(data[MIDI_MENU].dat, TRUE);
    midi = 1;
}
// end commands


// shows the intro :)
void intro() {
    FILE *fp;
    char buf[512];
    Ttoken *token;


    fp = fopen("scripts/intro.scr", "rt");
    if (fp == NULL) {
        allegro_message("could not open file");
        return;
    }

    done = FALSE;

    cmd_reset();

    while(!done) {
        // get commands from file
        if (fgets(buf, 512, fp) == NULL) break;
        token = tokenize(buf);
		if (token != NULL) {
			if (!stricmp(token->word, "reset")) cmd_reset();
			else if (!stricmp(token->word, "showbmp")) {
				cmd_showbmp(atoi(get_next_word(token)));
			}
			else if (!stricmp(token->word, "showtext")) {
	            cmd_showtext(atoi(get_next_word(token)), (Ttoken *)((Ttoken *)token->next)->next);
			}
			else if (!stricmp(token->word, "fadein")) {
	            Ttoken *next = (Ttoken *)token->next;
				cmd_fadein(atoi(get_next_word(token)), atoi(get_next_word(next)));
			}
			else if (!stricmp(token->word, "fadeout")) cmd_fadeout(atoi(get_next_word(token)));
			else if (!stricmp(token->word, "wait")) cmd_wait(atoi(get_next_word(token)));
			else if (!stricmp(token->word, "end")) cmd_end();
			else if (!stricmp(token->word, "playsong")) cmd_playsong();

	        flush_tokens(token);
		}
    }

    fclose(fp);
}


// launches the intro
void show_intro(BITMAP *bmp, DATAFILE *data) {
    // load data
    intro_data = load_datafile("data/intro.dat");
    if (intro_data == NULL) return;

    // fix pal
    ((RGB *)intro_data[AAAPAL].dat)[0].r = 0;
    ((RGB *)intro_data[AAAPAL].dat)[0].g = 0;
    ((RGB *)intro_data[AAAPAL].dat)[0].b = 0;

    // get buffer
    buffer = bmp;
    
    // go!
    intro();

    if (!midi) cmd_playsong();

    // done
    unload_datafile(intro_data);
}


