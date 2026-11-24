#include <stdlib.h>
#include "queue.h"

void initQueue(Queue *q) {
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
}

int enqueue(Queue *q, int value) {
    QueueNode *node = malloc(sizeof(QueueNode));
    if (node == NULL) {
        return 0;
    }
    node->value = value;
    node->next = NULL;
    if (q->rear != NULL) {
        q->rear->next = node;
    } else {
        q->front = node;
    }
    q->rear = node;
    q->size++;
    return 1;
}

int dequeue(Queue *q, int *value) {
    if (q->front == NULL) {
        return 0;
    }
    QueueNode *node = q->front;
    *value = node->value;
    q->front = node->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    free(node);
    q->size--;
    return 1;
}

int isQueueEmpty(const Queue *q) {
    return q->front == NULL;
}

void freeQueue(Queue *q) {
    int unused;
    while (dequeue(q, &unused));
}
