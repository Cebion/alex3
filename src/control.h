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
 
    control.h   										  

    Handles all control issues. Keyboard, joystick,
    whatever.

 ******************************************************/


#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <stdio.h>

// define posisble keys to use
#define	K_LEFT		0x0001
#define	K_RIGHT		0x0002
#define K_UP		0x0004
#define K_DOWN		0x0008
#define	K_FIRE		0x0010
#define	K_ENTER		0x0020

// the control struct
typedef struct {
	int use_joy;
	int key_left,				// actual keys
		key_right,
		key_up,
		key_down,
		key_fire,
		key_enter;

	unsigned int flags;					// bit field w/ info on which keys that are down
} Tcontrol;


void init_control(Tcontrol *c);
void save_control(Tcontrol *c, PACKFILE *fp);
void load_control(Tcontrol *c, PACKFILE *fp);
void set_control(Tcontrol *c, int up, int down, int left, int right, int fire);
void poll_control(Tcontrol *c);
void clear_control(Tcontrol *c);
int check_control_key(Tcontrol *c, int key);
int is_up(Tcontrol *c);
int is_down(Tcontrol *c);
int is_left(Tcontrol *c);
int is_right(Tcontrol *c);
int is_fire(Tcontrol *c);
int is_enter(Tcontrol *c);
int is_any(Tcontrol *c);

#endif
