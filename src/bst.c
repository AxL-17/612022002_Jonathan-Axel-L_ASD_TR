#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

#define BESTIARY_FIELDS 5

static BstNode *createNode(const BestiaryEntry *entry) {
    BstNode *node = malloc(sizeof(BstNode));
    if (node == NULL) {
        return NULL;
    }
    node->entry = *entry;
    node->left = NULL;
    node->right = NULL;
    return node;
}

BstNode *bstInsert(BstNode *root, const BestiaryEntry *entry) {
    if (root == NULL) {
        return createNode(entry);
    }
    if (entry->id < root->entry.id) {
        BstNode *child = bstInsert(root->left, entry);
        if (child != NULL) root->left = child;
    } else if (entry->id > root->entry.id) {
        BstNode *child = bstInsert(root->right, entry);
        if (child != NULL) root->right = child;
    } else {
        root->entry.timesDefeated += entry->timesDefeated;
    }
    return root;
}

BstNode *bstSearch(BstNode *root, int id) {
    while (root != NULL && root->entry.id != id) {
        root = (id < root->entry.id) ? root->left : root->right;
    }
    return root;
}

static BstNode *findMinimum(BstNode *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

BstNode *bstDelete(BstNode *root, int id, int *deleted) {
    if (root == NULL) {
        return NULL;
    }
    if (id < root->entry.id) {
        root->left = bstDelete(root->left, id, deleted);
    } else if (id > root->entry.id) {
        root->right = bstDelete(root->right, id, deleted);
    } else {
        *deleted = 1;
        if (root->left == NULL) {            /* tanpa anak / hanya anak kanan */
            BstNode *child = root->right;
            free(root);
            return child;
        }
        if (root->right == NULL) {           /* hanya anak kiri */
            BstNode *child = root->left;
            free(root);
            return child;
        }
        /* dua anak: ganti dengan successor (terkecil di subtree kanan) */
        BstNode *successor = findMinimum(root->right);
        root->entry = successor->entry;
        int removed = 0;
        root->right = bstDelete(root->right, successor->entry.id, &removed);
    }
    return root;
}

void printBestiaryHeader(void) {
    printf("\n%-5s %-16s %-6s %-8s %-8s\n", "ID", "Nama", "HP", "Serangan", "Kalah(x)");
    printf("--------------------------------------------------\n");
}

void printBestiaryEntry(const BestiaryEntry *e) {
    printf("%-5d %-16s %-6d %-8d %-8d\n", e->id, e->name, e->hp, e->attack, e->timesDefeated);
}

void bstPreorder(const BstNode *root) {
    if (root == NULL) return;
    printBestiaryEntry(&root->entry);
    bstPreorder(root->left);
    bstPreorder(root->right);
}

void bstInorder(const BstNode *root) {
    if (root == NULL) return;
    bstInorder(root->left);
    printBestiaryEntry(&root->entry);
    bstInorder(root->right);
}

void bstPostorder(const BstNode *root) {
    if (root == NULL) return;
    bstPostorder(root->left);
    bstPostorder(root->right);
    printBestiaryEntry(&root->entry);
}

int bstCount(const BstNode *root) {
    if (root == NULL) return 0;
    return 1 + bstCount(root->left) + bstCount(root->right);
}

void bstFree(BstNode *root) {
    if (root == NULL) return;
    bstFree(root->left);
    bstFree(root->right);
    free(root);
}

static void writePreorder(FILE *f, const BstNode *root) {
    if (root == NULL) return;
    const BestiaryEntry *e = &root->entry;
    fprintf(f, "%d;%s;%d;%d;%d\n", e->id, e->name, e->hp, e->attack, e->timesDefeated);
    writePreorder(f, root->left);
    writePreorder(f, root->right);
}

/* Disimpan pre-order agar bentuk pohon sama persis saat dimuat ulang. */
int saveBestiary(const char *path, const BstNode *root) {
    FILE *f = fopen(path, "w");
    if (f == NULL) {
        return 0;
    }
    writePreorder(f, root);
    fclose(f);
    return 1;
}

BstNode *loadBestiary(const char *path) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        return NULL;
    }
    BstNode *root = NULL;
    char line[MAX_LINE];
    BestiaryEntry e;
    while (fgets(line, sizeof(line), f) != NULL) {
        if (sscanf(line, BESTIARY_FILE_FORMAT, &e.id, e.name, &e.hp, &e.attack,
                   &e.timesDefeated) == BESTIARY_FIELDS) {
            root = bstInsert(root, &e);
        }
    }
    fclose(f);
    return root;
}
