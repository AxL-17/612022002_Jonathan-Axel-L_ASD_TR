#include <stdlib.h>
#include "stack.h"

void initStack(Stack *s) {
    s->top = NULL;
    s->size = 0;
}

int push(Stack *s, int value) {
    StackNode *node = malloc(sizeof(StackNode));
    if (node == NULL) {
        return 0;
    }
    node->value = value;
    node->next = s->top;
    s->top = node;
    s->size++;
    return 1;
}

int pop(Stack *s, int *value) {
    if (s->top == NULL) {
        return 0;
    }
    StackNode *node = s->top;
    *value = node->value;
    s->top = node->next;
    free(node);
    s->size--;
    return 1;
}

int isStackEmpty(const Stack *s) {
    return s->top == NULL;
}

void freeStack(Stack *s) {
    int unused;
    while (pop(s, &unused));
}
