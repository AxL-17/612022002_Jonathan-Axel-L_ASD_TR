#include <stdio.h>
#include "monster.h"

#define MONSTER_FORMAT "%d;%49[^;];%d;%d;%d"
#define MONSTER_FIELDS 5

int loadMonsters(const char *path, MonsterCatalog *catalog) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        return 0;
    }
    catalog->count = 0;
    char line[MAX_LINE];
    while (catalog->count < MAX_MONSTERS && fgets(line, sizeof(line), f) != NULL) {
        Monster *m = &catalog->list[catalog->count];
        if (sscanf(line, MONSTER_FORMAT, &m->id, m->name, &m->hp,
                   &m->attack, &m->reward) == MONSTER_FIELDS) {
            catalog->count++;
        }
    }
    fclose(f);
    return catalog->count > 0;
}

void printMonsterTable(const MonsterCatalog *catalog) {
    printf("\n%-4s %-5s %-16s %-6s %-8s %-6s\n", "No", "ID", "Nama", "HP", "Serangan", "Skor");
    printf("---------------------------------------------------\n");
    for (int i = 0; i < catalog->count; i++) {
        const Monster *m = &catalog->list[i];
        printf("%-4d %-5d %-16s %-6d %-8d %-6d\n", i + 1, m->id, m->name,
               m->hp, m->attack, m->reward);
    }
}
