#include "prioridad.h"
#include <stdio.h>
#include <stdlib.h> // Para qsort

// Función de comparación inicial por prioridad y tiempo de llegada
int compararPorPrioridad(const void *a, const void *b) {
    const Proceso* proc_a = (const Proceso*)a;
    const Proceso* proc_b = (const Proceso*)b;

    unsigned int prioridad = proc_a->prioridad - proc_b->prioridad;

    if (prioridad == 0) {
        return proc_a->tiempoLlegada - proc_b->tiempoLlegada;
    }

    return prioridad;
}

// Ajusta las prioridades de los procesos basándose en el envejecimiento
void ajustarPrioridadesPorEnvejecimiento(Proceso* procesos, int numProcesos) {
    // Función opcional para modificar las prioridades basadas en el tiempo de espera, mitigando el hambre.
}

void planificarPrioridades(Proceso* procesos, int numProcesos) {
    // Ordena inicialmente los procesos por prioridad original y tiempo de llegada
    qsort(procesos, numProcesos, sizeof(Proceso), compararPorPrioridad);

    // Ajusta las prioridades basadas en el envejecimiento
    ajustarPrioridadesPorEnvejecimiento(procesos, numProcesos);
/*
    // Opcionalmente, vuelve a ordenar los procesos si el ajuste de prioridades puede alterar el orden de ejecución
*/
}
