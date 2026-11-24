#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "item.h"
#include "graph.h"

/* Menjalankan sesi game untuk pemain yang sudah login. */
void runGame(Player *player, const Inventory *catalog, const Graph *dungeon);

#endif
