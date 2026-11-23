#include <stdio.h>
#include "utils.h"
#include "player.h"
#include "auth.h"

#define MENU_REGISTER 1
#define MENU_LOGIN    2
#define MENU_EXIT     0

#define GAME_STATUS 1
#define GAME_LOGOUT 0

static void showMainMenu(void) {
    printf("\n=== DUNGEON OF ASD ===\n");
    printf("1. Register\n");
    printf("2. Login\n");
    printf("0. Keluar\n");
}

static void showGameMenu(void) {
    printf("\n--- Menu Game ---\n");
    printf("1. Lihat status pemain\n");
    printf("0. Simpan & Logout\n");
}

/* Tempat modul lain (battle, inventori, bestiary, dll.) akan dipasang. */
static void runGame(Player *player) {
    int choice;
    do {
        showGameMenu();
        choice = readInt("Pilih: ", 0, 1);
        if (choice == GAME_STATUS) {
            printPlayer(player);
        }
    } while (choice != GAME_LOGOUT);

    if (savePlayer(player)) {
        printf("Progres tersimpan. Logout berhasil.\n");
    } else {
        printf("Peringatan: progres gagal disimpan.\n");
    }
}

int main(void) {
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
                    runGame(&player);
                }
                break;
            }
            default:
                break;
        }
    } while (choice != MENU_EXIT);

    printf("Sampai jumpa!\n");
    return 0;
}
