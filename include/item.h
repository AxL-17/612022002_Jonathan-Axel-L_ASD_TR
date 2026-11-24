#ifndef ITEM_H
#define ITEM_H

#include "utils.h"

#define INITIAL_CAPACITY 5
#define CATALOG_FILE     "data/items.txt"
#define PATH_SIZE        128

typedef struct {
    int  id;
    char name[MAX_NAME];
    int  power;
    int  price;
} Item;

/* Array dinamis untuk inventori pemain maupun katalog item. */
typedef struct {
    Item *data;
    int   size;
    int   capacity;
} Inventory;

void initInventory(Inventory *inv);
int  addItem(Inventory *inv, const Item *item);   /* 1 = berhasil */
void freeInventory(Inventory *inv);

int  loadItemsFromFile(const char *path, Inventory *inv);
int  saveItemsToFile(const char *path, const Inventory *inv);
void buildInventoryPath(char *out, int size, const char *username);

void printItemTable(const Item *items, int count);

#endif
