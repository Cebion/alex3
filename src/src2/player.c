
#include "allegro.h"
#include "main.h"
#include "player.h"

void reset_player(Tplayer *p) {
	p->sx = 0;
	p->sy = 0;
	p->level = 0;
	p->status = 0;
	p->jump_key = 1;
	p->frame = 0;
	p->level = 0;
	p->in_combo = 0;
	p->acc_level = 0;
	p->acc_jumps = 0;
	p->score = 0;
	p->dead = 0;
	p->max_s = 0;
	p->rotate = 0;
	p->edge = 0;
	p->edge_drawn = 0;
	p->bounce = 0;
	p->shake = 0;
}

void update_player(Tplayer *p) {
	p->sy = MID(-100.0, p->sy, 12.0);
	p->sx = MID(-12.0, p->sx, 12.0);

	p->x += p->sx;
	p->y += p->sy;

	if (p->y > 1000) p->y = 1000;

	if (p->x > 555.0) {
		p->x = 555.0;
		p->sx = -p->sx * 0.9;
		if (ABS(p->sx) > 4) p->bounce = -20;
	}
	if (p->x < 85.0) {
		p->x = 85.0;
		p->sx = -p->sx * 0.9;
		if (ABS(p->sx) > 4) p->bounce = 20;
	}

	if (p->status != 0) p->sy += 0.8;
	if (p->status == P_JUMPING_UP && p->sy > 0) p->status = P_JUMPING_DOWN;
}

int jump_player(Tplayer *p) {
	if (p->status) return FALSE;

	p->status = P_JUMPING_UP;
	p->sy = MIN(-ABS(p->sx*2), -12.0);
	p->max_s = p->sx;
	if (p->sy < -22) p->rotate = 1;
	p->angle = 0;

	return TRUE;
}

