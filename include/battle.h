#ifndef BATTLE_H
#define BATTLE_H

#include "player.h"
#include "monster.h"

typedef enum {
    BATTLE_WON,
    BATTLE_LOST,
    BATTLE_FLED
} BattleResult;

/* Pertarungan bergiliran; urutan giliran diatur oleh queue. */
BattleResult runBattle(Player *player, const Monster *monster, int playerAttack);

#endif
