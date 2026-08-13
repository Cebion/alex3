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


#include <stdio.h>
#include "options.h"

// save the option struct to disk using fp
void save_options(Toptions *o, PACKFILE *fp) {
	pack_fwrite(o, sizeof(Toptions), fp);
}


// loads the option struct from disk using fp
void load_options(Toptions *o, PACKFILE *fp) {
	pack_fread(o, sizeof(Toptions), fp);
}
