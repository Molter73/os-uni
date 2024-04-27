// queue.c
#include "queue.h"
#include <assert.h>
#include <stdlib.h>

void initQueue(FIFOQueue* queue) {
    assert(queue != NULL);
    queue->front = queue->rear = NULL;
}

void freeQueue(FIFOQueue* queue) {
    assert(queue != NULL);

    for (Node* current = queue->front; current != NULL;) {
        Node* next = current->next;
        free(current);
        current = next;
    }
}

void enqueue(FIFOQueue* queue, int page_id) {
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

int dequeue(FIFOQueue* queue, PageTable* pt) {
    Node* temp  = queue->front;
    Node* prev  = NULL;
    int page_id = -1;
    while (temp != NULL) {
        page_id          = temp->page_id;
        const Page* page = &pt->pages[page_id];
        if (page->valid && page->frame_id == -1) {
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
