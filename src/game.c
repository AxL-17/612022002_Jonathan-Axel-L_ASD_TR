#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "game.h"
#include "auth.h"
#include "sorting.h"
#include "searching.h"
#include "stack.h"
#include "bst.h"
#include "battle.h"

#define MENU_LOGOUT      0
#define MENU_STATUS      1
#define MENU_EXPLORE     2
#define MENU_UNDO        3
#define MENU_MAP         4
#define MENU_TAKE_ITEM   5
#define MENU_INVENTORY   6
#define MENU_SORT        7
#define MENU_SEARCH      8
#define MENU_HASH        9
#define MENU_BATTLE      10
#define MENU_BESTIARY    11
#define MENU_MAX         11

#define MAP_SHOW  1
#define MAP_BFS   2
#define MAP_DFS   3

#define ALGO_BUBBLE 1
#define ALGO_MERGE  2

#define SEARCH_SEQUENTIAL 1
#define SEARCH_BINARY     2

#define MAX_ITEM_ID 9999

#define BESTIARY_PREORDER  1
#define BESTIARY_INORDER   2
#define BESTIARY_POSTORDER 3
#define BESTIARY_SEARCH    4
#define BESTIARY_DELETE    5

#define HASH_LOOKUP 1
#define HASH_SHOW   2

static void showGameMenu(void) {
    printf("\n--- Menu Game ---\n");
    printf("1. Lihat status pemain\n");
    printf("2. Jelajah (pindah ruangan)\n");
    printf("3. Undo langkah (stack)\n");
    printf("4. Peta dungeon (graph, BFS, DFS)\n");
    printf("5. Ambil item dari katalog\n");
    printf("6. Lihat inventori\n");
    printf("7. Urutkan inventori (sorting)\n");
    printf("8. Cari item di inventori (searching)\n");
    printf("9. Katalog item (hash table)\n");
    printf("10. Battle monster\n");
    printf("11. Bestiary (BST)\n");
    printf("0. Simpan & Logout\n");
}

static void takeItem(Inventory *inventory, const Inventory *catalog) {
    printItemTable(catalog->data, catalog->size);
    int id = readInt("Masukkan ID item yang diambil (0 = batal): ", 0, MAX_ITEM_ID);
    if (id == 0) {
        return;
    }
    /* Katalog sudah diurutkan by ID saat program mulai, jadi binary search aman. */
    int index = binarySearchById(catalog->data, catalog->size, id);
    if (index == NOT_FOUND) {
        printf("Item dengan ID %d tidak ditemukan di katalog.\n", id);
        return;
    }
    if (addItem(inventory, &catalog->data[index])) {
        printf("Item '%s' masuk ke inventori.\n", catalog->data[index].name);
    } else {
        printf("Gagal menambah item (memori habis).\n");
    }
}

static void sortInventory(Inventory *inventory) {
    if (inventory->size == 0) {
        printf("Inventori kosong, tidak ada yang diurutkan.\n");
        return;
    }
    printf("Algoritma: 1. Bubble Sort  2. Merge Sort\n");
    int algo = readInt("Pilih algoritma: ", ALGO_BUBBLE, ALGO_MERGE);
    printf("Urut berdasarkan: 1. ID  2. Nama  3. Power  4. Harga\n");
    int field = readInt("Pilih kolom: ", SORT_BY_ID, SORT_BY_PRICE);
    printf("Urutan: 1. Ascending  2. Descending\n");
    int ascending = (readInt("Pilih urutan: ", 1, 2) == 1) ? ORDER_ASCENDING : ORDER_DESCENDING;

    if (algo == ALGO_BUBBLE) {
        bubbleSort(inventory->data, inventory->size, (SortField)field, ascending);
    } else if (!mergeSort(inventory->data, inventory->size, (SortField)field, ascending)) {
        printf("Merge sort gagal (memori habis).\n");
        return;
    }
    printf("Inventori berhasil diurutkan.\n");
    printItemTable(inventory->data, inventory->size);
}

