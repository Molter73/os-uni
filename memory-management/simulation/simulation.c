// simulation.c
#include "simulation.h"
#include "memory.h"
#include "queue.h"
#include "request.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const static int MAX_PAGES = 256;

void initSimulation(Frame frames[], int num_frames) {
    assert(frames != NULL);

    for (int i = 0; i < num_frames; i++) {
        frames[i].frame_id   = i;
        frames[i].page_id    = -1;
        frames[i].process_id = -1;
        frames[i].occupied   = 0;
    }
}

void simulate(PageRequest* requests, unsigned long num_requests, Frame frames[], int num_frames, ProcessPageTables* ppt,
              Queue* queue) {
    for (int i = 0; i < num_requests; i++) {
        processPageRequest(ppt, frames, queue, &requests[i]);
        printMemoryState(frames, num_frames, ppt, queue);
    }
}

void adjustQueue(Queue* q, const PageRequest* pr) {
    Node* node = q->front;
    for (; node != NULL; node = node->next) {
        PageRequest* data = node->data;
        if (data->process_id == pr->process_id && data->page_id == pr->page_id) {
            break;
        }
    }

    if (node == NULL || node == q->rear) {
        return;
    }

    if (node == q->front) {
        q->front       = q->front->next;
        q->front->prev = NULL;
    } else {
        Node* prev       = node->prev;
        prev->next       = node->next;
        node->next->prev = prev;
    }

    node->prev    = q->rear;
    node->next    = NULL;
    q->rear->next = node;
    q->rear       = node;
}

void runTest(PageRequest* requests, unsigned long num_requests, const char* test_name, QueueType queue_type) {
    void (*adjust)(Queue*, void*) = queue_type == LRU ? (void (*)(Queue*, void*))adjustQueue : NULL;
    Frame frames[NUM_FRAMES];
    ProcessPageTables ppt;
    initProcessPageTables(&ppt, 3); // Inicializa tablas de página para 3 procesos
    Queue* queue = newQueue(NULL, adjust);
    initSimulation(frames, NUM_FRAMES);

    printf("Iniciando prueba: %s\n", test_name);
    simulate(requests, num_requests, frames, NUM_FRAMES, &ppt, queue);

    freeProcessPageTables(&ppt);
    freeQueue(queue);
}

void testRandomAccess(int num_requests, QueueType queue_type) { // NOLINT
    PageRequest requests[num_requests];
    for (int i = 0; i < num_requests; i++) {
        requests[i].page_id    = rand() % MAX_PAGES; // Assuming 256 possible pages
        requests[i].process_id = rand() % 3;         // Randomize process IDs among 3 processes
    }
    runTest(requests, num_requests, "Acceso Aleatorio", queue_type);
}

// NOLINTNEXTLINE
void testTemporalLocality(int num_pages, int num_accesses, int num_processes, QueueType queue_type) {
    static const int CACHE_SIZE = 5;
    Queue* previousPages[num_processes];
    PageRequest* requests = malloc(sizeof(PageRequest) * num_accesses);

    for (int i = 0; i < num_processes; i++) {
        previousPages[i] = newQueue(free, NULL);
        if (previousPages[i] == NULL) {
            fprintf(stderr, "Fallo al inicializar cache de páginas\n");
            goto cleanup;
        }
    }

    for (int i = 0; i < num_accesses; i++) {
        int page_id; // NOLINT
        int process_id = rand() % num_processes;
        Queue* q       = previousPages[process_id];
        // Randomly decide to reuse a recent page or access a new one
        if (q->size != 0 && rand() % 2) {
            size_t index = rand() % CACHE_SIZE;
            index        = index < q->size ? index : q->size - 1;

            const Node* n = q->front;
            for (; index != 0; index--) {
                n = n->next;
            }
            page_id = *(int*)n->data;
        } else {
            int* pint = malloc(sizeof(int));
            if (pint == NULL) {
                fprintf(stderr, "Fallo al reservar elemento del cache\n");
                goto cleanup;
            }

            page_id = rand() % num_pages;
            *pint   = page_id;
            enqueue(q, pint);
            while (q->size > CACHE_SIZE) {
                free(dequeue(q));
            }
        }

        requests[i].page_id    = page_id;
        requests[i].process_id = process_id;
    }

    runTest(requests, num_accesses, "Localidad Temporal", queue_type);

cleanup:
    for (int i = 0; i < num_processes; i++) {
        freeQueue(previousPages[i]);
    }
    free(requests);
}

// NOLINTNEXTLINE
void testThrashing(int num_processes, QueueType queue_type) {
    int num_pages         = MAX_PAGES;      // Assuming more pages than frames
    int num_requests      = num_pages * 10; // NOLINT High number of accesses
    PageRequest* requests = malloc(sizeof(PageRequest) * num_requests);
    for (int i = 0; i < num_requests; i++) {
        requests[i].page_id    = rand() % num_pages;
        requests[i].process_id = rand() % num_processes;
    }

    runTest(requests, num_requests, "Thrashing", queue_type);
    free(requests);
}
