#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "item.h"

#define HASH_SIZE 7   /* sengaja kecil (bilangan prima) agar collision terlihat */

typedef struct HashNode {
    Item item;
    struct HashNode *next;
} HashNode;

/* Hash table dengan separate chaining; kunci = ID item. */
typedef struct {
    HashNode *buckets[HASH_SIZE];
    int count;
    int collisions;
} HashTable;

unsigned int hashFunction(int id);

void initHashTable(HashTable *table);
int  hashInsert(HashTable *table, const Item *item);   /* 0 jika ID ganda / memori gagal */
const Item *hashSearch(const HashTable *table, int id, int *bucket, int *steps);
void printHashTable(const HashTable *table);
void freeHashTable(HashTable *table);

#endif
