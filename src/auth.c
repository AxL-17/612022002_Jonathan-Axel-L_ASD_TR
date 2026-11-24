#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "auth.h"

#define FIELD_SEPARATOR ';'
#define PARSE_FORMAT    "%49[^;];%49[^;];%d;%d;%d;%d"
#define PARSE_FIELDS    6

static int parseLine(const char *line, Player *p) {
    return sscanf(line, PARSE_FORMAT, p->username, p->password,
                  &p->hp, &p->level, &p->score, &p->currentRoom) == PARSE_FIELDS;
}

static void writePlayer(FILE *f, const Player *p) {
    fprintf(f, "%s;%s;%d;%d;%d;%d\n", p->username, p->password,
            p->hp, p->level, p->score, p->currentRoom);
}

/* Mencari user di file. Return 1 jika ketemu dan mengisi *found (boleh NULL). */
static int findUser(const char *username, Player *found) {
    FILE *f = fopen(USERS_FILE, "r");
    if (f == NULL) {
        return 0;
    }
    char line[MAX_LINE];
    Player temp;
    int exists = 0;

    while (fgets(line, sizeof(line), f) != NULL) {
        if (parseLine(line, &temp) && strcmp(temp.username, username) == 0) {
            if (found != NULL) {
                *found = temp;
            }
            exists = 1;
            break;
        }
    }
    fclose(f);
    return exists;
}

static int isValidInput(const char *text) {
    return text[0] != '\0' && strchr(text, FIELD_SEPARATOR) == NULL;
}

static int isValidUsername(const char *name) {
    if (name[0] == '\0') {
        return 0;
    }
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isalnum((unsigned char)name[i]) && name[i] != '_') {
            return 0;
        }
    }
    return 1;
}

static void askCredentials(char *username, char *password) {
    printf("Username: ");
    readLine(username, MAX_NAME);
    printf("Password: ");
    readLine(password, MAX_PASS);
}

int registerUser(void) {
    char username[MAX_NAME], password[MAX_PASS];
    askCredentials(username, password);

    if (!isValidUsername(username)) {
        printf("Username tidak boleh kosong dan hanya boleh huruf, angka, atau '_'.\n");
        return 0;
    }
    if (!isValidInput(password)) {
        printf("Password tidak boleh kosong atau mengandung ';'.\n");
        return 0;
    }
    if (findUser(username, NULL)) {
        printf("Username sudah digunakan.\n");
        return 0;
    }

    Player p;
    initPlayer(&p, username, password);

    FILE *f = fopen(USERS_FILE, "a");
    if (f == NULL) {
        printf("Gagal membuka %s. Jalankan program dari folder utama proyek.\n", USERS_FILE);
        return 0;
    }
    writePlayer(f, &p);
    fclose(f);
    printf("Register berhasil. Silakan login.\n");
    return 1;
}

int loginUser(Player *out) {
    char username[MAX_NAME], password[MAX_PASS];
    askCredentials(username, password);

    if (username[0] == '\0' || password[0] == '\0') {
        printf("Username dan password tidak boleh kosong.\n");
        return 0;
    }

    Player found;
    if (!findUser(username, &found) || strcmp(found.password, password) != 0) {
        printf("Login gagal: username atau password salah.\n");
        return 0;
    }
    *out = found;
    printf("Login berhasil. Selamat datang, %s!\n", out->username);
    return 1;
}

int savePlayer(const Player *p) {
    FILE *in = fopen(USERS_FILE, "r");
    FILE *out = fopen(USERS_TEMP, "w");
    if (in == NULL || out == NULL) {
        if (in) fclose(in);
        if (out) fclose(out);
        return 0;
    }

    char line[MAX_LINE];
    Player temp;
    while (fgets(line, sizeof(line), in) != NULL) {
        if (parseLine(line, &temp) && strcmp(temp.username, p->username) == 0) {
            writePlayer(out, p);
        } else {
            fputs(line, out);
        }
    }
    fclose(in);
    fclose(out);

    remove(USERS_FILE);
    return rename(USERS_TEMP, USERS_FILE) == 0;
}
