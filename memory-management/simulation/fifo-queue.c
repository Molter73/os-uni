// queue.c
#include "queue.h"
#include <assert.h>
#include <stdlib.h>

void freeQueue(Queue* queue);

static void enqueue(Queue* queue, const PageRequest* pr) {
    Node* newNode       = (Node*)malloc(sizeof(Node));
    newNode->page_id    = pr->page_id;
    newNode->process_id = pr->process_id;
    newNode->next       = NULL;
    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear       = newNode;
    }
}

static Node* dequeue(Queue* queue) {
    Node* temp = queue->front;

    if (temp == NULL) {
        return NULL;
    }

    if (temp == queue->rear) {
        queue->front = queue->rear = NULL;
    } else {
        queue->front = queue->front->next;
    }

    temp->next = temp->prev = NULL;
    return temp;
}

// NOLINTNEXTLINE
Queue FIFOQueue = {
    .front   = NULL,
    .rear    = NULL,
    .free    = freeQueue,
    .adjust  = NULL,
    .enqueue = enqueue,
    .dequeue = dequeue,
};
