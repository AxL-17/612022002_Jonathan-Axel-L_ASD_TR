#ifndef STACK_H
#define STACK_H

typedef struct StackNode {
    int value;
    struct StackNode *next;
} StackNode;

typedef struct {
    StackNode *top;
    int size;
} Stack;

void initStack(Stack *s);
int  push(Stack *s, int value);        /* 1 = berhasil */
int  pop(Stack *s, int *value);        /* 0 jika stack kosong */
int  isStackEmpty(const Stack *s);
void freeStack(Stack *s);

#endif
