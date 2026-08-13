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
 
    menu.h   										  

    Handles all menu like operations. Defines the menu
    structs and similar data types.

 ******************************************************/



#include <stdio.h>
#include <string.h>

#include "allegro.h"
#include "main.h"
#include "menu.h"
#include "control.h"
#include "timer.h"


// builds the menu string into *dest
void build_menu_string(Tmenu *m, char *dest) {
	int v,t,i;
	if (m->flags & MF_SLIDER) {
		//sprintf(dest, "%s: %d/%d", m->caption, ((Tmenu_slider *)m->data)->value, ((Tmenu_slider *)m->data)->max);
		v = (((Tmenu_slider *)m->data)->value - ((Tmenu_slider *)m->data)->min)/((Tmenu_slider *)m->data)->step;
		t = (((Tmenu_slider *)m->data)->max - ((Tmenu_slider *)m->data)->min)/((Tmenu_slider *)m->data)->step;
		sprintf(dest, "%s: ", m->caption);
		for(i=0;i<v;i++) strcat(dest, "!");
		for(;i<t;i++) strcat(dest, ".");
		
		return;
	}
	if (m->flags & MF_SELECTION) {
		sprintf(dest, "%s: %s", m->caption, ((Tmenu_selection *)m->data)->caption[((Tmenu_selection *)m->data)->value]);
		return;
	}
	if (m->flags & MF_CHECKBOX) {
		sprintf(dest, "%s: %s", m->caption, (*(int *)m->data ? "YES" : "NO"));
		return;
	}
	if (m->flags & MF_KEY) { 
		char str[32];
		key_to_str(*(int *)m->data, str);
		sprintf(dest, "%s: (%s)", m->caption, str);
		return;
	}

	sprintf(dest, "%s", m->caption);
}


// draws one menu post
void draw_menu_item(BITMAP *bmp, char *str, Tmenu_params *mp, int x, int y) {
    textout_centre(bmp, mp->font, str, x-1, y, 1);
	textout_centre(bmp, mp->font, str, x+1, y, 1);
	textout_centre(bmp, mp->font, str, x, y-1, 1);
	textout_centre(bmp, mp->font, str, x, y+1, 1);
	textout_centre(bmp, mp->font, str, x+1, y+2, 1);
	textout_centre(bmp, mp->font, str, x, y, -1);
}


// draws a menu on specific bitmap at specified coordinates
// mp - parameters
void draw_menu(BITMAP *bmp, Tmenu *m, Tmenu_params *mp, int x, int y) {
	int pos = -1;
	char str[256];
	int h = mp->font_height;
    static count = 0;

	do {
		pos++;

		build_menu_string(&m[pos], str);

		if (m[pos].flags & MF_SELECTED) {
//			if (mp->bullet != NULL) draw_sprite(bmp, mp->bullet, x - mp->bullet->w, y + pos * h - 8);
//			else circle(bmp, x-10, y + pos * h+15, 2, 15);
            draw_menu_item(bmp, str, mp, 320, y + pos * h + fixtoi(5 * fcos(itofix((count++)*13))));
		}
		else {
            draw_menu_item(bmp, str, mp, 320, y + pos * h);
		}

	} while(!(m[pos].flags & MF_LAST));
}


// reset a menu
// sel_pos is the the menu pos to be selected
void reset_menu(Tmenu *m, Tmenu_params *mp, int sel_pos) {
	int i=-1;

	// unselect all
	do {
		m[++i].flags &= ~MF_SELECTED;
	} while(!(m[i].flags & MF_LAST));

	// set first one to selected
	m[sel_pos].flags |= MF_SELECTED;

	mp->font_height = text_height(mp->font);
}


