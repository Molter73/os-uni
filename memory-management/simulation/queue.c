// queue.c
#include "queue.h"
#include <assert.h>
#include <stdlib.h>

void initQueue(Queue* queue) {
    assert(queue != NULL);
    queue->front = queue->rear = NULL;
}

void freeQueue(Queue* queue) {
    assert(queue != NULL);

    for (Node* current = queue->front; current != NULL;) {
        Node* next = current->next;
        free(current);
        current = next;
    }
}
