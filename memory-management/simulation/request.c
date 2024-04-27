// request.c
#include "request.h"
#include "log.h"
#include "memory.h"
#include "queue.h"

int evictPage(PageTable* pt, Frame frames[], Queue* queue) {
    int evictedPageId = queue->vtable.dequeue(queue, pt);
    if (evictedPageId == -1) {
        logEvent("No hay páginas válidas en la cola FIFO para desalojar.");
        return -1;
    }

    int frameIndex                    = pt->pages[evictedPageId].frame_id;
    pt->pages[evictedPageId].valid    = 0;
    pt->pages[evictedPageId].frame_id = -1;
    frames[frameIndex].page_id        = -1;
    frames[frameIndex].occupied       = 0;

    logEvent("Fallos de página: la página %d desalojada del marco %d", evictedPageId, frameIndex);
    return frameIndex;
}

void processPageRequest(PageTable* pt, Frame frames[], Queue* queue, PageRequest request) {
    // Log the request being processed
    logEvent("Procesando solicitud de página %d para el proceso %d", request.page_id, request.process_id);

    // Log the initial state
    logEvent("Estado inicial: Página %d es %s", request.page_id,
             pt->pages[request.page_id].valid ? "válida" : "no válida");

    if (pt->pages[request.page_id].valid) {
        logEvent("Página %d ya está en memoria, no se requiere acción.", request.page_id);
        return;
    }

    int frameIndex = findFreeFrame(frames, NUM_FRAMES);
    if (frameIndex == -1) {
        frameIndex = evictPage(pt, frames, queue);
        if (frameIndex == -1) {
            return;
        }
    }

    frames[frameIndex].page_id          = request.page_id;
    frames[frameIndex].occupied         = 1;
    pt->pages[request.page_id].frame_id = frameIndex;
    pt->pages[request.page_id].valid    = 1;

    queue->vtable.enqueue(queue, request.page_id);

    logEvent("Página %d cargada en el marco %d", request.page_id, frameIndex);

    // Log the final state
    logEvent("Estado final: Página %d en marco %d", request.page_id, frameIndex);
}
