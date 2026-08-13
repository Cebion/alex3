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
 
    player.h   										  

    Handles a player.

 ******************************************************/


#ifndef _PLAYER_H_
#define _PLAYER_H_



#include "allegro.h"
#include "control.h"
#include "vehicle.h"



typedef struct {
    int car_id;     // control which car 
    int points;     // number of points scored
    Tcontrol ctrl;  
    Tvehicle *car;  // ptr to car
} Tplayer;


void init_player(Tplayer *p, int id);


#endif
