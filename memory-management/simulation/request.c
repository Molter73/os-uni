// request.c
#include "request.h"
#include "log.h"
#include "memory.h"
#include "queue.h"
#include <stddef.h>

int evictFrame(PageTable* pt, Frame frames[], Queue* queue) {
    int evictedFrameId = queue->dequeue(queue);
    if (evictedFrameId == -1) {
        logEvent("No hay páginas válidas en la cola FIFO para desalojar.");
        return -1;
    }

    int pageId                      = frames[evictedFrameId].page_id;
    pt->pages[pageId].valid         = 0;
    pt->pages[pageId].frame_id      = -1;
    frames[evictedFrameId].page_id  = -1;
    frames[evictedFrameId].occupied = 0;

    logEvent("Fallos de página: la página %d desalojada del marco %d", evictedFrameId, pageId);
    return evictedFrameId;
}

void processPageRequest(PageTable* pt, Frame frames[], Queue* queue, PageRequest request) {
    // Log the request being processed
    logEvent("Procesando solicitud de página %d para el proceso %d", request.page_id, request.process_id);

    // Log the initial state
    logEvent("Estado inicial: Página %d es %s", request.page_id,
             pt->pages[request.page_id].valid ? "válida" : "no válida");

    Page* page = &pt->pages[request.page_id];
    if (page->valid) {
        logEvent("Página %d ya está en memoria.", request.page_id);
        if (queue->adjust != NULL) {
            queue->adjust(queue, page->frame_id);
        }
        return;
    }

    int frameIndex = findFreeFrame(frames, NUM_FRAMES);
    if (frameIndex == -1) {
        frameIndex = evictFrame(pt, frames, queue);
        if (frameIndex == -1) {
            return;
        }
    }

    frames[frameIndex].page_id  = request.page_id;
    frames[frameIndex].occupied = 1;
    page->frame_id              = frameIndex;
    page->valid                 = 1;

    queue->enqueue(queue, frameIndex);

    logEvent("Página %d cargada en el marco %d", request.page_id, frameIndex);

    // Log the final state
    logEvent("Estado final: Página %d en marco %d", request.page_id, frameIndex);
}
