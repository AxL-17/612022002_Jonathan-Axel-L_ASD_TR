#ifndef BST_H
#define BST_H

#include "utils.h"

#define BESTIARY_FILE_FORMAT "%d;%49[^;];%d;%d;%d"

/* Catatan satu monster di bestiary pemain. Kunci BST = id. */
typedef struct {
    int  id;
    char name[MAX_NAME];
    int  hp;
    int  attack;
    int  timesDefeated;
} BestiaryEntry;

typedef struct BstNode {
    BestiaryEntry entry;
    struct BstNode *left;
    struct BstNode *right;
} BstNode;

/* Insert: jika id sudah ada, timesDefeated ditambah (tidak ada duplikat). */
BstNode *bstInsert(BstNode *root, const BestiaryEntry *entry);
BstNode *bstSearch(BstNode *root, int id);
BstNode *bstDelete(BstNode *root, int id, int *deleted);

void printBestiaryHeader(void);
void printBestiaryEntry(const BestiaryEntry *entry);
void bstPreorder(const BstNode *root);
void bstInorder(const BstNode *root);
void bstPostorder(const BstNode *root);

int  bstCount(const BstNode *root);
void bstFree(BstNode *root);

int      saveBestiary(const char *path, const BstNode *root);
BstNode *loadBestiary(const char *path);

#endif
