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
 
    player.c 										  

    Handles a player.

 ******************************************************/



#include "allegro.h"
#include "control.h"
#include "player.h"
#include "vehicle.h"



void init_player(Tplayer *p, int id) {
    p->car_id = id;
    init_control(&p->ctrl);
    p->points = 0;
    p->car = NULL;
}

