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
 
    hisc.h   										  

    All high score routines. 

 ******************************************************/



#include <string.h>
#include "allegro.h"
#include "hisc.h"


// creates a table to work with
Thisc* make_hisc_table() {
	Thisc *tmp;

	tmp = malloc(MAX_SCORES*sizeof(Thisc));
	if (!tmp) return NULL;

	return tmp;
}


// destroys a table 
void destroy_hisc_table(Thisc *table) {
	free(table);
}


// check if provided score is allowed to enter the table
int qualify_hisc_table(Thisc *table, Thisc post) {
	int i;
	if (post.score == 0) return 0;
	for (i=0;i<MAX_SCORES;i++)
		if (post.score >= table[i].score) return 1;
	return 0;
}

 
// sorts the table
void sort_hisc_table(Thisc *table) {
	int i,j;
	Thisc tmp;

	for (i=1;i<MAX_SCORES;i++) {
		j=i;
		tmp = table[i];
		while (j!=0 && tmp.score > table[j-1].score) {
			table[j]= table[j-1];
			j--;
		}
		table[j] = tmp;
	}
}


// Replaces the lowest score with the specified
void enter_hisc_table(Thisc *table, Thisc post) { 
	unsigned int lo=10000000;
	int loID=-1;
	int i;

	// find lowest score
	for (i=0;i<MAX_SCORES;i++) {
		if (table[i].score < lo) {
			loID = i;
			lo = table[i].score;
		}
	}

	if (loID>=0) table[loID] = post;
}


// Resets the table to the values specified
void reset_hisc_table(Thisc *table, char *name, int hi, int lo) {
	int i;
	int d = (hi-lo)/MAX_SCORES;
	int acc = hi;

	for (i=0;i<MAX_SCORES;i++) {
		strcpy(table[i].name, name);
		table[i].score = acc;
		table[i].floor = acc / 20 + rand()%7;
		acc-=d;
	}
}


// Loads table from disk, returns 1 on success
int load_hisc_table(Thisc *table, PACKFILE *fp) {
	pack_fread(table, MAX_SCORES*sizeof(Thisc), fp);
	return 1;
}


// saves table to disk
void save_hisc_table(Thisc *table, PACKFILE *fp) {
	pack_fwrite(table, MAX_SCORES * sizeof(Thisc), fp);
}


// draws a single hisc post
void draw_hisc_post(Thisc *table, BITMAP *bmp, FONT *fnt, int x, int y) {
	textprintf(bmp, fnt, x, y, -1, "%s", table->name);
	textprintf_right(bmp, fnt, x+100, y, -1, "%d", table->floor);
	textprintf_right(bmp, fnt, x+200, y, -1, "%d", table->score);
}


// draws the entire table
void draw_hisc_table(Thisc *table, BITMAP *bmp, FONT *fnt, int x, int y) {
	int i;

	for(i=0;i<MAX_SCORES;i++)
		draw_hisc_post(&table[i], bmp, fnt, x, y + (text_height(fnt) - 5) * (i+1));
}