static void searchBySequential(const Inventory *inventory) {
    char name[MAX_NAME];
    printf("Nama item: ");
    readLine(name, sizeof(name));
    int index = sequentialSearchByName(inventory->data, inventory->size, name);
    if (index == NOT_FOUND) {
        printf("Item '%s' tidak ditemukan di inventori.\n", name);
    } else {
        printf("Ditemukan di posisi ke-%d:\n", index + 1);
        printItemTable(&inventory->data[index], 1);
    }
}

static void searchByBinary(Inventory *inventory) {
    if (!isSortedByIdAscending(inventory->data, inventory->size)) {
        printf("Binary search butuh data terurut. Mengurutkan berdasarkan ID...\n");
        if (!mergeSort(inventory->data, inventory->size, SORT_BY_ID, ORDER_ASCENDING)) {
            printf("Gagal mengurutkan (memori habis).\n");
            return;
        }
    }
    int id = readInt("ID item: ", 1, MAX_ITEM_ID);
    int index = binarySearchById(inventory->data, inventory->size, id);
    if (index == NOT_FOUND) {
        printf("Item dengan ID %d tidak ditemukan di inventori.\n", id);
    } else {
        printf("Ditemukan di indeks %d:\n", index);
        printItemTable(&inventory->data[index], 1);
    }
}

static void searchInventory(Inventory *inventory) {
    if (inventory->size == 0) {
        printf("Inventori kosong.\n");
        return;
    }
    printf("1. Sequential search (nama)  2. Binary search (ID)\n");
    int method = readInt("Pilih metode: ", SEARCH_SEQUENTIAL, SEARCH_BINARY);
    if (method == SEARCH_SEQUENTIAL) {
        searchBySequential(inventory);
    } else {
        searchByBinary(inventory);
    }
}

static void printCurrentRoom(const Player *player, const Graph *dungeon) {
    printf("Kamu berada di: %s\n", dungeon->names[player->currentRoom]);
}

static void exploreDungeon(Player *player, const Graph *dungeon, Stack *history) {
    int neighbors[MAX_ROOMS];
    int count = getNeighbors(dungeon, player->currentRoom, neighbors);

    printCurrentRoom(player, dungeon);
    if (count == 0) {
        printf("Tidak ada pintu keluar dari ruangan ini.\n");
        return;
    }
    printf("Pintu yang tersedia:\n");
    for (int i = 0; i < count; i++) {
        printf("  %d. %s\n", i + 1, dungeon->names[neighbors[i]]);
    }
    int choice = readInt("Pilih pintu (0 = batal): ", 0, count);
    if (choice == 0) {
        return;
    }
    if (!push(history, player->currentRoom)) {
        printf("Gagal mencatat riwayat (memori habis).\n");
        return;
    }
    player->currentRoom = neighbors[choice - 1];
    printf("Kamu masuk ke %s.\n", dungeon->names[player->currentRoom]);
    if (player->currentRoom == getBossRoom(dungeon)) {
        printf("Kamu tiba di ruang BOSS! (battle hadir di modul berikutnya)\n");
    }
}

static void undoMove(Player *player, const Graph *dungeon, Stack *history) {
    int previousRoom;
    if (!pop(history, &previousRoom)) {
        printf("Tidak ada langkah yang bisa di-undo.\n");
        return;
    }
    player->currentRoom = previousRoom;
    printf("Undo berhasil. Kamu kembali ke %s. (sisa riwayat: %d langkah)\n",
           dungeon->names[previousRoom], history->size);
}

static void showShortestPathToBoss(const Player *player, const Graph *dungeon) {
    int path[MAX_ROOMS];
    int length = 0;
    int boss = getBossRoom(dungeon);

    if (!bfsShortestPath(dungeon, player->currentRoom, boss, path, &length)) {
        printf("Tidak ada jalur menuju ruang boss.\n");
        return;
    }
    if (length == 1) {
        printf("Kamu sudah berada di ruang boss!\n");
        return;
    }
    printf("Jalur terpendek ke %s (%d langkah):\n", dungeon->names[boss], length - 1);
    for (int i = 0; i < length; i++) {
        printf("%s%s", dungeon->names[path[i]], (i < length - 1) ? " -> " : "\n");
    }
}

