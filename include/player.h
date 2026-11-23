#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"

#define START_HP    100
#define START_LEVEL 1
#define START_SCORE 0
#define START_ROOM  0

typedef struct {
    char username[MAX_NAME];
    char password[MAX_PASS];
    int  hp;
    int  level;
    int  score;
    int  currentRoom;
} Player;

void initPlayer(Player *p, const char *username, const char *password);
void printPlayer(const Player *p);

#endif
