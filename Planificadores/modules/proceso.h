#ifndef PROCESO_H
#define PROCESO_H

#include <stdbool.h>
#include <stdio.h>

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

typedef struct RunFrame_s {
    int id;
    int tiempoLlegada;
    unsigned int duracion;
    unsigned int inicio;
    unsigned int fin;
} RunFrame;

// Estructura auxiliar para contener el contexto de ejecución.
typedef struct context_s {
    int process;
    int prevProcess;
    unsigned int time;
    unsigned int prevTime;
} context_t;

#define NO_PROCESS_READY (-1)
#define DONE_PROCESSING  (-2)
#define CONTEXT_INIT \
    { .process = NO_PROCESS_READY, .prevProcess = NO_PROCESS_READY, .time = 0, .prevTime = 0, }

procesos_t* procesos_new(size_t numProcesos);
void procesos_free(procesos_t* p);
Proceso* procesos_get(const procesos_t* p, size_t i);
bool procesos_is_valid(const procesos_t* p);
bool procesos_is_ready(const Proceso* p, unsigned int currentTime);
procesos_t* leerProcesosDesdeArchivoBinario(const char* nombreArchivo);
void guardarProcesosEnCSV(const char* nombreArchivo, procesos_t* procesos);
FILE* prepararFicheroSalida(const char* outputPath);
void guardarFrame(FILE* f, RunFrame* rf);

#endif