static void showDungeonMap(const Player *player, const Graph *dungeon) {
    printf("1. Tampilkan peta  2. Jalur terpendek ke boss (BFS)  3. Eksplorasi (DFS)\n");
    int choice = readInt("Pilih: ", MAP_SHOW, MAP_DFS);
    switch (choice) {
        case MAP_SHOW: printMap(dungeon); break;
        case MAP_BFS:  showShortestPathToBoss(player, dungeon); break;
        case MAP_DFS:  dfsExplore(dungeon, player->currentRoom); break;
    }
}

static void searchCatalogByHash(const HashTable *table) {
    printf("1. Cari item by ID  2. Tampilkan isi hash table\n");
    int choice = readInt("Pilih: ", HASH_LOOKUP, HASH_SHOW);
    if (choice == HASH_SHOW) {
        printHashTable(table);
        return;
    }
    int id = readInt("ID item: ", 1, MAX_ITEM_ID);
    int bucket, steps;
    const Item *found = hashSearch(table, id, &bucket, &steps);
    if (found == NULL) {
        printf("Item dengan ID %d tidak ditemukan (bucket %d, %d langkah).\n", id, bucket, steps);
        return;
    }
    printf("Ditemukan di bucket %d setelah %d langkah.\n", bucket, steps);
    printItemTable(found, 1);
}

static int getAttackPower(const Inventory *inventory) {
    int strongest = 0;
    for (int i = 0; i < inventory->size; i++) {
        if (inventory->data[i].power > strongest) {
            strongest = inventory->data[i].power;
        }
    }
    return PLAYER_BASE_ATTACK + strongest;
}

static void recordInBestiary(BstNode **bestiary, const Monster *monster) {
    BestiaryEntry entry;
    entry.id = monster->id;
    snprintf(entry.name, sizeof(entry.name), "%s", monster->name);
    entry.hp = monster->hp;
    entry.attack = monster->attack;
    entry.timesDefeated = 1;
    *bestiary = bstInsert(*bestiary, &entry);
}

static void handleVictory(Player *player, const Monster *monster, BstNode **bestiary) {
    player->score += monster->reward;
    healPlayer(player, HEAL_AFTER_WIN);
    printf("Menang! +%d skor (total %d). HP pulih sedikit.\n", monster->reward, player->score);
    if (updateLevel(player)) {
        printf("LEVEL UP! Sekarang level %d (HP maksimum %d).\n", player->level, getMaxHp(player));
    }
    recordInBestiary(bestiary, monster);
    printf("%s dicatat di bestiary.\n", monster->name);
}

static void handleDefeat(Player *player, Stack *history) {
    player->hp = getMaxHp(player);
    player->currentRoom = START_ROOM;
    freeStack(history);
    initStack(history);
    printf("Kamu terbangun di Gerbang Masuk dengan HP pulih. Riwayat langkah dihapus.\n");
}

static void startBattle(Player *player, const MonsterCatalog *monsters,
                        const Inventory *inventory, BstNode **bestiary, Stack *history) {
    printMonsterTable(monsters);
    int choice = readInt("Pilih monster (0 = batal): ", 0, monsters->count);
    if (choice == 0) {
        return;
    }
    const Monster *monster = &monsters->list[choice - 1];
    BattleResult result = runBattle(player, monster, getAttackPower(inventory));

    if (result == BATTLE_WON) {
        handleVictory(player, monster, bestiary);
    } else if (result == BATTLE_LOST) {
        handleDefeat(player, history);
    }
}

