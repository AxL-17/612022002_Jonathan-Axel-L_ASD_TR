#ifndef SORTING_H
#define SORTING_H

#include "item.h"

typedef enum {
    SORT_BY_ID = 1,
    SORT_BY_NAME,
    SORT_BY_POWER,
    SORT_BY_PRICE
} SortField;

#define ORDER_ASCENDING  1
#define ORDER_DESCENDING 0

int compareItems(const Item *a, const Item *b, SortField field);
void bubbleSort(Item *items, int count, SortField field, int ascending);
int  mergeSort(Item *items, int count, SortField field, int ascending); /* 0 jika memori gagal */

#endif
