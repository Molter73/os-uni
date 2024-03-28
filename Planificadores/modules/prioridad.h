#ifndef PRIORIDAD_H
#define PRIORIDAD_H

#include "proceso.h"
#include <stdbool.h>

void planificarPrioridades(procesos_t* procesos);
void planificarPrioridadesPreemptive(procesos_t* procesos, const char* outputPath);

#endif
