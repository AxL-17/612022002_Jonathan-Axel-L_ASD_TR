#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "player.h"
#include "auth.h"
#include "item.h"
#include "sorting.h"
#include "game.h"
#include "graph.h"
#include "hashtable.h"
#include "monster.h"

#define MENU_REGISTER 1
#define MENU_LOGIN    2
#define MENU_EXIT     0

static void showMainMenu(void) {
    printf("\n=== DUNGEON OF ASD ===\n");
    printf("1. Register\n");
    printf("2. Login\n");
    printf("0. Keluar\n");
}

static int loadCatalog(Inventory *catalog) {
    initInventory(catalog);
    if (!loadItemsFromFile(CATALOG_FILE, catalog)) {
        printf("Gagal membaca %s. Jalankan program dari folder utama proyek.\n", CATALOG_FILE);
        return 0;
    }
    /* Urutkan by ID agar katalog bisa dicari dengan binary search. */
    mergeSort(catalog->data, catalog->size, SORT_BY_ID, ORDER_ASCENDING);
    return 1;
}

int main(void) {
    Inventory catalog;
    Graph dungeon;

    if (!loadCatalog(&catalog)) {
        freeInventory(&catalog);
        return 1;
    }
    initGraph(&dungeon);
    if (!loadGraph(&dungeon, DUNGEON_FILE)) {
        printf("Gagal membaca %s. Jalankan program dari folder utama proyek.\n", DUNGEON_FILE);
        freeInventory(&catalog);
        return 1;
    }

    MonsterCatalog monsters;
    if (!loadMonsters(MONSTERS_FILE, &monsters)) {
        printf("Gagal membaca %s. Jalankan program dari folder utama proyek.\n", MONSTERS_FILE);
        freeInventory(&catalog);
        freeGraph(&dungeon);
        return 1;
    }

    HashTable itemIndex;
    initHashTable(&itemIndex);
    for (int i = 0; i < catalog.size; i++) {
        hashInsert(&itemIndex, &catalog.data[i]);
    }
    GameData gameData = { &catalog, &dungeon, &itemIndex, &monsters };

    int choice;
    do {
        getchar();
        system("cls");
        showMainMenu();
        choice = readInt("Pilih: ", 0, 2);
        switch (choice) {
            case MENU_REGISTER:
                registerUser();
                break;
            case MENU_LOGIN: {
                Player player;
                if (loginUser(&player)) {
                    runGame(&player, &gameData);
                }
                break;
            }
            default:
                break;
        }
    } while (choice != MENU_EXIT);

    freeInventory(&catalog);
    freeGraph(&dungeon);
    freeHashTable(&itemIndex);
    printf("Sampai jumpa!\n");
    return 0;
}
