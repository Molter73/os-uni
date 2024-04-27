// queue.h
#ifndef QUEUE_H
#define QUEUE_H

#include "defs.h"

// Nodo para construir una cola utilizada en la gestión de páginas.
typedef struct Node {
    int page_id;       // ID de la página en el nodo.
    struct Node* next; // Puntero al siguiente nodo en la cola.
} Node;

// Tabla virtual de funciones de inicialización y acceso a colas
typedef struct {
    void (*init)(void* queue);
    void (*free)(void* queue);
    void (*enqueue)(void* queue, int page_id);
    int (*dequeue)(void* queue, PageTable* pt);
} QueueVtable;

// Cola que se utiliza para el algoritmo de reemplazo de página.
typedef struct {
    Node* front; // Puntero al primer nodo de la cola.
    Node* rear;  // Puntero al último nodo de la cola.

    QueueVtable vtable;
} Queue;

extern Queue FIFOQueue; // NOLINT

#endif // QUEUE_H
