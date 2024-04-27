// request.h
#ifndef REQUEST_H
#define REQUEST_H

#include "defs.h"
#include "queue.h"

void processPageRequest(PageTable* pt, Frame frames[], Queue* queue, PageRequest request);

#endif // REQUEST_H
