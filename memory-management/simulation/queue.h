// queue.h
#ifndef QUEUE_H
#define QUEUE_H

#include "defs.h"

void initQueue(FIFOQueue* queue);
void enqueue(FIFOQueue* queue, int page_id);
int dequeue(FIFOQueue* queue, PageTable* pt);

#endif // QUEUE_H
