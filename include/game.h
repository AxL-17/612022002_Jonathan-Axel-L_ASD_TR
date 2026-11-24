#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "item.h"
#include "graph.h"
#include "hashtable.h"
#include "monster.h"

/* Data bersama (hanya dibaca) yang dimuat sekali saat program mulai. */
typedef struct {
    const Inventory *catalog;
    const Graph *dungeon;
    const HashTable *itemIndex;
    const MonsterCatalog *monsters;
} GameData;

/* Menjalankan sesi game untuk pemain yang sudah login. */
void runGame(Player *player, const GameData *data);

#endif
