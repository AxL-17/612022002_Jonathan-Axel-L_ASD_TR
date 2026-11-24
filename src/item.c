#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "item.h"

#define ITEM_FORMAT  "%d;%49[^;];%d;%d"
#define ITEM_FIELDS  4
#define GROWTH_RATE  2

void initInventory(Inventory *inv) {
    inv->data = malloc(INITIAL_CAPACITY * sizeof(Item));
    inv->size = 0;
    inv->capacity = (inv->data != NULL) ? INITIAL_CAPACITY : 0;
}

int addItem(Inventory *inv, const Item *item) {
    if (inv->size == inv->capacity) {
        int newCapacity = (inv->capacity == 0) ? INITIAL_CAPACITY
                                               : inv->capacity * GROWTH_RATE;
        Item *bigger = realloc(inv->data, newCapacity * sizeof(Item));
        if (bigger == NULL) {
            return 0;
        }
        inv->data = bigger;
        inv->capacity = newCapacity;
    }
    inv->data[inv->size++] = *item;
    return 1;
}

void freeInventory(Inventory *inv) {
    free(inv->data);
    inv->data = NULL;
    inv->size = 0;
    inv->capacity = 0;
}

int loadItemsFromFile(const char *path, Inventory *inv) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        return 0;
    }
    char line[MAX_LINE];
    Item item;
    while (fgets(line, sizeof(line), f) != NULL) {
        if (sscanf(line, ITEM_FORMAT, &item.id, item.name,
                   &item.power, &item.price) == ITEM_FIELDS) {
            addItem(inv, &item);
        }
    }
    fclose(f);
    return 1;
}

int saveItemsToFile(const char *path, const Inventory *inv) {
    FILE *f = fopen(path, "w");
    if (f == NULL) {
        return 0;
    }
    for (int i = 0; i < inv->size; i++) {
        const Item *it = &inv->data[i];
        fprintf(f, "%d;%s;%d;%d\n", it->id, it->name, it->power, it->price);
    }
    fclose(f);
    return 1;
}

void buildInventoryPath(char *out, int size, const char *username) {
    snprintf(out, size, "data/inventory_%s.txt", username);
}

void printItemTable(const Item *items, int count) {
    printf("\n%-5s %-22s %-8s %-8s\n", "ID", "Nama", "Power", "Harga");
    printf("-------------------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%-5d %-22s %-8d %-8d\n", items[i].id, items[i].name,
               items[i].power, items[i].price);
    }
    if (count == 0) {
        printf("(kosong)\n");
    }
}
