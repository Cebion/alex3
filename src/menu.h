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



#ifndef _MENU_H_
#define _MENU_H_

#include "allegro.h"
#include "main.h"
#include "control.h"

// actions					(returnValue)
#define IDM_NEWGAME1			101		// start new game
#define IDM_NEWGAME2			102		// start new game
#define IDM_SUBMENU				103		// open a sub menu
#define IDM_INSTRUCTIONS		104		// show instructions
#define IDM_HIGHSCORES			105		// show hiscore list
#define IDM_CREDITS				106		// show credits
#define IDM_EXIT_TO_OS			107		// exit to os
#define IDM_BACK				108		// leave sub menu
#define IDM_INCREASE			109		// increase the value of a sliderbar
#define IDM_DECREASE			110		// decrease the value of a sliderbar
#define IDM_PREV				111		// previous item in a selection
#define IDM_NEXT				112		// next     item in a selection
#define IDM_TOGGLE_CHECKBOX		113		// toggles the value of a check box
#define IDM_GET_KEY				114		// gets a key from user 
#define IDM_RESUME				115	
#define IDM_END_GAME			116	


// menu post flags
#define MF_SELECTED				0x0001
#define MF_SLIDER				0x0002
#define MF_CHECKBOX				0x0004
#define MF_SELECTION			0x0008
#define MF_KEY					0x0010
#define MF_LAST					0x0020


// define the parameters for a menu
typedef struct {
	FONT *font;
	int font_height;
	Tcontrol ctrl;
	BITMAP *bullet;
	int pos;
} Tmenu_params;


// define a single menu post
typedef struct {
	char	caption[128];
	int		return_select;
	int		return_left;
	int		return_right;
	int		flags;
	void	*data;
} Tmenu;


// define a slidebar
typedef struct {
	int value;
	int min,
		max,
		step;
} Tmenu_slider;


// define a selection menu
typedef struct {
	int value;
	int size;
	char *caption[];
} Tmenu_selection;


void reset_menu(Tmenu *m, Tmenu_params *mp, int selPos);
void draw_menu(BITMAP *bmp, Tmenu *m, Tmenu_params *mp, int x, int y);
int handle_menu(Tmenu *menu, Tmenu_params *mp, Tcontrol *ctrl, BITMAP *bmp, void (*callback)(void), int x, int y);
void key_to_str(int k, char *dest);

int get_slider_value(Tmenu_slider *s);
int set_slider_value(Tmenu_slider *s, int v);
int get_selection_value(Tmenu_selection *s);
int set_selection_value(Tmenu_selection *s, int v);

#endif