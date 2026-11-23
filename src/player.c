#include <stdio.h>
#include <string.h>
#include "player.h"

void initPlayer(Player *p, const char *username, const char *password) {
    strncpy(p->username, username, MAX_NAME - 1);
    p->username[MAX_NAME - 1] = '\0';
    strncpy(p->password, password, MAX_PASS - 1);
    p->password[MAX_PASS - 1] = '\0';
    p->hp = START_HP;
    p->level = START_LEVEL;
    p->score = START_SCORE;
    p->currentRoom = START_ROOM;
}

void printPlayer(const Player *p) {
    printf("Pemain : %s\n", p->username);
    printf("HP     : %d\n", p->hp);
    printf("Level  : %d\n", p->level);
    printf("Skor   : %d\n", p->score);
    printf("Ruangan: %d\n", p->currentRoom);
}
