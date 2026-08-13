#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "allegro.h"

#define MAX_PARTICLES		512

typedef struct {
	int intensity;
	fixed x, y;
	fixed sx, sy;
	int color;
} Tparticle;

int create_particle(Tparticle *p, int x, int y);
void reset_particles(Tparticle *p);
void update_particle(Tparticle *p);

#endif