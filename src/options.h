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
 
    options.h   										  

    Routines for handling the options available to 
    the user. 

 ******************************************************/



#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include "allegro.h"

// the available options
typedef struct {
	int snd_volume;
	int msc_volume;
	int use_joy;
	int full_screen;
} Toptions;

void save_options(Toptions *o, PACKFILE *fp);
void load_options(Toptions *o, PACKFILE *fp);

#endif