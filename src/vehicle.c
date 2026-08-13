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
 
    vehicle.c  										  

    Routines for handling the the vehicles in the game.
    Collision code partly done by Bob:
        http://pages.infinit.net/voidstar/

 ******************************************************/


#include <math.h>
#include "allegro.h"
#include "main.h"
#include "map.h"
#include "vehicle.h"

// returns true if the vehicles are touching each other
int check_collision(Tvehicle *v1, Tvehicle *v2) {
    int hyp = fixtoi(fixhypot(v1->pos.x - v2->pos.x, v1->pos.y - v2->pos.y));
    fixed car_angle;
    fixed a1, a2, h1, h2;

    if (hyp > v1->radius + v2->radius) return FALSE;

    // get angle between cars
    car_angle = fixatan2(v1->pos.y - v2->pos.y, v1->pos.x - v2->pos.x);

    // get velocity vectors (polar)
    h1 = fixhypot(v1->vel.x, v1->vel.y);
    a1 = fixatan2(v1->vel.x, v1->vel.y);
    h2 = fixhypot(v2->vel.x, v2->vel.y);
    a2 = fixatan2(v2->vel.x, v2->vel.y);

    // apply forces
    v2->vel.x -= 0.9 * fixmul(h1, fixcos(car_angle)) - 0.3 * fixmul(h2, fixcos(car_angle - itofix(128)));
    v2->vel.y -= 0.9 * fixmul(h1, fixsin(car_angle)) - 0.3 * fixmul(h2, fixsin(car_angle - itofix(128)));
    v1->vel.x -= 0.9 * fixmul(h2, fixcos(car_angle - itofix(128))) - 0.3 * fixmul(h1, fixcos(car_angle));
    v1->vel.y -= 0.9 * fixmul(h2, fixsin(car_angle - itofix(128))) - 0.3 * fixmul(h1, fixsin(car_angle));

    return TRUE;
}


// sets initial values to the vehiclew
void init_vehicle(Tvehicle *v, BITMAP *bmp) { 
    // car specific
	v->pos.x = 0;
	v->pos.y = 0;
	v->vel.x = 0;
	v->vel.y = 0;

	v->angle = 0;
	v->spin = 0;

	v->steer_angle = 0;
	v->throttle = 0;
	v->brake = 0;

    // stuff
    v->image = bmp;
    // size
    v->w = bmp->w; 
    v->h = bmp->h;
    v->radius = fixtoi(fixhypot(itofix(v->w), itofix(v->h))) >> 1;
    v->radius -= 8;
    v->slip = 0;

    v->next_checkpoint = 1;
    v->in_goal = 0;
    v->next_ai_point = 0;
    v->ai_on = 1;
    v->place = 0;
}

// updates the vehicle according to the forces that applies to it
void update_vehicle(Tvehicle *v) {
    fixed vel;

    // update velocity
    v->vel.x *= 0.95;
    v->vel.y *= 0.95;
    if (v->slip) v->spin *= 0.85;
    else v->spin *= 0.6;

    vel = fixhypot(v->vel.x, v->vel.y);

    if (vel > ftofix(0.1))
        v->angle -= (SGN(v->throttle - v->brake)) * (v->steer_angle * 0.05);

    if (v->slip) v->spin += v->steer_angle * 0.05;
    else v->spin += v->steer_angle * 0.02;

    // hack to make AI better
    if (v->ai_on) {
        v->spin *= 0.4;
        v->vel.x *= 0.8;
        v->vel.y *= 0.8;
    }


    v->vel.x += fixmul(v->throttle - v->brake, fixcos(v->angle));
    v->vel.y += fixmul(v->throttle - v->brake, fixsin(v->angle));

    // update position
    v->pos.x += v->vel.x;
    v->pos.y += v->vel.y;
    v->angle -= v->spin; 
}


