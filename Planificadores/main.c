#include "modules/fcfs.h"
#include "modules/prioridad.h"
#include "modules/proceso.h"
#include "modules/rr.h"
#include "modules/sjf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void usage(char* prog) {
    printf("Uso: %s <algoritmo> <archivo>\n\n", prog);
    printf("algoritmo: El algoritmo a utilizar. Uno de fcfs, sjf, prioridad o rr\n");
    printf("archivo: Archivo binario conteniendo objetos de tipo Proceso a ser leídos\n");
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        usage(argv[0]);
        return 1;
    }

    procesos_t* procesos = leerProcesosDesdeArchivoBinario(argv[2]);
    if (procesos == NULL) {
        fprintf(stderr, "Fallo al leer el archivo de entrada");
        return 1;
    }

    if (procesos->size <= 0) {
        fprintf(stderr, "No se pudieron leer los procesos del archivo o el archivo está vacío.\n");
        return 1;
    }

    // Selecciona el algoritmo de planificación basado en el argumento de la línea de comandos
    if (strcmp(argv[1], "fcfs") == 0) {
        printf("Planificando usando First-Come, First-Served (FCFS)...\n");
        planificarFCFS(procesos);
        guardarProcesosEnCSV("./data/fcfs_procesos.csv", procesos);
    } else if (strcmp(argv[1], "sjf") == 0) {
        printf("Planificando usando Shortest Job First (SJF)...\n");
        planificarSJF(procesos);
        guardarProcesosEnCSV("./data/sjf_procesos.csv", procesos);
    } else if (strcmp(argv[1], "prioridad") == 0) {
        printf("Planificando usando Prioridades...\n");
        planificarPrioridades(procesos);
        guardarProcesosEnCSV("./data/prioridad_procesos.csv", procesos);
    } else if (strcmp(argv[1], "rr") == 0) {
        int quantum = 4;
        printf("Planificando usando Round Robin (RR)...\n");
        planificarPrioridades(procesos);
        planificarRR(procesos, quantum);
        guardarProcesosEnCSV("./data/prioridad_procesos.csv", procesos);
    } else {
        fprintf(stderr, "Algoritmo no reconocido.\n\n");
        usage(argv[0]);
        procesos_free(procesos);
        return 1;
    }

    procesos_free(procesos);
    return 0;
}
