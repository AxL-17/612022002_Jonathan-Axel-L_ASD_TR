#include <stdio.h>
#include "battle.h"
#include "queue.h"

#define PLAYER_TURN  0
#define MONSTER_TURN 1

#define ACTION_ATTACK 1
#define ACTION_FLEE   2

static void announceStatus(const Player *player, const Monster *monster, int monsterHp) {
    printf("\n[%s HP %d/%d]  vs  [%s HP %d]\n", player->username, player->hp,
           getMaxHp(player), monster->name, monsterHp);
}

BattleResult runBattle(Player *player, const Monster *monster, int playerAttack) {
    int monsterHp = monster->hp;
    BattleResult result = BATTLE_FLED;
    Queue turns;

    initQueue(&turns);
    enqueue(&turns, PLAYER_TURN);
    enqueue(&turns, MONSTER_TURN);
    printf("\n*** %s muncul! ***\n", monster->name);

    int who;
    while (dequeue(&turns, &who)) {
        if (who == PLAYER_TURN) {
            announceStatus(player, monster, monsterHp);
            printf("1. Serang (kekuatan %d)  2. Kabur\n", playerAttack);
            if (readInt("Aksi: ", ACTION_ATTACK, ACTION_FLEE) == ACTION_FLEE) {
                printf("Kamu kabur dari pertarungan.\n");
                result = BATTLE_FLED;
                break;
            }
            monsterHp -= playerAttack;
            printf("Kamu menyerang %s! Damage %d.\n", monster->name, playerAttack);
            if (monsterHp <= 0) {
                printf("%s dikalahkan!\n", monster->name);
                result = BATTLE_WON;
                break;
            }
        } else {
            player->hp -= monster->attack;
            printf("%s menyerangmu! Damage %d.\n", monster->name, monster->attack);
            if (player->hp <= 0) {
                player->hp = 0;
                printf("Kamu tumbang...\n");
                result = BATTLE_LOST;
                break;
            }
        }
        enqueue(&turns, who);   /* yang masih hidup antre lagi di belakang */
    }
    freeQueue(&turns);
    return result;
}
