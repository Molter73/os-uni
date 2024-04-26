// queue.c
#include "queue.h"
#include <stdlib.h>

void initQueue(FIFOQueue* queue) {
    queue->front = queue->rear = NULL;
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
    Node* temp = queue->front;
    while (temp != NULL) {
        int page_id = temp->page_id;
        if (pt->pages[page_id].valid && pt->pages[page_id].frame_id != -1) {
            if (temp == queue->front) {
                queue->front = temp->next;
                if (queue->front == NULL) {
                    queue->rear = NULL;
                }
            } else {
                Node* prev = queue->front;
                while (prev->next != temp) {
                    prev = prev->next;
                }
                prev->next = temp->next;
                if (prev->next == NULL) {
                    queue->rear = prev;
                }
            }
            free(temp);
            return page_id;
        }
        temp = temp->next;
    }
    return -1;
}
