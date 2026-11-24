#ifndef SEARCHING_H
#define SEARCHING_H

#include "item.h"

#define NOT_FOUND (-1)

/* Sequential search berdasarkan nama (tidak membedakan huruf besar/kecil). */
int sequentialSearchByName(const Item *items, int count, const char *name);

/* Binary search berdasarkan ID. Data HARUS sudah terurut ascending by ID. */
int binarySearchById(const Item *items, int count, int id);

int isSortedByIdAscending(const Item *items, int count);

#endif
