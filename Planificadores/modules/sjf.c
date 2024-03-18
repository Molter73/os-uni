#include "sjf.h"
#include <stdlib.h>
#include <stdio.h> // Para fprintf

int compararPorDuracionRafaga(const void *a, const void *b) {
    // Implemetar la lógica de la función
}

void planificarSJF(Proceso* procesos, int numProcesos) {
    // Validación de los argumentos y ordenar después por su tiempo de llegada (se puede utilizar qsort)
    if (procesos == NULL || numProcesos <= 0) {
        fprintf(stderr, "Error: Argumentos no válidos para planificarSJF.\n");
        return;
    }
    qsort(procesos, numProcesos, sizeof(Proceso), compararPorDuracionRafaga);
}
