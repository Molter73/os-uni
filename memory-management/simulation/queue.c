// queue.c
#include "queue.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

void freeQueue(Queue* queue) {
    assert(queue != NULL);

    for (Node* current = queue->front; current != NULL;) {
        Node* next = current->next;
        free(current);
        current = next;
    }
}

bool isInQueue(const Queue* queue, int frame_id) {
    for (Node* n = queue->front; n != NULL; n = n->next) {
        if (n->frame_id == frame_id) {
            return true;
        }
    }
    return false;
}
