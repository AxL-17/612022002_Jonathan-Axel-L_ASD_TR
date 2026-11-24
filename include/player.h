#ifndef PLAYER_H
#define PLAYER_H

#include "utils.h"

#define START_HP          100
#define START_LEVEL       1
#define START_SCORE       0
#define START_ROOM        0

#define PLAYER_BASE_ATTACK 10
#define LEVEL_HP_BONUS     10
#define SCORE_PER_LEVEL    100
#define HEAL_AFTER_WIN     20

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

int  getMaxHp(const Player *p);
void healPlayer(Player *p, int amount);      /* tidak melebihi HP maksimum */
int  updateLevel(Player *p);                 /* return 1 jika naik level */

#endif