// handle a menu, returns selected slot
// bmp - where to draw the menu
// m - menu to draw/check
// mp - parameters
// x,y - where to draw the menu
// ctrl - control method
// return - action selected
int update_menu(BITMAP *bmp, Tmenu *m, Tmenu_params *mp, Tcontrol *ctrl, int x, int y, void **data) {
	int num_posts, old_pos, pos=0;
	int return_value = 0;

	// find current pos and number of posts
	num_posts = -1; 
	do {
		if (m[++num_posts].flags & MF_SELECTED) pos = num_posts;
	} while(!(m[num_posts].flags & MF_LAST));

	// draw the menu
	draw_menu(bmp, m, mp, x, y);
	
	if (ctrl != NULL) {
		// check controls for action
		old_pos = pos;
		if ((is_up(ctrl) || is_up(&mp->ctrl)) && pos) pos--; 
		if ((is_down(ctrl) || is_down(&mp->ctrl)) && pos<num_posts) pos++; 
		if (pos != old_pos) {
			m[old_pos].flags &= ~MF_SELECTED;
			m[pos].flags |= MF_SELECTED;
			play_menu_move();
		}
		if (is_fire(ctrl) || is_enter(&mp->ctrl)) {
			return_value = m[pos].return_select;
		}
		if (is_left(ctrl) || is_left(&mp->ctrl)) return_value = m[pos].return_left;
		if (is_right(ctrl) || is_right(&mp->ctrl)) return_value = m[pos].return_right;
	}
		

	if (key[KEY_F1]) {
		take_screenshot(bmp, "a3_m");
		while(key[KEY_F1]);
	}


	*data = (void *)m[pos].data;
	return return_value;
}
	


// handles a Tmenu
// menu - menu to use
// mp - menu parameters
// ctrl - keys to use (control method)
// bmp - bitmap where to draw the menu
// callback - called every frame BEFORE the menu is drawn
// x,y - where to draw the menu
// return - menu action
int handle_menu(Tmenu *menu,  Tmenu_params *mp, Tcontrol *ctrl, BITMAP *bmp, void (*callback)(void), int x, int y) {
	int menu_return = 0;
	int hold_counter = 10;
	int handle_keys = FALSE;
	int done = FALSE;
	void *data = NULL;

	reset_menu(menu, mp, mp->pos);

	while (!done) {
        cycle_count = 0;
		if (callback != NULL) callback();
		else clear(bmp);

		menu_return = update_menu(bmp, menu, mp, (handle_keys ? ctrl : NULL), x, y, &data);
		blit_to_screen(bmp);
		
		if (!is_any(ctrl) && !is_any(&mp->ctrl)) handle_keys = TRUE;
		else handle_keys = FALSE;
		poll_control(ctrl);
		poll_control(&mp->ctrl);

		if (menu_return) play_menu_select();

        while(cycle_count == 0);

		switch(menu_return) {
		    case (0) : break; // nothing has happened

			case (IDM_NEWGAME1) : {
				return menu_return;
				break;
			}
			case (IDM_NEWGAME2) : {
				return menu_return;
				break;
			}
			case (IDM_INSTRUCTIONS) : {
				return menu_return;
				break;
			}
			case (IDM_HIGHSCORES) : {
				return menu_return;
				break;
			}
			case (IDM_SUBMENU) : {
				mp->pos = 0;
				return handle_menu(data, mp, ctrl, bmp, callback, x, y);
				break;
			}
			case (IDM_EXIT_TO_OS) : { 
				return IDM_EXIT_TO_OS;
				break;
			}
			case (IDM_BACK) : {
				return IDM_BACK;
				break;
			}

			case (IDM_INCREASE) : {
				Tmenu_slider *sld = (Tmenu_slider *)data;
				sld->value = MIN(sld->value + sld->step, sld->max);
				break;
			}
			case (IDM_DECREASE) : {
				Tmenu_slider *sld = (Tmenu_slider *)data;
				sld->value = MAX(sld->value - sld->step, sld->min);
				break;
			}
			case (IDM_NEXT) : {
				Tmenu_selection *sel = (Tmenu_selection *)data;
				sel->value = MIN(sel->value + 1, sel->size - 1);
				break;
			}
			case (IDM_PREV) : {
				Tmenu_selection *sel = (Tmenu_selection *)data;
				sel->value = MAX(sel->value - 1, 0);
				break;
			}
			case (IDM_TOGGLE_CHECKBOX) : {
				int *box = (int *)data;
				if (*box) *box = FALSE;
				else *box = TRUE;
				break;
			}
			case (IDM_GET_KEY) : {
				int kp = 0, k;

				for(k=0;k<128;k++) key[k] = 0;
				while(!kp) {
					for(k=0;k<128;k++) if (key[k]) kp=k;
					if (check_control_key(ctrl, kp)) kp = 0;
				}
				for(k=0;k<128;k++) key[k] = 0;
				*((int *)data) = kp;
				break;
			}

			default : {   // unknown return value
				char buf[128];
				sprintf(buf,"unknown return value: %d", menu_return);
				my_alert("handle_menu", buf);
				break;
			}
		}
	}
	return menu_return;
}


