#ifndef GRAPH_H
#define GRAPH_H

#include "utils.h"

#define MAX_ROOMS     20
#define DUNGEON_FILE  "data/dungeon.txt"
#define EDGES_MARKER  "EDGES"

typedef struct EdgeNode {
    int dest;
    struct EdgeNode *next;
} EdgeNode;

/* Graph tak berarah, disimpan sebagai adjacency list. */
typedef struct {
    int count;
    char names[MAX_ROOMS][MAX_NAME];
    EdgeNode *adj[MAX_ROOMS];
} Graph;

void initGraph(Graph *g);
int  loadGraph(Graph *g, const char *path);
void freeGraph(Graph *g);

int  addEdge(Graph *g, int a, int b);
int  getNeighbors(const Graph *g, int room, int *neighbors);   /* return jumlah */
int  getBossRoom(const Graph *g);                              /* ruangan terakhir */
void printMap(const Graph *g);

/* BFS: jalur terpendek start -> goal. Return 1 jika ada jalur. */
int  bfsShortestPath(const Graph *g, int start, int goal, int *path, int *length);

/* DFS: mencetak urutan ruangan yang dapat dijangkau dari start. */
void dfsExplore(const Graph *g, int start);

#endif
