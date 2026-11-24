#include <stdio.h>
#include <string.h>
#include "game.h"
#include "auth.h"
#include "sorting.h"
#include "searching.h"

#define MENU_LOGOUT      0
#define MENU_STATUS      1
#define MENU_TAKE_ITEM   2
#define MENU_INVENTORY   3
#define MENU_SORT        4
#define MENU_SEARCH      5
#define MENU_MAX         5

#define ALGO_BUBBLE 1
#define ALGO_MERGE  2

#define SEARCH_SEQUENTIAL 1
#define SEARCH_BINARY     2

#define MAX_ITEM_ID 9999

static void showGameMenu(void) {
    printf("\n--- Menu Game ---\n");
    printf("1. Lihat status pemain\n");
    printf("2. Ambil item dari katalog\n");
    printf("3. Lihat inventori\n");
    printf("4. Urutkan inventori (sorting)\n");
    printf("5. Cari item di inventori (searching)\n");
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

void runGame(Player *player, const Inventory *catalog) {
    Inventory inventory;
    char path[PATH_SIZE];

    initInventory(&inventory);
    buildInventoryPath(path, sizeof(path), player->username);
    loadItemsFromFile(path, &inventory);   /* file belum ada = inventori baru */

    int choice;
    do {
        showGameMenu();
        choice = readInt("Pilih: ", 0, MENU_MAX);
        switch (choice) {
            case MENU_STATUS:    printPlayer(player); break;
            case MENU_TAKE_ITEM: takeItem(&inventory, catalog); break;
            case MENU_INVENTORY: printItemTable(inventory.data, inventory.size); break;
            case MENU_SORT:      sortInventory(&inventory); break;
            case MENU_SEARCH:    searchInventory(&inventory); break;
            default: break;
        }
    } while (choice != MENU_LOGOUT);

    if (savePlayer(player) && saveItemsToFile(path, &inventory)) {
        printf("Progres dan inventori tersimpan. Logout berhasil.\n");
    } else {
        printf("Peringatan: progres gagal disimpan.\n");
    }
    freeInventory(&inventory);
}