// puts the name of the allegro key k into dest
void key_to_str(int k, char *dest) {
    if      (k == KEY_A) strcpy(dest, "A");
    else if (k == KEY_B) strcpy(dest, "B");
    else if (k == KEY_C) strcpy(dest, "C");              
    else if (k == KEY_D) strcpy(dest, "D");
    else if (k == KEY_E) strcpy(dest, "E");
    else if (k == KEY_F) strcpy(dest, "F");
    else if (k == KEY_G) strcpy(dest, "G");
    else if (k == KEY_H) strcpy(dest, "H");
    else if (k == KEY_I) strcpy(dest, "I");
    else if (k == KEY_J) strcpy(dest, "J");
    else if (k == KEY_K) strcpy(dest, "K");
    else if (k == KEY_L) strcpy(dest, "L");
    else if (k == KEY_M) strcpy(dest, "M");
    else if (k == KEY_N) strcpy(dest, "N");
    else if (k == KEY_O) strcpy(dest, "O");
    else if (k == KEY_P) strcpy(dest, "P");
    else if (k == KEY_Q) strcpy(dest, "Q");
    else if (k == KEY_R) strcpy(dest, "R");
    else if (k == KEY_S) strcpy(dest, "S");
    else if (k == KEY_T) strcpy(dest, "T");
    else if (k == KEY_U) strcpy(dest, "U");
    else if (k == KEY_V) strcpy(dest, "V");
    else if (k == KEY_W) strcpy(dest, "W");
    else if (k == KEY_X) strcpy(dest, "X");
    else if (k == KEY_Y) strcpy(dest, "Y");
    else if (k == KEY_Z) strcpy(dest, "Z");
    else if (k == KEY_0) strcpy(dest, "0");
    else if (k == KEY_1) strcpy(dest, "1");
    else if (k == KEY_2) strcpy(dest, "2");
    else if (k == KEY_3) strcpy(dest, "3");
    else if (k == KEY_4) strcpy(dest, "4");
    else if (k == KEY_5) strcpy(dest, "5");
    else if (k == KEY_6) strcpy(dest, "6");
    else if (k == KEY_7) strcpy(dest, "7");
    else if (k == KEY_8) strcpy(dest, "8");
    else if (k == KEY_9) strcpy(dest, "9");
    else if (k == KEY_0_PAD) strcpy(dest, "0 (Pad)");
    else if (k == KEY_1_PAD) strcpy(dest, "1 (Pad)");
    else if (k == KEY_2_PAD) strcpy(dest, "2 (Pad)");
    else if (k == KEY_3_PAD) strcpy(dest, "3 (Pad)");
    else if (k == KEY_4_PAD) strcpy(dest, "4 (Pad)");
    else if (k == KEY_5_PAD) strcpy(dest, "5 (Pad)");
    else if (k == KEY_6_PAD) strcpy(dest, "6 (Pad)");
    else if (k == KEY_7_PAD) strcpy(dest, "7 (Pad)");
    else if (k == KEY_8_PAD) strcpy(dest, "8 (Pad)");
    else if (k == KEY_9_PAD) strcpy(dest, "9 (Pad)");
    else if (k == KEY_F1) strcpy(dest, "F1");
    else if (k == KEY_F2) strcpy(dest, "F2");
    else if (k == KEY_F3) strcpy(dest, "F3");
    else if (k == KEY_F4) strcpy(dest, "F4");
    else if (k == KEY_F5) strcpy(dest, "F5");
    else if (k == KEY_F6) strcpy(dest, "F6");
    else if (k == KEY_F7) strcpy(dest, "F7");
    else if (k == KEY_F8) strcpy(dest, "F8");
    else if (k == KEY_F9) strcpy(dest, "F9");
    else if (k == KEY_F10) strcpy(dest, "F10");
    else if (k == KEY_F11) strcpy(dest, "F11");
    else if (k == KEY_F12) strcpy(dest, "F12");
    else if (k == KEY_ESC) strcpy(dest, "ESC");
    else if (k == KEY_TILDE) strcpy(dest, "~");
    else if (k == KEY_MINUS) strcpy(dest, "-");
    else if (k == KEY_EQUALS) strcpy(dest, "=");
    else if (k == KEY_BACKSPACE) strcpy(dest, "Backspace");
    else if (k == KEY_TAB) strcpy(dest, "Tab");
    else if (k == KEY_OPENBRACE) strcpy(dest, "{");
    else if (k == KEY_CLOSEBRACE) strcpy(dest, "}");
    else if (k == KEY_ENTER) strcpy(dest, "Enter");
    else if (k == KEY_COLON) strcpy(dest, ":");
    else if (k == KEY_QUOTE) strcpy(dest, "'");
    else if (k == KEY_BACKSLASH) strcpy(dest, "\\");
    else if (k == KEY_BACKSLASH2) strcpy(dest, "\\");
    else if (k == KEY_COMMA) strcpy(dest, ",");
    else if (k == KEY_STOP) strcpy(dest, ".");
    else if (k == KEY_SLASH) strcpy(dest, "/");
    else if (k == KEY_SPACE) strcpy(dest, "Space");
    else if (k == KEY_INSERT) strcpy(dest, "Insert");
    else if (k == KEY_DEL) strcpy(dest, "Delete");
    else if (k == KEY_HOME) strcpy(dest, "Home");
    else if (k == KEY_END) strcpy(dest, "End");
    else if (k == KEY_PGUP) strcpy(dest, "Page Up");
    else if (k == KEY_PGDN) strcpy(dest, "Page Down");
    else if (k == KEY_LEFT) strcpy(dest, "Left Arrow");
    else if (k == KEY_RIGHT) strcpy(dest, "Right Arrow");
    else if (k == KEY_UP) strcpy(dest, "Up Arrow");
    else if (k == KEY_DOWN) strcpy(dest, "Down Arrow");
    else if (k == KEY_SLASH_PAD) strcpy(dest, "/ (Pad)");
    else if (k == KEY_ASTERISK) strcpy(dest, "*");
    else if (k == KEY_MINUS_PAD) strcpy(dest, "- (Pad)");
    else if (k == KEY_PLUS_PAD) strcpy(dest, "+ (Pad)");
    else if (k == KEY_DEL_PAD) strcpy(dest, "Delete (Pad)");
    else if (k == KEY_ENTER_PAD) strcpy(dest, "Enter (Pad)");
    else if (k == KEY_PRTSCR) strcpy(dest, "Print Screen");
    else if (k == KEY_PAUSE) strcpy(dest, "Pause");
    else if (k == KEY_YEN) strcpy(dest, "Yen");
    else if (k == KEY_KANA) strcpy(dest, "Kana");
    else if (k == KEY_LSHIFT) strcpy(dest, "Left Shift");
    else if (k == KEY_RSHIFT) strcpy(dest, "Right Shift");
    else if (k == KEY_LCONTROL) strcpy(dest, "Left Control");
    else if (k == KEY_RCONTROL) strcpy(dest, "Right Control");
    else if (k == KEY_ALT) strcpy(dest, "Alt");
    else if (k == KEY_ALTGR) strcpy(dest, "Alt Gr");
    else if (k == KEY_LWIN) strcpy(dest, "Left Win");
    else if (k == KEY_RWIN) strcpy(dest, "Right Win");
    else if (k == KEY_MENU) strcpy(dest, "Menu");
    else if (k == KEY_SCRLOCK) strcpy(dest, "Scroll Lock");
    else if (k == KEY_NUMLOCK) strcpy(dest, "Num Lock");
    else if (k == KEY_CAPSLOCK) strcpy(dest, "Caps Lock");
	else strcpy(dest, "undefined");
}


// returns the current value of a slider
int get_slider_value(Tmenu_slider *s) {
	return s->value;
}


// sets the value of a slider
// returns FALSE if value was outside the limits
int set_slider_value(Tmenu_slider *s, int v) {
	if (s->min <= v && v <= s->max) {
		s->value = v; 
		return TRUE;
	}
	return FALSE;
}


// returns the current value of a slider
int get_selection_value(Tmenu_selection *s) {
	return s->value;
}


// sets the value of a selection
// returns FALSE if value was outside the limits
int set_selection_value(Tmenu_selection *s, int v) {
	if (0 <= v && v < s->size) {
		s->value = v; 
		return TRUE;
	}
	return FALSE;
}
