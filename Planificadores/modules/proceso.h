#ifndef PROCESO_H
#define PROCESO_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    int id;
    int tiempoLlegada;
    int duracionRafaga;
    int prioridad;
} Proceso;

typedef struct procesos_s {
    Proceso* ptr;
    size_t size;
} procesos_t;

Proceso* procesos_get(const procesos_t* p, size_t i);
void procesos_free(procesos_t* p);
bool procesos_is_valid(const procesos_t* p);
procesos_t* leerProcesosDesdeArchivoBinario(const char* nombreArchivo);
void guardarProcesosEnCSV(const char* nombreArchivo, procesos_t* procesos);

#endif
