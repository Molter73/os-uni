// request.h
#ifndef REQUEST_H
#define REQUEST_H

#include "defs.h"
#include "log.h"
#include "memory.h"
#include "queue.h"

void processPageRequest(PageTable* pt, Frame frames[], FIFOQueue* queue, PageRequest request);

#endif // REQUEST_H
