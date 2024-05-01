// queue.c
#include "queue.h"
#include <assert.h>
#include <stdlib.h>

void freeQueue(Queue* queue) {
    assert(queue != NULL);

    for (Node* current = queue->front; current != NULL;) {
        Node* next = current->next;
        freeNode(current);
        current = next;
    }
}

void freeNode(Node* node) {
    free(node);
}
