#include "prioridad.h"
#include "proceso.h"
#include <stdlib.h> // Para qsort

// Función de comparación inicial por prioridad y tiempo de llegada
int compararPorPrioridad(const void* a, const void* b) {
    const Proceso* proc_a = (const Proceso*)a;
    const Proceso* proc_b = (const Proceso*)b;

    int prioridad = proc_a->prioridad - proc_b->prioridad;

    if (prioridad == 0) {
        return proc_a->tiempoLlegada - proc_b->tiempoLlegada;
    }

    return prioridad;
}

// Ajusta las prioridades de los procesos basándose en el envejecimiento
void ajustarPrioridadesPorEnvejecimiento(procesos_t* procesos) {
    // Función opcional para modificar las prioridades basadas en el tiempo de espera, mitigando el hambre.
}

void planificarPrioridades(procesos_t* procesos) {
    // Ordena inicialmente los procesos por prioridad original y tiempo de llegada
    qsort(procesos->ptr, procesos->size, sizeof(Proceso), compararPorPrioridad);

    // Ajusta las prioridades basadas en el envejecimiento
    ajustarPrioridadesPorEnvejecimiento(procesos);
    // Opcionalmente, vuelve a ordenar los procesos si el ajuste de prioridades puede alterar el orden de ejecución
}
