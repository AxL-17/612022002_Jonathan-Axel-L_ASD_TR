#ifndef AUTH_H
#define AUTH_H

#include "player.h"

#define USERS_FILE "data/users.txt"
#define USERS_TEMP "data/users.tmp"

/* Return 1 jika register berhasil, 0 jika gagal. */
int registerUser(void);

/* Return 1 jika login berhasil dan mengisi *out, 0 jika gagal. */
int loginUser(Player *out);

/* Menyimpan progres pemain ke users.txt. Return 1 jika berhasil. */
int savePlayer(const Player *p);

#endif
