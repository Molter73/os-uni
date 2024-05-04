// simulation.h
#ifndef SIMULATION_H
#define SIMULATION_H

#include "defs.h"

void testRandomAccess(const TestConfiguration* config);
void testTemporalLocality(const TestConfiguration* config);
void testThrashing(const TestConfiguration* config);

#endif // SIMULATION_H
