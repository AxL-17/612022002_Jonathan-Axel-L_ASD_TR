#ifndef MONSTER_H
#define MONSTER_H

#include "utils.h"

#define MAX_MONSTERS  20
#define MONSTERS_FILE "data/monsters.txt"

typedef struct {
    int  id;
    char name[MAX_NAME];
    int  hp;
    int  attack;
    int  reward;     /* skor yang didapat jika menang */
} Monster;

typedef struct {
    Monster list[MAX_MONSTERS];
    int count;
} MonsterCatalog;

int  loadMonsters(const char *path, MonsterCatalog *catalog);
void printMonsterTable(const MonsterCatalog *catalog);

#endif
