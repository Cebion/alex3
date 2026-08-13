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

 ******************************************************/



#ifndef _MAIN_H_
#define _MAIN_H_


void my_alert(char *func, char *txt);
void blit_to_screen(BITMAP *bmp);
void play_menu_move();
void play_menu_select();
void take_screenshot(BITMAP *bmp, char *fname);

#endif