// request.c
#include "request.h"
#include "defs.h"
#include "log.h"
#include "memory.h"
#include "queue.h"
#include <stddef.h>
#include <stdlib.h>

int evictPage(ProcessPageTables* ppt, Frame frames[], Queue* queue) {
    PageRequest* evictedNode = dequeue(queue);
    if (evictedNode == NULL) {
        logEvent("No hay páginas válidas en la cola FIFO para desalojar.");
        return -1;
    }

    PageTable* pt  = &ppt->tables[evictedNode->process_id];
    Page* page     = &pt->pages[evictedNode->page_id];
    int frameIndex = page->frame_id;

    page->valid    = 0;
    page->frame_id = -1;

    frames[frameIndex].page_id    = -1;
    frames[frameIndex].process_id = -1;
    frames[frameIndex].occupied   = 0;

    logEvent("Fallos de página: la página %d desalojada del marco %d", evictedNode, frameIndex);
    return frameIndex;
}

void processPageRequest(ProcessPageTables* ppt, Frame frames[], Queue* q, PageRequest* request) {
    // Log the request being processed
    logEvent("Procesando solicitud de página %d para el proceso %d", request->page_id, request->process_id);

    // Log the initial state
    PageTable* pt = &ppt->tables[request->process_id];
    logEvent("Estado inicial: Página %d es %s", request->page_id,
             pt->pages[request->page_id].valid ? "válida" : "no válida");

    if (pt->pages[request->page_id].valid) {
        logEvent("Página %d ya está en memoria.", request->page_id);
        if (q->adjust != NULL) {
            q->adjust(q, request);
        }
        return;
    }

    int frameIndex = findFreeFrame(frames, NUM_FRAMES);
    if (frameIndex == -1) {
        frameIndex = evictPage(ppt, frames, q);
        if (frameIndex == -1) {
            return;
        }
    }

    frames[frameIndex].page_id    = request->page_id;
    frames[frameIndex].process_id = request->process_id;
    frames[frameIndex].occupied   = 1;

    pt->pages[request->page_id].frame_id = frameIndex;
    pt->pages[request->page_id].valid    = 1;

    enqueue(q, request);

    logEvent("Página %d cargada en el marco %d", request->page_id, frameIndex);

    // Log the final state
    logEvent("Estado final: Página %d en marco %d", request->page_id, frameIndex);
}
