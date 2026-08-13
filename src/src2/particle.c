
#include "allegro.h"
#include "particle.h"


int create_particle(Tparticle *p, int x, int y) {
	int i=0;
	while(i<MAX_PARTICLES && p[i].intensity) i++;
	if (i == MAX_PARTICLES) return 0;
		
	p[i].x = itofix(x);
	p[i].y = itofix(y);
	p[i].sx = itofix(rand()%50-25)/10;
	p[i].sy = itofix(rand()%50-25)/50;
	p[i].intensity = 255;
	p[i].color = rand()%8;
	
	return i;
}

void update_particle(Tparticle *p) {
	p->x += p->sx;
	p->y += p->sy;
	p->sy += ftofix(0.3);
	p->intensity--;
	if (rand()%5 == 1) p->color = rand()%8;
}

void reset_particles(Tparticle *p) {
	int i;
	for(i=0;i<MAX_PARTICLES;i++) p[i].intensity = 0;
}
