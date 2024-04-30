// queue.c
#include "queue.h"
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

void freeQueue(Queue* queue);
bool isInQueue(const Queue* queue, int frame_id);

static void enqueue(Queue* queue, int frame_id) {
    // El siguiente assert se dispara si se intenta agregar un nodo que
    // se encuentra presente en la cola.
    assert(!isInQueue(queue, frame_id));

    Node* newNode     = (Node*)malloc(sizeof(Node));
    newNode->frame_id = frame_id;
    newNode->next     = NULL;
    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear       = newNode;
    }
}

static int dequeue(Queue* queue) {
    Node* temp = queue->front;
    if (temp == NULL) {
        return -1;
    }

    int frame_id = temp->frame_id;
    if (temp == queue->rear) {
        queue->front = queue->rear = NULL;
    } else {
        queue->front = temp->next;
    }

    free(temp);
    return frame_id;
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
