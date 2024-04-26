// request.h
#ifndef REQUEST_H
#define REQUEST_H

#include "defs.h"

void processPageRequest(PageTable* pt, Frame frames[], FIFOQueue* queue, PageRequest request);

#endif // REQUEST_H
