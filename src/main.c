#include <stdio.h>
#include "utils.h"
#include "player.h"
#include "auth.h"
#include "item.h"
#include "sorting.h"
#include "game.h"

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
    if (!loadCatalog(&catalog)) {
        freeInventory(&catalog);
        return 1;
    }

    int choice;
    do {
        showMainMenu();
        choice = readInt("Pilih: ", 0, 2);
        switch (choice) {
            case MENU_REGISTER:
                registerUser();
                break;
            case MENU_LOGIN: {
                Player player;
                if (loginUser(&player)) {
                    runGame(&player, &catalog);
                }
                break;
            }
            default:
                break;
        }
    } while (choice != MENU_EXIT);

    freeInventory(&catalog);
    printf("Sampai jumpa!\n");
    return 0;
}
