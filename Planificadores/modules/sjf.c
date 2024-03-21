#include "sjf.h"
#include <stdio.h> // Para fprintf
#include <stdlib.h>

int compararPorDuracionRafaga(const void* a, const void* b) {
    const Proceso* proc_a = (const Proceso*)a;
    const Proceso* proc_b = (const Proceso*)b;

    return proc_a->duracionRafaga - proc_b->duracionRafaga;
}

void planificarSJF(procesos_t* procesos) {
    // Validación de los argumentos y ordenar después por su tiempo de llegada (se puede utilizar qsort)
    if (!procesos_is_valid(procesos)) {
        fprintf(stderr, "Error: Argumentos no válidos para planificarSJF.\n");
        return;
    }
    qsort(procesos->ptr, procesos->size, sizeof(Proceso), compararPorDuracionRafaga);
}
