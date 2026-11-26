# Stack Overflow Dungeon

- Nama: Jonathan Axel L
- NIM: 612022002

## Deskripsi
Game RPG konsol berbasis dungeon, dibuat 100% dengan Bahasa C. Pemain
mendaftar, login, menjelajahi dungeon, mengumpulkan item, melawan monster,
dan mencatat monster yang dikalahkan di bestiary. Progres tersimpan permanen.

## Fitur
- Register, login, logout (data akun di file .txt, username unik)
- Jelajah dungeon dan undo langkah (stack)
- Peta dungeon: jalur terpendek ke boss (BFS) dan eksplorasi (DFS) pada graph
- Inventori dinamis (malloc/realloc/free), simpan dan muat dari file
- Sorting inventori: bubble sort dan merge sort, ascending/descending
- Searching: sequential search (nama) dan binary search (ID)
- Katalog item dengan hash table (chaining) untuk pencarian by ID
- Battle bergiliran dengan queue sebagai turn order
- Bestiary berbasis Binary Search Tree: insert, search, delete, pre/in/post-order

## Struktur Folder
```
include/  header (.h)
src/      sumber (.c), termasuk main.c
data/     database .txt (akun, katalog item, monster, dungeon)
docs/     laporan TR (PDF)
```

## Kompilasi
```
gcc src/*.c -Iinclude -o app
```

## Menjalankan
Jalankan dari folder utama proyek (bukan dari dalam `src/`):
```
./app
```
(Windows PowerShell: `./app` atau `app.exe`)