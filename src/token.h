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
 
    token.h  										  

    Simple tokenizer.
    
 ******************************************************/

#ifndef _TOKEN_H_
#define _TOKEN_H_



typedef struct {
    char *word;
    struct Ttoken *next;
} Ttoken;

Ttoken *create_token(char *word);
void destroy_token(Ttoken *t);
void flush_tokens(Ttoken *head);
void insert_token(Ttoken *list, Ttoken *t);
char *get_next_word(Ttoken *t);

Ttoken *tokenize(char *str);


#endif