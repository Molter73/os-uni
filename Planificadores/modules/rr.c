#include "proceso.h"
#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NO_PROCESS_READY -1
#define DONE_PROCESSING  -2

// Estructura auxiliar para contener el contexto de ejecución.
typedef struct context_s {
    int process;
    unsigned int time;
} context_t;

FILE* prepararFicheroSalida(const char* outputPath) {
    FILE* output = fopen(outputPath, "w");
    if (output == NULL) {
        fprintf(stderr, "Error al abrir archivo '%s': %s", outputPath, strerror(errno));
        return NULL;
    }

    fprintf(output, "ID Proceso,Tiempo de Llegada,Duración de la Ráfaga,Inicio de Ejecución,Fin de Ejecución\n");

    return output;
}

bool isReady(const Proceso* p, unsigned int currentTime) {
    assert(p != NULL);

    return p->tiempoLlegada <= currentTime && p->duracionRafaga != 0;
}

int nextProcess(const procesos_t* procesos, const context_t* context) {
    assert(procesos_is_valid(procesos));
    assert(context != NULL);

    bool processPending = false;
    bool noProcessReady = false;

    // Iteramos todos los procesos para encontrar cuál se debe ejecutar.
    size_t next = context->process + 1;
    const Proceso* p = procesos_get(procesos, next);
    for (; next != context->process; p = procesos_get(procesos, ++next)) {
        if (p == NULL) {
            next = 0;
            p    = procesos_get(procesos, next);

            // En caso que no hubiese ningún proceso ejecutándose, marcamos
            // esta condición y si damos la vuelta sin encontrar otro proceso
            // devolvemos NO_PROCESS_READY.
            if (context->process == NO_PROCESS_READY) {
                if (noProcessReady) {
                    return NO_PROCESS_READY;
                }
                noProcessReady = true;
            }
        }

        if (isReady(p, context->time)) {
            return next;
        }

        if (p->duracionRafaga != 0) {
            processPending = true;
        }
    }

    // Llegado este punto estamos en el mismo proceso inicial,
    // si está listo para ejecutar, lo ejecutamos.
    if (isReady(procesos_get(procesos, next), context->time)) {
        return context->process;
    }

    if (processPending) {
        // No hemos terminado de ejecutar procesos, pero no tenemos procesos preparados
        return NO_PROCESS_READY;
    }
    // Revisamos todos los procesos y no queda nada por procesar
    return DONE_PROCESSING;
}

void planificarRR(procesos_t* procesos, int quantum, const char* outputPath) {
    context_t context = {.process = NO_PROCESS_READY, .time = 0};
    FILE* output      = prepararFicheroSalida(outputPath);

    if (output == NULL) {
        return;
    }

    while (true) {
        context.process = nextProcess(procesos, &context);
        while (context.process == NO_PROCESS_READY) {
            // Si no tenemos procesos listos para ejecutar, dejamos pasar un tick.
            context.time++;
            context.process = nextProcess(procesos, &context);
        }

        if (context.process == DONE_PROCESSING) {
            break;
        }

        Proceso* p             = procesos_get(procesos, context.process);
        unsigned int startTime = context.time;

        if (p->duracionRafaga < quantum) {
            context.time += p->duracionRafaga;
            p->duracionRafaga = 0; // El proceso terminó su ejecución.
        } else {
            context.time += quantum;
            p->duracionRafaga -= quantum;
        }

        fprintf(output, "%d,%d,%d,%d,%d\n", p->id, p->tiempoLlegada, context.time - startTime, startTime, context.time);
    }

    fclose(output);
}
