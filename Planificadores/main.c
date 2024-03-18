#include "modules/fcfs.h"
#include "modules/prioridad.h"
#include "modules/proceso.h"
#include "modules/rr.h"
#include "modules/sjf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Uso: %s <algoritmo> <archivo>\n", argv[0]);
        return 1;
    }

    Proceso* procesos = NULL;
    int numProcesos   = leerProcesosDesdeArchivoBinario(argv[2], &procesos);

    if (numProcesos <= 0) {
        printf("No se pudieron leer los procesos del archivo o el archivo está vacío.\n");
        return 1;
    }

    // Selecciona el algoritmo de planificación basado en el argumento de la línea de comandos
    if (strcmp(argv[1], "fcfs") == 0) {
        printf("Planificando usando First-Come, First-Served (FCFS)...\n");
        planificarFCFS(procesos, numProcesos);
        guardarProcesosEnCSV("./data/fcfs_procesos.csv", procesos, numProcesos);
    } else if (strcmp(argv[1], "sjf") == 0) {
        printf("Planificando usando Shortest Job First (SJF)...\n");
        planificarSJF(procesos, numProcesos);
        guardarProcesosEnCSV("./data/sjf_procesos.csv", procesos, numProcesos);
    } else if (strcmp(argv[1], "prioridad") == 0) {
        printf("Planificando usando Prioridades...\n");
        planificarPrioridades(procesos, numProcesos);
        guardarProcesosEnCSV("./data/prioridad_procesos.csv", procesos, numProcesos);
    } else if (strcmp(argv[1], "rr") == 0) {
        int quantum = 4;
        printf("Planificando usando Round Robin (RR)...\n");
        planificarPrioridades(procesos, numProcesos);
        planificarRR(procesos, numProcesos, quantum);
        guardarProcesosEnCSV("./data/prioridad_procesos.csv", procesos, numProcesos);
    } else {
        printf("Algoritmo no reconocido. Opciones válidas: fcfs, sjf\n");
        free(procesos); // Libera la memoria asignada para los procesos antes de salir
        return 1;
    }

    free(procesos); // Libera la memoria asignada para los procesos al finalizar correctamente
    return 0;
}
