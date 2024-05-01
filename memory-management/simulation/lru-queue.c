// lru-queue.c
#include "queue.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void freeQueue(Queue* queue);

static void adjust(Queue* queue, const PageRequest* pr) {
    Node* target = queue->front;
    while (target != NULL && target->page_id != pr->page_id && target->process_id != pr->process_id) {
        target = target->next;
    }

    if (target == NULL) {
        fprintf(stderr, "La página %d no estaba en la cola, ESTO NO DEBERIA PASAR!!\n", pr->page_id);
        exit(EXIT_FAILURE);
    }

    if (target == queue->front) {
        // Ya es la página más reciente, nada que hacer
        return;
    }

    // Movemos la página al principio de la cola.
    Node* prev = target->prev;
    prev->next = target->next;
    if (target->next == NULL) {
        queue->rear = prev;
    } else {
        target->next->prev = prev;
    }
    target->next       = queue->front;
    target->prev       = NULL;
    queue->front->prev = target;
    queue->front       = target;
}

// Agrega el nodo al principio de la cola.
static void enqueue(Queue* queue, const PageRequest* pr) {
    Node* newNode       = (Node*)malloc(sizeof(Node));
    newNode->page_id    = pr->page_id;
    newNode->process_id = pr->process_id;
    newNode->next       = queue->front;
    newNode->prev       = NULL;

    if (queue->front != NULL) {
        queue->front->prev = newNode;
    }

    queue->front = newNode;
    if (queue->rear == NULL) {
        queue->rear = queue->front;
    }
}

// Elimina siempre el último nodo del a cola.
static Node* dequeue(Queue* queue) {
    Node* temp = queue->rear;

    if (temp == NULL) {
        return NULL;
    }

    if (temp == queue->front) {
        queue->front = queue->rear = NULL;
    } else {
        Node* prev  = temp->prev;
        prev->next  = NULL;
        queue->rear = prev;
    }

    temp->next = temp->prev = NULL;
    return temp;
}

// NOLINTNEXTLINE
Queue LRUQueue = {
    .front   = NULL,
    .rear    = NULL,
    .free    = freeQueue,
    .adjust  = adjust,
    .enqueue = enqueue,
    .dequeue = dequeue,
};
