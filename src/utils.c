#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define INT_BUFFER_SIZE 32

int readLine(char *buffer, int size) {
    if (fgets(buffer, size, stdin) == NULL) {
        return 0;
    }
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    } else {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }
    return 1;
}

int readInt(const char *prompt, int min, int max) {
    char buffer[INT_BUFFER_SIZE];
    char *end;

    while (1) {
        printf("%s", prompt);
        if (!readLine(buffer, sizeof(buffer))) {
            return min;
        }
        long value = strtol(buffer, &end, 10);
        if (end == buffer || *end != '\0' || value < min || value > max) {
            printf("Input tidak valid. Masukkan angka %d-%d.\n", min, max);
            continue;
        }
        return (int)value;
    }
}

void pauseScreen(void) {
    char tmp[8];
    printf("Tekan Enter untuk lanjut...");
    readLine(tmp, sizeof(tmp));
}
