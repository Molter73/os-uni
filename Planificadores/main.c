#include "modules/fcfs.h"
#include "modules/prioridad.h"
#include "modules/proceso.h"
#include "modules/rr.h"
#include "modules/sjf.h"

#include <assert.h>
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct options_s {
    const char* algorithm;
    const char* input;
    const char* output;
    bool preemptive;
} options_t;

void usage(char* prog) {
    printf("Uso: %s [FLAGS] <algoritmo> <archivo>\n\n", prog);
    printf("algoritmo: El algoritmo a utilizar. Uno de fcfs, sjf, prioridad o rr\n");
    printf("archivo: Archivo binario conteniendo objetos de tipo Proceso a ser leídos\n\n");
    printf("FLAGS:\n");
    printf("-h\tImprime este texto de ayuda\n");
    printf("-o\tDirectorio de salida para los archivos CSV generados. Default: './data'\n");
    printf("-p\tCuando se usa el algoritmo por prioridades, realiza ajuste por envejecimiento\n");
}

int run(const options_t* opts) {
    assert(opts != NULL);

    char output[PATH_MAX];
    procesos_t* procesos = leerProcesosDesdeArchivoBinario(opts->input);
    if (procesos == NULL) {
        fprintf(stderr, "Fallo al leer el archivo de entrada");
        return 1;
    }

    if (procesos->size <= 0) {
        fprintf(stderr, "No se pudieron leer los procesos del archivo o el archivo está vacío.\n");
        return 1;
    }

    if (opts->preemptive) {
        snprintf(output, PATH_MAX, "%s/%s_preemptive_procesos.csv", opts->output, opts->algorithm);
    } else {
        snprintf(output, PATH_MAX, "%s/%s_procesos.csv", opts->output, opts->algorithm);
    }

    // Selecciona el algoritmo de planificación basado en el argumento de la línea de comandos
    if (strcmp(opts->algorithm, "fcfs") == 0) {
        printf("Planificando usando First-Come, First-Served (FCFS)...\n");
        planificarFCFS(procesos);
        guardarProcesosEnCSV(output, procesos);
    } else if (strcmp(opts->algorithm, "sjf") == 0) {
        printf("Planificando usando Shortest Job First (SJF)...\n");
        planificarSJF(procesos);
        guardarProcesosEnCSV(output, procesos);
    } else if (strcmp(opts->algorithm, "prioridad") == 0) {
        if (opts->preemptive) {
            printf("Planificando usando Prioridades con prevaciado...\n");
            planificarPrioridadesPreemptive(procesos, output);
        } else {
            printf("Planificando usando Prioridades...\n");
            planificarPrioridades(procesos);
            guardarProcesosEnCSV(output, procesos);
        }
    } else if (strcmp(opts->algorithm, "rr") == 0) {
        int quantum = 4;
        printf("Planificando usando Round Robin (RR)...\n");
        planificarPrioridades(procesos);
        planificarRR(procesos, quantum, output);
    } else {
        fprintf(stderr, "opts->Algoritmo no reconocido.\n\n");
        procesos_free(procesos);
        return 1;
    }

    procesos_free(procesos);
    return 0;
}

int main(int argc, char* argv[]) {
    options_t opts = {
        .algorithm  = NULL,
        .output     = "./data",
        .input      = NULL,
        .preemptive = false,
    };
    int opt = -1;

    while ((opt = getopt(argc, argv, "ho:p")) != -1) {
        switch (opt) {
        case 'h':
            usage(argv[0]);
            return 0;
        case 'o':
            opts.output = optarg;
            break;
        case 'p':
            opts.preemptive = true;
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

    opts.algorithm = argv[optind];
    opts.input     = argv[optind + 1];

    return run(&opts);
}
