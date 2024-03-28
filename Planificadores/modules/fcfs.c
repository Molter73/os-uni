#include "fcfs.h"
#include <stdio.h>  // Para fprintf en caso de error
#include <stdlib.h> // Para qsort

// Función de comparación para ordenar procesos por tiempo de llegada
int compararPorTiempoLlegada(const void* a, const void* b) { // NOLINT
    const Proceso* proc_a = (const Proceso*)a;
    const Proceso* proc_b = (const Proceso*)b;

    return proc_a->tiempoLlegada - proc_b->tiempoLlegada;
}

// Función para planificar procesos usando el algoritmo First-Come, First-Served (FCFS)
void planificarFCFS(procesos_t* procesos) {
    // Validación de los argumentos y ordenar después por su tiempo de llegada (se puede utilizar qsort)
    if (!procesos_is_valid(procesos)) {
        fprintf(stderr, "Error: Argumentos no válidos para planificarSJF.\n");
        return;
    }
    qsort(procesos->ptr, procesos->size, sizeof(Proceso), compararPorTiempoLlegada);
    /*
    Después de esta llamada a qsort, los procesos están ordenados según el algoritmo FCFS, listos para ser procesados en
    este orden.
    */
}
