#include <stdlib.h>
#include <string.h>
#include "sorting.h"

int compareItems(const Item *a, const Item *b, SortField field) {
    switch (field) {
        case SORT_BY_ID:    return a->id - b->id;
        case SORT_BY_NAME:  return strcmp(a->name, b->name);
        case SORT_BY_POWER: return a->power - b->power;
        case SORT_BY_PRICE: return a->price - b->price;
    }
    return 0;
}

/* Hasil > 0 berarti a harus berada SETELAH b pada urutan yang diminta. */
static int compareOrdered(const Item *a, const Item *b, SortField field, int ascending) {
    int result = compareItems(a, b, field);
    return ascending ? result : -result;
}

void bubbleSort(Item *items, int count, SortField field, int ascending) {
    for (int pass = 0; pass < count - 1; pass++) {
        int swapped = 0;
        for (int j = 0; j < count - 1 - pass; j++) {
            if (compareOrdered(&items[j], &items[j + 1], field, ascending) > 0) {
                Item temp = items[j];
                items[j] = items[j + 1];
                items[j + 1] = temp;
                swapped = 1;
            }
        }
        if (!swapped) {
            break;
        }
    }
}

static void merge(Item *items, Item *buffer, int left, int mid, int right,
                  SortField field, int ascending) {
    int i = left, j = mid + 1, k = left;

    while (i <= mid && j <= right) {
        if (compareOrdered(&items[i], &items[j], field, ascending) <= 0) {
            buffer[k++] = items[i++];
        } else {
            buffer[k++] = items[j++];
        }
    }
    while (i <= mid)   buffer[k++] = items[i++];
    while (j <= right) buffer[k++] = items[j++];

    for (k = left; k <= right; k++) {
        items[k] = buffer[k];
    }
}

static void mergeSortRange(Item *items, Item *buffer, int left, int right,
                           SortField field, int ascending) {
    if (left >= right) {
        return;
    }
    int mid = left + (right - left) / 2;
    mergeSortRange(items, buffer, left, mid, field, ascending);
    mergeSortRange(items, buffer, mid + 1, right, field, ascending);
    merge(items, buffer, left, mid, right, field, ascending);
}

int mergeSort(Item *items, int count, SortField field, int ascending) {
    if (count < 2) {
        return 1;
    }
    Item *buffer = malloc(count * sizeof(Item));
    if (buffer == NULL) {
        return 0;
    }
    mergeSortRange(items, buffer, 0, count - 1, field, ascending);
    free(buffer);
    return 1;
}