static void searchBestiary(BstNode *bestiary) {
    int id = readInt("ID monster: ", 1, MAX_ITEM_ID);
    BstNode *found = bstSearch(bestiary, id);
    if (found == NULL) {
        printf("Monster dengan ID %d belum ada di bestiary.\n", id);
        return;
    }
    printBestiaryHeader();
    printBestiaryEntry(&found->entry);
}

static void deleteFromBestiary(BstNode **bestiary) {
    int id = readInt("ID monster yang dihapus: ", 1, MAX_ITEM_ID);
    int deleted = 0;
    *bestiary = bstDelete(*bestiary, id, &deleted);
    if (deleted) {
        printf("Monster ID %d dihapus dari bestiary.\n", id);
    } else {
        printf("Monster dengan ID %d tidak ada di bestiary.\n", id);
    }
}

static void bestiaryMenu(BstNode **bestiary) {
    printf("Bestiary berisi %d monster.\n", bstCount(*bestiary));
    printf("1. Pre-order  2. In-order  3. Post-order  4. Cari  5. Hapus\n");
    int choice = readInt("Pilih: ", BESTIARY_PREORDER, BESTIARY_DELETE);

    if (choice == BESTIARY_SEARCH) {
        searchBestiary(*bestiary);
        return;
    }
    if (choice == BESTIARY_DELETE) {
        deleteFromBestiary(bestiary);
        return;
    }
    if (*bestiary == NULL) {
        printf("Bestiary masih kosong. Kalahkan monster dulu!\n");
        return;
    }
    printBestiaryHeader();
    if (choice == BESTIARY_PREORDER) {
        bstPreorder(*bestiary);
    } else if (choice == BESTIARY_INORDER) {
        bstInorder(*bestiary);
    } else {
        bstPostorder(*bestiary);
    }
}

void runGame(Player *player, const GameData *data) {
    const Inventory *catalog = data->catalog;
    const Graph *dungeon = data->dungeon;
    Inventory inventory;
    Stack history;
    BstNode *bestiary;
    char path[PATH_SIZE];
    char bestiaryPath[PATH_SIZE];

    initStack(&history);
    if (player->currentRoom < 0 || player->currentRoom >= dungeon->count) {
        player->currentRoom = START_ROOM;
    }

    initInventory(&inventory);
    buildInventoryPath(path, sizeof(path), player->username);
    loadItemsFromFile(path, &inventory);   /* file belum ada = inventori baru */
    snprintf(bestiaryPath, sizeof(bestiaryPath), "data/bestiary_%s.txt", player->username);
    bestiary = loadBestiary(bestiaryPath);

    int choice;
    do {
        system("cls");
        getchar();
        showGameMenu();
        choice = readInt("Pilih: ", 0, MENU_MAX);
        switch (choice) {
            case MENU_STATUS:
                printPlayer(player);
                printCurrentRoom(player, dungeon);
                break;
            case MENU_EXPLORE:   exploreDungeon(player, dungeon, &history); break;
            case MENU_UNDO:      undoMove(player, dungeon, &history); break;
            case MENU_MAP:       showDungeonMap(player, dungeon); break;
            case MENU_HASH:      searchCatalogByHash(data->itemIndex); break;
            case MENU_BATTLE:
                startBattle(player, data->monsters, &inventory, &bestiary, &history);
                break;
            case MENU_BESTIARY:  bestiaryMenu(&bestiary); break;
            case MENU_TAKE_ITEM: takeItem(&inventory, catalog); break;
            case MENU_INVENTORY: printItemTable(inventory.data, inventory.size); break;
            case MENU_SORT:      sortInventory(&inventory); break;
            case MENU_SEARCH:    searchInventory(&inventory); break;
            default: break;
        }
    } while (choice != MENU_LOGOUT);

    if (savePlayer(player) && saveItemsToFile(path, &inventory)
        && saveBestiary(bestiaryPath, bestiary)) {
        printf("Progres dan inventori tersimpan. Logout berhasil.\n");
    } else {
        printf("Peringatan: progres gagal disimpan.\n");
    }
    freeInventory(&inventory);
    freeStack(&history);
    bstFree(bestiary);
}
