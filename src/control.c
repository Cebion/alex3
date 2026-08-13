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
 
    control.c   										  

    Handles all control issues. Keyboard, joystick,
    whatever.

 ******************************************************/


#include "allegro.h"
#include "control.h"


// "constructor"
// sets some initial values
void init_control(Tcontrol *c) {
	set_control(c,  KEY_UP,
					KEY_DOWN,
					KEY_LEFT,
					KEY_RIGHT,
					KEY_SPACE);

	c->key_enter = KEY_ENTER;
	c->flags = 0;
	c->use_joy = 0;
}


// sets the desired keys for the control unit
void set_control(Tcontrol *c, int up, int down, int left, int right, int fire) {
	c->key_up     = up;
	c->key_down   = down;
	c->key_left   = left;
	c->key_right  = right;
	c->key_fire	  = fire;
}


// updates the control unit
void poll_control(Tcontrol *c) {
	c->flags = 0;

	if (c->use_joy) {
		poll_joystick();
		
		if (joy[0].stick[0].axis[1].d1)	c->flags |= K_UP;
		if (joy[0].stick[0].axis[1].d2)	c->flags |= K_DOWN;
		if (joy[0].stick[0].axis[0].d1)	c->flags |= K_LEFT;
		if (joy[0].stick[0].axis[0].d2)	c->flags |= K_RIGHT;
		if (joy[0].button[0].b)	c->flags |= K_FIRE;
	}

	if (key[c->key_up]) c->flags |= K_UP;
	if (key[c->key_down]) c->flags |= K_DOWN;
	if (key[c->key_left]) c->flags |= K_LEFT;
	if (key[c->key_right]) c->flags |= K_RIGHT;
	if (key[c->key_fire]) c->flags |= K_FIRE;
	if (key[c->key_enter]) c->flags |= K_ENTER;
}


// check if a key in the control is available
int check_control_key(Tcontrol *c, int key) {
	if (key == c->key_left) return TRUE;
	if (key == c->key_right) return TRUE;
	if (key == c->key_up) return TRUE;
	if (key == c->key_down) return TRUE;
	if (key == c->key_fire) return TRUE;
	if (key == c->key_enter) return TRUE;
	return FALSE;
}


// wipes all the flags
void clear_control(Tcontrol *c) {
    c->flags = 0;
}



// returns true or false depending on if keys are pressed or not
int is_up(Tcontrol *c)       { return (c->flags & K_UP       ? TRUE : FALSE); }
int is_down(Tcontrol *c)     { return (c->flags & K_DOWN     ? TRUE : FALSE); }
int is_left(Tcontrol *c)     { return (c->flags & K_LEFT     ? TRUE : FALSE); }
int is_right(Tcontrol *c)    { return (c->flags & K_RIGHT    ? TRUE : FALSE); }
int is_fire(Tcontrol *c)     { return (c->flags & K_FIRE     ? TRUE : FALSE); }
int is_enter(Tcontrol *c)    { return (c->flags & K_ENTER    ? TRUE : FALSE); }
int is_any(Tcontrol *c)      { return (c->flags              ? TRUE : FALSE); }


// saves the control config to disk using FP
void save_control(Tcontrol *c, PACKFILE *fp) {
	pack_fwrite(c, sizeof(Tcontrol), fp);
}


// loads the control config from disk using FP
void load_control(Tcontrol *c, PACKFILE *fp) {
	pack_fread(c, sizeof(Tcontrol), fp);
}
