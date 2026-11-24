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

int getMaxHp(const Player *p) {
    return START_HP + (p->level - 1) * LEVEL_HP_BONUS;
}

void healPlayer(Player *p, int amount) {
    p->hp += amount;
    if (p->hp > getMaxHp(p)) {
        p->hp = getMaxHp(p);
    }
}

int updateLevel(Player *p) {
    int newLevel = START_LEVEL + p->score / SCORE_PER_LEVEL;
    if (newLevel > p->level) {
        p->level = newLevel;
        return 1;
    }
    return 0;
}

void printPlayer(const Player *p) {
    printf("Pemain : %s\n", p->username);
    printf("HP     : %d/%d\n", p->hp, getMaxHp(p));
    printf("Level  : %d\n", p->level);
    printf("Skor   : %d\n", p->score);
    printf("Ruangan: %d\n", p->currentRoom);
}
