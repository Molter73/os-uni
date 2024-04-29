// queue.c
#include "queue.h"
#include <assert.h>
#include <stdlib.h>

void freeQueue(Queue* queue);

static void enqueue(Queue* queue, int page_id) {
    Node* newNode    = (Node*)malloc(sizeof(Node));
    newNode->page_id = page_id;
    newNode->next    = NULL;
    if (queue->rear == NULL) {
        queue->front = queue->rear = newNode;
    } else {
        queue->rear->next = newNode;
        queue->rear       = newNode;
    }
}

static int dequeue(Queue* queue, PageTable* pt) {
    Node* temp  = queue->front;
    Node* prev  = NULL;
    int page_id = -1;
    while (temp != NULL) {
        page_id          = temp->page_id;
        const Page* page = &pt->pages[page_id];
        if (page->valid && page->frame_id != -1) {
            break;
        }

        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        return -1;
    }

    if (temp == queue->front) {
        queue->front = temp->next;
        if (queue->front == NULL) {
            queue->rear = NULL;
        }
    } else {
        prev->next = temp->next;
        if (prev->next == NULL) {
            queue->rear = prev;
        }
    }
    free(temp);
    return page_id;
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
