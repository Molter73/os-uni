// simulation.c
#include "simulation.h"
#include "memory.h"
#include "queue.h"
#include "request.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const static int MAX_PAGES = 256;

void initSimulation(Frame frames[], int num_frames) {
    if (!frames) {
        perror("Error: El arreglo de marcos no está inicializado.");
        return;
    }

    for (int i = 0; i < num_frames; i++) {
        frames[i].frame_id = i;
        frames[i].page_id  = -1;
        frames[i].occupied = 0;
    }
}

void simulate(PageRequest* requests, unsigned long num_requests, Frame frames[], int num_frames, ProcessPageTables* ppt,
              FIFOQueue* queue) {
    for (int i = 0; i < num_requests; i++) {
        PageRequest req = requests[i];
        PageTable* pt   = &(ppt->tables[req.process_id]); // Use the page table of the corresponding process
        processPageRequest(pt, frames, queue, req);
        printMemoryState(frames, num_frames, *ppt, *queue);
        // usleep(500000); // Visual pause
    }
}

void runTest(PageRequest* requests, unsigned long num_requests, const char* test_name) {
    Frame frames[NUM_FRAMES];
    ProcessPageTables ppt;
    initProcessPageTables(&ppt, 3); // Inicializa tablas de página para 3 procesos
    FIFOQueue queue;
    initQueue(&queue);
    initSimulation(frames, NUM_FRAMES);

    printf("Iniciando prueba: %s\n", test_name);
    simulate(requests, num_requests, frames, NUM_FRAMES, &ppt, &queue);

    for (int i = 0; i < ppt.num_processes; i++) {
        free(ppt.tables[i].pages); // Free each process's pages
    }
    free(ppt.tables); // Free the array of page tables

    Node* current = queue.front;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
}

void testRandomAccess(int value) {
    PageRequest requests[value];
    for (int i = 0; i < value; i++) {
        requests[i].page_id    = rand() % MAX_PAGES; // Assuming 256 possible pages
        requests[i].process_id = rand() % 3;         // Randomize process IDs among 3 processes
    }
    unsigned long num_requests = sizeof(requests) / sizeof(requests[0]);
    runTest(requests, num_requests, "Acceso Aleatorio");
}

// NOLINTNEXTLINE
void testTemporalLocality(int num_pages, int num_accesses, int num_processes) {
    PageRequest* requests = malloc(sizeof(PageRequest) * num_accesses);
    for (int i = 0; i < num_accesses; i++) {
        int page_id    = rand() % num_pages;
        int process_id = rand() % num_processes;
        // Randomly decide to reuse a recent page or access a new one
        if (rand() % 2) {
            page_id = (page_id + rand() % 5) % num_pages; // NOLINT Reuse within a range of recent pages
        }
        requests[i] = (PageRequest){.page_id = page_id, .process_id = process_id};
    }
    runTest(requests, num_accesses, "Localidad Temporal");
    free(requests);
}

void testThrashing(int num_processes) {
    int num_pages         = MAX_PAGES;      // Assuming more pages than frames
    int num_requests      = num_pages * 10; // NOLINT High number of accesses
    PageRequest* requests = malloc(sizeof(PageRequest) * num_requests);
    for (int i = 0; i < num_requests; i++) {
        requests[i] = (PageRequest){.page_id = rand() % num_pages, .process_id = rand() % num_processes};
    }
    runTest(requests, num_requests, "Thrashing");
    free(requests);
}
