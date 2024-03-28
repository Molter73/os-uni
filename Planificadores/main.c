#include "modules/fcfs.h"
#include "modules/prioridad.h"
#include "modules/proceso.h"
#include "modules/rr.h"
#include "modules/sjf.h"

#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void usage(char* prog) {
    printf("Uso: %s [FLAGS] <algoritmo> <archivo>\n\n", prog);
    printf("algoritmo: El algoritmo a utilizar. Uno de fcfs, sjf, prioridad o rr\n");
    printf("archivo: Archivo binario conteniendo objetos de tipo Proceso a ser leídos\n\n");
    printf("FLAGS:\n");
    printf("-h\tImprime este texto de ayuda\n");
    printf("-o\tDirectorio de salida para los archivos CSV generados. Default: './data'\n");
}

int run(const char* algorithm, const char* input, const char* outputDirectory) {
    char output[PATH_MAX];
    procesos_t* procesos = leerProcesosDesdeArchivoBinario(input);
    if (procesos == NULL) {
        fprintf(stderr, "Fallo al leer el archivo de entrada");
        return 1;
    }

    if (procesos->size <= 0) {
        fprintf(stderr, "No se pudieron leer los procesos del archivo o el archivo está vacío.\n");
        return 1;
    }

    snprintf(output, PATH_MAX, "%s/%s_procesos.csv", outputDirectory, algorithm);

    // Selecciona el algoritmo de planificación basado en el argumento de la línea de comandos
    if (strcmp(algorithm, "fcfs") == 0) {
        printf("Planificando usando First-Come, First-Served (FCFS)...\n");
        planificarFCFS(procesos);
        guardarProcesosEnCSV(output, procesos);
    } else if (strcmp(algorithm, "sjf") == 0) {
        printf("Planificando usando Shortest Job First (SJF)...\n");
        planificarSJF(procesos);
        guardarProcesosEnCSV(output, procesos);
    } else if (strcmp(algorithm, "prioridad") == 0) {
        printf("Planificando usando Prioridades...\n");
        planificarPrioridades(procesos);
        guardarProcesosEnCSV(output, procesos);
    } else if (strcmp(algorithm, "rr") == 0) {
        int quantum = 4;
        printf("Planificando usando Round Robin (RR)...\n");
        planificarPrioridades(procesos);
        planificarRR(procesos, quantum, output);
    } else {
        fprintf(stderr, "Algoritmo no reconocido.\n\n");
        procesos_free(procesos);
        return 1;
    }

    procesos_free(procesos);
    return 0;
}

int main(int argc, char* argv[]) {
    char* output    = "./data";
    char* input     = NULL;
    char* algorithm = NULL;
    int opt         = -1;

    while ((opt = getopt(argc, argv, "ho:")) != -1) {
        switch (opt) {
        case 'h':
            usage(argv[0]);
            return 0;
        case 'o':
            output = optarg;
            break;
        default:
            usage(argv[0]);
            return -1;
        }
    }

    if (argc - optind != 2) {
        fprintf(stderr, "Cantidad de parámetros insuficiente\n\n");
        usage(argv[0]);
        return -1;
    }

    algorithm = argv[optind];
    input     = argv[optind + 1];

    return run(algorithm, input, output);
}
