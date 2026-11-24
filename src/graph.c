#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "queue.h"

#define ROOM_FORMAT "%d;%49[^\n]"
#define EDGE_FORMAT "%d;%d"
#define NO_PARENT   (-1)

void initGraph(Graph *g) {
    g->count = 0;
    for (int i = 0; i < MAX_ROOMS; i++) {
        g->names[i][0] = '\0';
        g->adj[i] = NULL;
    }
}

void freeGraph(Graph *g) {
    for (int i = 0; i < MAX_ROOMS; i++) {
        EdgeNode *edge = g->adj[i];
        while (edge != NULL) {
            EdgeNode *next = edge->next;
            free(edge);
            edge = next;
        }
        g->adj[i] = NULL;
    }
    g->count = 0;
}

static int appendEdge(Graph *g, int from, int to) {
    EdgeNode *node = malloc(sizeof(EdgeNode));
    if (node == NULL) {
        return 0;
    }
    node->dest = to;
    node->next = NULL;
    if (g->adj[from] == NULL) {
        g->adj[from] = node;
    } else {
        EdgeNode *tail = g->adj[from];
        while (tail->next != NULL) {
            tail = tail->next;
        }
        tail->next = node;
    }
    return 1;
}

int addEdge(Graph *g, int a, int b) {
    if (a < 0 || b < 0 || a >= g->count || b >= g->count || a == b) {
        return 0;
    }
    return appendEdge(g, a, b) && appendEdge(g, b, a);
}

int loadGraph(Graph *g, const char *path) {
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        return 0;
    }
    char line[MAX_LINE];
    int declared = 0;

    if (fgets(line, sizeof(line), f) == NULL || sscanf(line, "%d", &declared) != 1
        || declared < 1 || declared > MAX_ROOMS) {
        fclose(f);
        return 0;
    }
    g->count = declared;

    int readingEdges = 0;
    while (fgets(line, sizeof(line), f) != NULL) {
        line[strcspn(line, "\r\n")] = '\0';
        if (strcmp(line, EDGES_MARKER) == 0) {
            readingEdges = 1;
            continue;
        }
        if (!readingEdges) {
            int id;
            char name[MAX_NAME];
            if (sscanf(line, ROOM_FORMAT, &id, name) == 2 && id >= 0 && id < g->count) {
                strcpy(g->names[id], name);
            }
        } else {
            int a, b;
            if (sscanf(line, EDGE_FORMAT, &a, &b) == 2) {
                addEdge(g, a, b);
            }
        }
    }
    fclose(f);
    return 1;
}

int getNeighbors(const Graph *g, int room, int *neighbors) {
    int n = 0;
    for (EdgeNode *e = g->adj[room]; e != NULL; e = e->next) {
        neighbors[n++] = e->dest;
    }
    return n;
}

int getBossRoom(const Graph *g) {
    return g->count - 1;
}

void printMap(const Graph *g) {
    printf("\n=== Peta Dungeon ===\n");
    for (int i = 0; i < g->count; i++) {
        printf("[%d] %s -> ", i, g->names[i]);
        for (EdgeNode *e = g->adj[i]; e != NULL; e = e->next) {
            printf("%s%s", g->names[e->dest], e->next ? ", " : "");
        }
        printf("\n");
    }
}

int bfsShortestPath(const Graph *g, int start, int goal, int *path, int *length) {
    int visited[MAX_ROOMS] = {0};
    int parent[MAX_ROOMS];
    for (int i = 0; i < MAX_ROOMS; i++) {
        parent[i] = NO_PARENT;
    }

    Queue queue;
    initQueue(&queue);
    visited[start] = 1;
    enqueue(&queue, start);

    int current;
    while (dequeue(&queue, &current)) {
        for (EdgeNode *e = g->adj[current]; e != NULL; e = e->next) {
            if (!visited[e->dest]) {
                visited[e->dest] = 1;
                parent[e->dest] = current;
                enqueue(&queue, e->dest);
            }
        }
    }
    freeQueue(&queue);

    if (!visited[goal]) {
        return 0;
    }
    int reversed[MAX_ROOMS];
    int n = 0;
    for (int v = goal; v != NO_PARENT; v = parent[v]) {
        reversed[n++] = v;
    }
    for (int i = 0; i < n; i++) {
        path[i] = reversed[n - 1 - i];
    }
    *length = n;
    return 1;
}

static void dfsVisit(const Graph *g, int room, int *visited, int *order) {
    visited[room] = 1;
    printf("%s%s", (*order > 0) ? " -> " : "", g->names[room]);
    (*order)++;
    for (EdgeNode *e = g->adj[room]; e != NULL; e = e->next) {
        if (!visited[e->dest]) {
            dfsVisit(g, e->dest, visited, order);
        }
    }
}

void dfsExplore(const Graph *g, int start) {
    int visited[MAX_ROOMS] = {0};
    int order = 0;
    printf("Urutan eksplorasi DFS: ");
    dfsVisit(g, start, visited, &order);
    printf("\n(%d ruangan dapat dijangkau)\n", order);
}
