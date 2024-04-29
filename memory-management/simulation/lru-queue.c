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
static int dequeue(Queue* queue, PageTable* pt, int process_id) {
    Node* temp  = queue->rear;
    int page_id = -1;

    while (temp != NULL) {
        page_id          = temp->page_id;
        const Page* page = &pt->pages[page_id];
        if (temp->process_id == process_id && page->valid && page->frame_id != -1) {
            break;
        }

        temp = temp->prev;
    }

    if (temp == NULL) {
        return -1;
    }

    if (temp == queue->front) {
        queue->front = temp->next;
        if (queue->front == NULL) {
            queue->rear = NULL;
        } else {
            queue->front->prev = NULL;
        }
    } else {
        Node* prev = temp->prev;
        prev->next = temp->next;
        if (temp->next == NULL) {
            queue->rear = prev;
        } else {
            temp->next->prev = prev;
        }
    }
    free(temp);
    return page_id;
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
