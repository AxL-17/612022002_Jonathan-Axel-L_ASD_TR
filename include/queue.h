#ifndef QUEUE_H
#define QUEUE_H

typedef struct QueueNode {
    int value;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
    int size;
} Queue;

void initQueue(Queue *q);
int  enqueue(Queue *q, int value);     /* 1 = berhasil */
int  dequeue(Queue *q, int *value);    /* 0 jika queue kosong */
int  isQueueEmpty(const Queue *q);
void freeQueue(Queue *q);

#endif
