// memory.c
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>

void initPageTable(PageTable* pt, int num_pages) {
    pt->pages     = (Page*)malloc(sizeof(Page) * num_pages);
    pt->num_pages = num_pages;
    for (int i = 0; i < num_pages; i++) {
        pt->pages[i].page_id  = i;
        pt->pages[i].frame_id = -1;
        pt->pages[i].valid    = 0;
    }
}

void initProcessPageTables(ProcessPageTables* ppt, int num_processes) {
    static const int PAGES_PER_PROCESS = 256;
    ppt->tables                        = malloc(num_processes * sizeof(PageTable));
    ppt->num_processes                 = num_processes;
    for (int i = 0; i < num_processes; i++) {
        initPageTable(&(ppt->tables[i]), PAGES_PER_PROCESS); // Assign 256 pages per process
    }
}

int findFreeFrame(Frame frames[], int num_frames) {
    for (int i = 0; i < num_frames; i++) {
        if (!frames[i].occupied) {
            return i;
        }
    }
    return -1;
}

void printMemoryState(Frame frames[], int num_frames, ProcessPageTables ppt, FIFOQueue queue) {
    FILE* file = fopen("memory_details.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    fprintf(file, "Estado Actual de los Marcos de Memoria:\n");
    for (int i = 0; i < num_frames; i++) {
        if (frames[i].occupied) {
            int processId = -1;
            // Buscar a qué proceso pertenece la página en el marco
            for (int p = 0; p < ppt.num_processes; p++) {
                if (ppt.tables[p].pages[frames[i].page_id].valid &&
                    ppt.tables[p].pages[frames[i].page_id].frame_id == i) {
                    processId = p;
                    break;
                }
            }
            fprintf(file, "Marco %d: Ocupado por página %d del proceso %d\n", i, frames[i].page_id, processId);
        } else {
            fprintf(file, "Marco %d: Libre\n", i);
        }
    }

    fprintf(file, "\nDetalle de la Memoria para cada Proceso:\n");
    for (int p = 0; p < ppt.num_processes; p++) {
        PageTable pt = ppt.tables[p];
        fprintf(file, "\nProceso %d:\n", p);
        fprintf(file, "Estado de la Tabla de Páginas:\n");
        for (int i = 0; i < pt.num_pages; i++) {
            fprintf(file, "Página %d: %s en marco %d\n", i, pt.pages[i].valid ? "Presente" : "No presente",
                    pt.pages[i].frame_id);
        }
    }

    fprintf(file, "\nContenido de la cola FIFO (orden de desalojo):\n");
    Node* current = queue.front;
    while (current != NULL) {
        fprintf(file, "Página %d -> ", current->page_id);
        current = current->next;
    }
    fprintf(file, "NULL\n");

    fclose(file);
}
