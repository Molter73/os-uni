// queue.h
#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

typedef enum {
    FIFO = 0,
    LRU,
} QueueType;

// Nodo para construir una cola utilizada en la gestión de páginas.
typedef struct Node {
    void* data;
    struct Node* next; // Puntero al siguiente nodo en la cola.
    struct Node* prev; // Puntero al nodo anterior en la cola.
} Node;

// Cola que se utiliza para el algoritmo de reemplazo de página.
typedef struct Queue {
    Node* front; // Puntero al primer nodo de la cola.
    Node* rear;  // Puntero al último nodo de la cola.
    size_t size;

    void (*freeData)(void*);
    void (*adjust)(struct Queue*, void*);
} Queue;

Queue* newQueue(void (*freeData)(void*), void (*adjust)(struct Queue*, void*));
void freeQueue(Queue* q);
void enqueue(Queue* q, void* data);
void* dequeue(Queue* q);

#endif // QUEUE_H
