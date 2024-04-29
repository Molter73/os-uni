// queue.h
#ifndef QUEUE_H
#define QUEUE_H

#include "defs.h"

// Nodo para construir una cola utilizada en la gestión de páginas.
typedef struct Node {
    int page_id;       // ID de la página en el nodo.
    struct Node* next; // Puntero al siguiente nodo en la cola.
    struct Node* prev; // Puntero al nodo anterior en la cola.
} Node;

// Cola que se utiliza para el algoritmo de reemplazo de página.
typedef struct Queue {
    Node* front; // Puntero al primer nodo de la cola.
    Node* rear;  // Puntero al último nodo de la cola.

    void (*free)(struct Queue* queue);
    void (*adjust)(struct Queue* queue, int page_id);
    void (*enqueue)(struct Queue* queue, int page_id);
    int (*dequeue)(struct Queue* queue, PageTable* pt);
} Queue;

typedef enum {
    FIFO = 0,
    LRU,
} QueueType;

extern Queue FIFOQueue; // NOLINT
extern Queue LRUQueue;  // NOLINT

#endif // QUEUE_H
