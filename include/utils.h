#ifndef UTILS_H
#define UTILS_H

#define MAX_NAME 50
#define MAX_PASS 50
#define MAX_LINE 128

/* Membaca satu baris dengan aman (tanpa newline). Return 1 jika berhasil. */
int  readLine(char *buffer, int size);

/* Meminta angka dalam rentang [min, max]; mengulang sampai input valid. */
int  readInt(const char *prompt, int min, int max);

void pauseScreen(void);

#endif
