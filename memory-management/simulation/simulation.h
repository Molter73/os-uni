// simulation.h
#ifndef SIMULATION_H
#define SIMULATION_H

#include "defs.h"
#include "queue.h"

void initSimulation(Frame frames[], int num_frames);
void simulate(PageRequest* requests, unsigned long num_requests, Frame frames[], int num_frames, ProcessPageTables* ppt,
              Queue* queue);
void runTest(PageRequest* requests, unsigned long num_requests, const char* test_name, QueueType queue_type);
void testRandomAccess(int num_requests, QueueType queue_type);
void testTemporalLocality(int num_pages, int num_accesses, int num_processes, QueueType queue_type);
void testThrashing(int num_processes, QueueType queue_type);

#endif // SIMULATION_H
