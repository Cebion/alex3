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
 
    rally.h   										  

    Handles a race.

 ******************************************************/


#ifndef _RALLY_H_
#define _RALLY_H_



#include "allegro.h"
#include "vehicle.h"
#include "map.h"


#define MAX_CARS    6



typedef struct {
    Tvehicle cars[MAX_CARS];
    Tmap *track;
} Trally;



#endif
