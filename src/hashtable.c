#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"

#define HASH_MULTIPLIER 31
#define HASH_OFFSET     7

/* Fungsi hash buatan sendiri: acak sedikit lalu ambil sisa bagi ukuran tabel. */
unsigned int hashFunction(int id) {
    unsigned int key = (unsigned int)id;
    return (key * HASH_MULTIPLIER + HASH_OFFSET) % HASH_SIZE;
}

void initHashTable(HashTable *table) {
    for (int i = 0; i < HASH_SIZE; i++) {
        table->buckets[i] = NULL;
    }
    table->count = 0;
    table->collisions = 0;
}

int hashInsert(HashTable *table, const Item *item) {
    int bucket;
    int steps;
    if (hashSearch(table, item->id, &bucket, &steps) != NULL) {
        return 0;   /* kunci harus unik */
    }
    HashNode *node = malloc(sizeof(HashNode));
    if (node == NULL) {
        return 0;
    }
    unsigned int index = hashFunction(item->id);
    node->item = *item;
    if (table->buckets[index] != NULL) {
        table->collisions++;   /* bucket sudah terisi: terjadi collision */
    }
    node->next = table->buckets[index];   /* sisip di depan chain */
    table->buckets[index] = node;
    table->count++;
    return 1;
}

const Item *hashSearch(const HashTable *table, int id, int *bucket, int *steps) {
    unsigned int index = hashFunction(id);
    *bucket = (int)index;
    *steps = 0;
    for (HashNode *node = table->buckets[index]; node != NULL; node = node->next) {
        (*steps)++;
        if (node->item.id == id) {
            return &node->item;
        }
    }
    return NULL;
}

void printHashTable(const HashTable *table) {
    printf("\n=== Hash Table (ukuran %d, %d item, %d collision) ===\n",
           HASH_SIZE, table->count, table->collisions);
    for (int i = 0; i < HASH_SIZE; i++) {
        printf("[%d] ", i);
        for (HashNode *node = table->buckets[i]; node != NULL; node = node->next) {
            printf("%d(%s)%s", node->item.id, node->item.name, node->next ? " -> " : "");
        }
        printf("\n");
    }
}

void freeHashTable(HashTable *table) {
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode *node = table->buckets[i];
        while (node != NULL) {
            HashNode *next = node->next;
            free(node);
            node = next;
        }
        table->buckets[i] = NULL;
    }
    table->count = 0;
    table->collisions = 0;
}
