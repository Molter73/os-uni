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
        frames[i].frame_id = i;
        frames[i].page_id  = -1;
        frames[i].occupied = 0;
    }
}

void simulate(PageRequest* requests, unsigned long num_requests, Frame frames[], int num_frames, ProcessPageTables* ppt,
              Queue* queue) {
    for (int i = 0; i < num_requests; i++) {
        PageRequest req = requests[i];
        PageTable* pt   = &(ppt->tables[req.process_id]); // Use the page table of the corresponding process
        processPageRequest(pt, frames, queue, req);
        printMemoryState(frames, num_frames, *ppt, *queue);
    }
}

void runTest(PageRequest* requests, unsigned long num_requests, const char* test_name, QueueType queue_type) {
    Frame frames[NUM_FRAMES];
    ProcessPageTables ppt;
    initProcessPageTables(&ppt, 3); // Inicializa tablas de página para 3 procesos
    Queue* queue = queue_type == FIFO ? &FIFOQueue : &LRUQueue;
    initSimulation(frames, NUM_FRAMES);

    printf("Iniciando prueba: %s\n", test_name);
    simulate(requests, num_requests, frames, NUM_FRAMES, &ppt, queue);

    freeProcessPageTables(&ppt);
    queue->free(queue);
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
    PageRequest* requests = malloc(sizeof(PageRequest) * num_accesses);
    for (int i = 0; i < num_accesses; i++) {
        int page_id = rand() % num_pages;
        // Randomly decide to reuse a recent page or access a new one
        if (rand() % 2) {
            page_id = (page_id + rand() % 5) % num_pages; // NOLINT Reuse within a range of recent pages
        }

        requests[i].page_id    = page_id;
        requests[i].process_id = rand() % num_processes;
    }

    runTest(requests, num_accesses, "Localidad Temporal", queue_type);
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
