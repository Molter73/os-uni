// simulation.h
#ifndef SIMULATION_H
#define SIMULATION_H

#include "defs.h"

void initSimulation(Frame frames[], int num_frames);
void simulate(PageRequest* requests, unsigned long num_requests, Frame frames[], int num_frames, ProcessPageTables* ppt,
              FIFOQueue* queue);
void runTest(PageRequest* requests, unsigned long num_requests, const char* test_name);
void testRandomAccess(int num_requests);
void testTemporalLocality(int num_pages, int num_accesses, int num_processes);
void testThrashing(int num_processes);

#endif // SIMULATION_H
