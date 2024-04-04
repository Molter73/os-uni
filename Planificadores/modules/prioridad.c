#include "prioridad.h"
#include "proceso.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h> // Para qsort
#include <string.h>

// Función de comparación inicial por prioridad y tiempo de llegada
int compararPorPrioridad(const void* a, const void* b) { // NOLINT
    const Proceso* proc_a = (const Proceso*)a;
    const Proceso* proc_b = (const Proceso*)b;

    int prioridad = proc_a->prioridad - proc_b->prioridad;

    if (prioridad == 0) {
        return proc_a->tiempoLlegada - proc_b->tiempoLlegada;
    }

    return prioridad;
}

// Ajusta las prioridades de los procesos basándose en el envejecimiento
void ajustarPrioridadesPorEnvejecimiento(procesos_t* procesos, context_t* context) {
    assert(procesos_is_valid(procesos));
    assert(context != NULL);

    static const int PRIORITY_TICK = 20;

    int next   = 0;
    Proceso* p = procesos_get(procesos, next);
    for (; p != NULL; p = procesos_get(procesos, ++next)) {
        int diff = (int)context->time - p->tiempoLlegada;
        if (diff <= 0) {
            continue;
        }

        // Cada PRIORITY_TICK ticks del sistema que espera un proceso,
        // aumentamos su prioridad en 1.
        int modulo = diff % PRIORITY_TICK;
        if (p->prioridad != 0 && modulo == 0) {
            p->prioridad--;
        }
    }

    if (context->prevProcess == NO_PROCESS_READY) {
        return;
    }

    // Una vez se reordenen los procesos, tenemos que ajustar el contexto en
    // caso que el proceso que se estaba ejecutando previamente se haya movido,
    // utilizamos su id para esto
    int id = procesos_get(procesos, context->prevProcess)->id;

    // Ajustamos los procesos según las nuevas prioridades
    planificarPrioridades(procesos);

    next = 0;
    p    = procesos_get(procesos, next);
    for (; p != NULL; p = procesos_get(procesos, ++next)) {
        if (p->id == id) {
            context->prevProcess = next;
            break;
        }
    }
}

static int nextProcess(procesos_t* procesos, context_t* context) {
    assert(procesos_is_valid(procesos));
    assert(context != NULL);

    ajustarPrioridadesPorEnvejecimiento(procesos, context);

    bool processPending = false;

    int next   = 0;
    Proceso* p = procesos_get(procesos, next);
    for (; p != NULL; p = procesos_get(procesos, ++next)) {
        if (procesos_is_ready(p, context->time)) {
            return next;
        }

        if (p->duracionRafaga != 0) {
            processPending = true;
        }
    }

    if (processPending) {
        return NO_PROCESS_READY;
    }
    return DONE_PROCESSING;
}

void planificarPrioridadesPreemptive(procesos_t* procesos, const char* outputPath) {
    context_t context = CONTEXT_INIT;
    FILE* output      = prepararFicheroSalida(outputPath);

    if (output == NULL) {
        fprintf(stderr, "Fallo al abrir '%s'\n", outputPath);
        return;
    }

    // Orden inicial dado por la función sin prevaciado
    planificarPrioridades(procesos);

    do {
        context.process = nextProcess(procesos, &context);
        if (context.process == NO_PROCESS_READY) {
            // Si no tenemos procesos listos para ejecutar, dejamos pasar un tick.
            goto tick;
        }

        // Pasamos de no ejecutar nada a ejecutar algo
        if (context.prevProcess == NO_PROCESS_READY) {
            context.prevProcess = context.process;
            context.prevTime    = context.time;
            goto tick;
        }

        // En este tick continuamos ejecutando el mismo proceso
        if (context.process == context.prevProcess) {
            Proceso* p = procesos_get(procesos, context.process);
            p->duracionRafaga--;
            goto tick;
        }

        // Cambiamos de proceso, guardamos la ejecución del anterior
        Proceso* p  = procesos_get(procesos, context.prevProcess);
        RunFrame rf = {
            .id            = p->id,
            .tiempoLlegada = p->tiempoLlegada,
            .duracion      = context.time - context.prevTime,
            .inicio        = context.prevTime,
            .fin           = context.time,
        };
        guardarFrame(output, &rf);

        context.prevTime    = context.time;
        context.prevProcess = context.process;

    tick:
        context.time++;
    } while (context.process != DONE_PROCESSING);
}

void planificarPrioridades(procesos_t* procesos) {
    // Ordena inicialmente los procesos por prioridad original y tiempo de llegada
    qsort(procesos->ptr, procesos->size, sizeof(Proceso), compararPorPrioridad);
}
