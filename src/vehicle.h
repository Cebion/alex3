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
 
    vehicle.h   										  

    Routines for handling the the vehicles in the game.

 ******************************************************/



#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include "allegro.h"
#include "map.h"



typedef struct {
	fixed 	x,y;
} Tvector;


typedef struct {
	Tvector	pos;		// position of car centre in world coordinates
	Tvector	vel;		// velocity vector of car in world coordinates

	fixed	angle;				// angle of car body orientation (in rads)
	fixed	spin;

	fixed	steer_angle;			// angle of steering (input)
	fixed	throttle;			// amount of throttle (input)
	fixed	brake;				// amount of braking (input)

    fixed   grip;               // variable

    // sizes
    int w, h;

    // my data
    BITMAP *workbench;
    BITMAP *image;
    BITMAP *front_tire;
    BITMAP *rear_tire;

    // stuff
    int collided;
    int radius;
    int slip;

    // track related
    int next_checkpoint;
    int laps;
    int in_goal;
    int next_ai_point;
    fixed target_angle;
    int ai_on;
    int place;
} Tvehicle;


void init_vehicle(Tvehicle *v, BITMAP *bmp); 
void update_vehicle(Tvehicle *car);
void draw_vehicle(BITMAP *bmp, Tvehicle *v, int ox, int oy);

int check_collision(Tvehicle *v1, Tvehicle *v2);

int check_vehicle_against_map(Tvehicle *v, Tmap *m);

#endif