// draws the vehicle v on bmp with offset ox, oy
void draw_vehicle(BITMAP *bmp, Tvehicle *v, int ox, int oy) {
    int x = fixtoi(v->pos.x) - ox;
    int y = fixtoi(v->pos.y) - oy;

    // border
    /*
	int hyp = fixtoi(fixhypot(itofix(v->w), itofix(v->h)))>>1;
	fixed ang = fixatan2(itofix(v->w), itofix(v->h));
	int cx1 = fixtoi(hyp*fixcos(ang+v->angle));
	int cy1 = fixtoi(hyp*fixsin(ang+v->angle));
	int cx2 = fixtoi(hyp*fixcos(-ang+v->angle+itofix(128)));
	int cy2 = fixtoi(hyp*fixsin(-ang+v->angle+itofix(128)));
	int cx3 = fixtoi(hyp*fixcos(ang+v->angle+itofix(128)));
	int cy3 = fixtoi(hyp*fixsin(ang+v->angle+itofix(128)));
	int cx4 = fixtoi(hyp*fixcos(-ang+v->angle));
	int cy4 = fixtoi(hyp*fixsin(-ang+v->angle));
    */

    v->workbench = create_bitmap(v->image->w + (v->rear_tire->w << 1), v->image->h);
    clear(v->workbench);
    // draw rear tires
    draw_sprite(v->workbench, v->rear_tire, 6, v->h - 19);
    draw_sprite(v->workbench, v->rear_tire, v->workbench->w - v->rear_tire->w - 6, v->h - 19);
    // draw front tires
    rotate_sprite(v->workbench, v->front_tire, 10, 5, -v->steer_angle);
    rotate_sprite(v->workbench, v->front_tire, v->workbench->w - v->front_tire->w - 10, 5, -v->steer_angle);
    // draw chassi
    draw_sprite(v->workbench, v->image, v->rear_tire->w, 0);

    rotate_sprite(bmp, v->workbench, x - (v->workbench->w >> 1), y - (v->workbench->h >> 1), v->angle + itofix(64));

    //textprintf(bmp, font, x, y, 15, "l:%d c:%d", v->laps, v->next_checkpoint);
    
    // draw border
    //circle(bmp, x, y, v->radius, (v->collided ? 26 : 15));
    /*
    line(bmp, x - cx1, y - cy1, x - cx2, y - cy2, 15);
	line(bmp, x - cx1, y - cy1, x - cx4, y - cy4, 15);
	line(bmp, x - cx3, y - cy3, x - cx2, y - cy2, 15);
	line(bmp, x - cx3, y - cy3, x - cx4, y - cy4, 15);
    */

    destroy_bitmap(v->workbench);
}


// checks the vehicle against the map 
// for checkpoints and bumps
// returns 1 if car is in goal
int check_vehicle_against_map(Tvehicle *v, Tmap *m) {
    int tx, ty, pos, dx, dy, x, y;
    int i;
    int ret = 0;

    tx = fixtoi(v->pos.x) >> 6;
    ty = fixtoi(v->pos.y) >> 6;

    pos = tx + ty * m->width;

    
    // check for checkpoints
    if (m->dat[pos].checkpoint == v->next_checkpoint) {
        v->next_checkpoint ++;
    }

    // check for goal line
    if (m->dat[pos].type & TYPE_GOAL) {
        if (m->max_checkpoints == v->next_checkpoint - 1) {
            if (m->max_checkpoints == v->next_checkpoint - 1) v->laps ++;
            v->next_checkpoint = 1;
            if (v->laps >= m->num_laps && !v->in_goal) {
                v->in_goal++;
                ret = 1;
            }
        }
    }

    // check for slippy ground
    if (m->dat[pos].type & TYPE_SLIP) v->slip = TRUE;
    else v->slip = FALSE;

    // check for bumpers
    for(i=0;i<256;i+=32) {
        y = fixtoi(v->pos.y + v->radius * fixsin(itofix(i)));
        ty = y >> 6;
        dy = y - (ty << 6);
        
        x = fixtoi(v->pos.x + v->radius * fixcos(itofix(i)));
        tx = x >> 6;
        dx = x - (tx << 6);
        
        if (m->dat[tx + ty * m->width].type & TYPE_BUMP) {
            if (i == 0) {
                v->pos.x -= itofix(dx);
                v->vel.x = -v->vel.x * 0.7;
                v->vel.y *= 0.7;
            }
            else if (i == 64) {
                v->pos.y -= itofix(dy);
                v->vel.y = -v->vel.y * 0.7;
                v->vel.x *= 0.7;
            }
            else if (i == 128) {
                v->pos.x += itofix(64 - dx);
                v->vel.x = -v->vel.x * 0.7;
                v->vel.y *= 0.7;
            }
            else if (i == 192) {
                v->pos.y += itofix(64 - dy);
                v->vel.y = -v->vel.y * 0.7;
                v->vel.x *= 0.7;
            }
        }
    }
    
    // check against outer border
    if (fixtoi(v->pos.x) < v->radius) {
        v->pos.x = itofix(v->radius);
        v->vel.x = -v->vel.x * 0.8;
    }
    if (fixtoi(v->pos.y) < v->radius) {
        v->pos.y = itofix(v->radius);
        v->vel.y = -v->vel.y * 0.8;
    }
    if (fixtoi(v->pos.x) > (m->width << 6) - v->radius) {
        v->pos.x = itofix((m->width << 6) - v->radius);
        v->vel.x = -v->vel.x * 0.8;
    }
    if (fixtoi(v->pos.y) > (m->height << 6) - v->radius) {
        v->pos.y = itofix((m->height << 6) - v->radius);
        v->vel.y = -v->vel.y * 0.8;
    }

    return ret;
}