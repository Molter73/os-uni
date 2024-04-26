// request.c
#include "request.h"
#include <stdio.h>
#include <stdlib.h>

void processPageRequest(PageTable* pt, Frame frames[], FIFOQueue* queue, PageRequest request) {
    char logMessage[256];

    // Log the request being processed
    sprintf(logMessage, "Procesando solicitud de página %d para el proceso %d", request.page_id, request.process_id);
    logEvent(logMessage);

    // Log the initial state
    sprintf(logMessage, "Estado inicial: Página %d es %s", request.page_id, pt->pages[request.page_id].valid ? "válida" : "no válida");
    logEvent(logMessage);

    if (pt->pages[request.page_id].valid) {
        sprintf(logMessage, "Página %d ya está en memoria, no se requiere acción.", request.page_id);
        logEvent(logMessage);
        return;
    }

    int frameIndex = findFreeFrame(frames, NUM_FRAMES);
    if (frameIndex == -1) {
        int evictedPageId = dequeue(queue, pt);
        if (evictedPageId == -1) {
            sprintf(logMessage, "No hay páginas válidas en la cola FIFO para desalojar.");
            logEvent(logMessage);
            return;
        }

        frameIndex = pt->pages[evictedPageId].frame_id;
        pt->pages[evictedPageId].valid = 0;
        pt->pages[evictedPageId].frame_id = -1;
        frames[frameIndex].page_id = -1;
        frames[frameIndex].occupied = 0;

        sprintf(logMessage, "Fallos de página: la página %d desalojada del marco %d", evictedPageId, frameIndex);
        logEvent(logMessage);
    }

    frames[frameIndex].page_id = request.page_id;
    frames[frameIndex].occupied = 1;
    pt->pages[request.page_id].frame_id = frameIndex;
    pt->pages[request.page_id].valid = 1;

    enqueue(queue, request.page_id);

    sprintf(logMessage, "Página %d cargada en el marco %d", request.page_id, frameIndex);
    logEvent(logMessage);

    // Log the final state
    sprintf(logMessage, "Estado final: Página %d en marco %d", request.page_id, frameIndex);
    logEvent(logMessage);
}
