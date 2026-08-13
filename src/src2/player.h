
#define P_JUMPING_UP		1
#define P_JUMPING_DOWN		2
#define P_FALLING			3


typedef struct {
	double x, y;
	double sx, sy;
	double max_s;
	int level;
	int score;
	int status;
	int jump_key;
	int frame;
	int in_combo;
	int acc_level;
	int acc_jumps;
	int dead;
	int rotate;
	fixed angle;
	int edge;
	int edge_drawn;
	int bounce;
	int shake;
} Tplayer;

void reset_player(Tplayer *p);
void update_player(Tplayer *p);
int jump_player(Tplayer *p);