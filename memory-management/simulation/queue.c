// queue.c
#include "queue.h"
#include <assert.h>
#include <stdlib.h>

void freeNode(Node* node) {
    free(node);
}

Node* allocNode(void* data) {
    Node* newNode = (Node*)calloc(1, sizeof(Node));
    if (newNode == NULL) {
        return NULL;
    }

    newNode->data = data;
    return newNode;
}

Queue* newQueue(void (*freeData)(void*), void (*adjust)(Queue*, void*)) {
    Queue* q = calloc(1, sizeof(Queue));
    if (q == NULL) {
        return NULL;
    }

    q->freeData = freeData;
    q->adjust   = adjust;
    return q;
}

void freeQueue(Queue* q) {
    if (q == NULL) {
        return;
    }

    for (Node* current = q->front; current != NULL;) {
        Node* next = current->next;
        if (q->freeData != NULL) {
            q->freeData(current->data);
        }
        freeNode(current);
        current = next;
    }
    free(q);
}

void enqueue(Queue* q, void* data) {
    Node* newNode = allocNode(data);
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        newNode->prev = q->rear;
        q->rear->next = newNode;
        q->rear       = newNode;
    }
}

void* dequeue(Queue* q) {
    Node* temp = q->front;
    if (temp == NULL) {
        return NULL;
    }
    void* data = temp->data;

    if (temp == q->rear) {
        q->front = q->rear = NULL;
    } else {
        q->front       = q->front->next;
        q->front->prev = NULL;
    }

    freeNode(temp);
    return data;
}
