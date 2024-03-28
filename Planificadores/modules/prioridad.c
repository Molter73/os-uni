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
    assert(procesos != NULL);
    assert(context != NULL);
    const unsigned int PRIORITY_TICK = 100;

    int i      = 0;
    Proceso* p = procesos_get(procesos, i);
    for (; p != NULL; p = procesos_get(procesos, ++i)) {
        // Cada PRIORITY_TICK ticks del sistema que espera un proceso, aumentamos su prioridad en 1.
        if (p->prioridad != 0 && ((context->time - p->tiempoLlegada) % PRIORITY_TICK == 0)) {
            p->prioridad--;
        }
    }

    if (context->prevProcess == NO_PROCESS_READY) {
        return;
    }

    // Una vez se reordenen los procesos, tenemos que ajustar el contexto en
    // caso que el proceso que estaba ejecutando previamente se haya movido,
    // utilizamos su id para esto
    int id = procesos_get(procesos, context->prevProcess)->id;

    // Ajustamos los procesos según las nuevas prioridades
    planificarPrioridades(procesos);

    i = 0;
    p = procesos_get(procesos, i);
    for (; p != NULL; p = procesos_get(procesos, ++i)) {
        if (p->id == id) {
            context->prevProcess = i;
            break;
        }
    }
}

static int nextProcess(procesos_t* procesos, context_t* context) {
    assert(procesos != NULL);
    assert(context != NULL);

    ajustarPrioridadesPorEnvejecimiento(procesos, context);

    bool processPending = false;

    int i      = 0;
    Proceso* p = procesos_get(procesos, i);
    for (; p != NULL; p = procesos_get(procesos, ++i)) {
        if (procesos_is_ready(p, context->time)) {
            return i;
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

    while (true) {
        context.process = nextProcess(procesos, &context);
        while (context.process == NO_PROCESS_READY) {
            // Si no tenemos procesos listos para ejecutar, dejamos pasar un tick.
            context.time++;
            context.process = nextProcess(procesos, &context);
        }

        // Pasamos de no ejecutar nada a ejecutar algo
        if (context.prevProcess == NO_PROCESS_READY) {
            context.prevProcess = context.process;
            context.prevTime    = context.time;
            context.time++;
            continue;
        }

        // En este tick continuamos ejecutando el mismo proceso
        if (context.process == context.prevProcess) {
            Proceso* p = procesos_get(procesos, context.process);
            p->duracionRafaga--;
            context.time++;
            continue;
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
        context.time++;

        if (context.process == DONE_PROCESSING) {
            break;
        }
    }
}

void planificarPrioridades(procesos_t* procesos) {
    // Ordena inicialmente los procesos por prioridad original y tiempo de llegada
    qsort(procesos->ptr, procesos->size, sizeof(Proceso), compararPorPrioridad);
}
