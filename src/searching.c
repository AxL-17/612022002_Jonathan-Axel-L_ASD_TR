#include <ctype.h>
#include "searching.h"

static int equalsIgnoreCase(const char *a, const char *b) {
    while (*a != '\0' && *b != '\0') {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) {
            return 0;
        }
        a++;
        b++;
    }
    return *a == *b;
}

int sequentialSearchByName(const Item *items, int count, const char *name) {
    for (int i = 0; i < count; i++) {
        if (equalsIgnoreCase(items[i].name, name)) {
            return i;
        }
    }
    return NOT_FOUND;
}

int binarySearchById(const Item *items, int count, int id) {
    int low = 0, high = count - 1;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (items[mid].id == id) {
            return mid;
        }
        if (items[mid].id < id) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return NOT_FOUND;
}

int isSortedByIdAscending(const Item *items, int count) {
    for (int i = 1; i < count; i++) {
        if (items[i - 1].id > items[i].id) {
            return 0;
        }
    }
    return 1;
}
