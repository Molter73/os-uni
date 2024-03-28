#include "proceso.h"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int nextProcess(const procesos_t* procesos, const context_t* context) {
    assert(procesos_is_valid(procesos));
    assert(context != NULL);

    bool processPending = false;
    bool noProcessReady = false;

    // Iteramos todos los procesos para encontrar cuál se debe ejecutar.
    size_t next      = context->process + 1;
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

        if (procesos_is_ready(p, context->time)) {
            return next;
        }

        if (p->duracionRafaga != 0) {
            processPending = true;
        }
    }

    // Llegado este punto estamos en el mismo proceso inicial,
    // si está listo para ejecutar, lo ejecutamos.
    if (procesos_is_ready(procesos_get(procesos, next), context->time)) {
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
    context_t context = CONTEXT_INIT;
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

        RunFrame rf = {
            .id            = p->id,
            .tiempoLlegada = p->tiempoLlegada,
            .duracion      = context.time - startTime,
            .inicio        = startTime,
            .fin           = context.time,
        };

        guardarFrame(output, &rf);
    }

    fclose(output);
}
