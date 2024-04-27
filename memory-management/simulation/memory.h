// memory.h
#ifndef MEMORY_H
#define MEMORY_H

#include "defs.h"

void initPageTable(PageTable* pt, int num_pages);
void initProcessPageTables(ProcessPageTables* ppt, int num_processes);
void freeProcessPageTables(ProcessPageTables* ppt);
int findFreeFrame(Frame frames[], int num_frames);
void printMemoryState(Frame frames[], int num_frames, ProcessPageTables ppt, FIFOQueue queue);

#endif // MEMORY_H
