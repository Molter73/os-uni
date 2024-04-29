// main.c
#include "simulation/log.h"
#include "simulation/simulation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include <unistd.h>

void usage(const char* prog) {
    printf("Uso: %s <FLAGS> [random|locality|thrashing]\n\n", prog);
    printf("FLAGS:\n");
    printf(" -q\tTipo de cola a utilizar, FIFO o LRU. Default: FIFO\n");
}

int main(int argc, char* argv[]) {
    int res = -1;
    int opt = -1;

    QueueType queue_type = FIFO;
    while ((opt = getopt(argc, argv, "q:")) != -1) {
        switch (opt) {
        case 'q':
            if (strcasecmp("FIFO", optarg) == 0) {
                queue_type = FIFO;
            } else if (strcasecmp("LRU", optarg) == 0) {
                queue_type = LRU;
            } else {
                fprintf(stderr, "Tipo de cola desconocido: %s\n", optarg);
                usage(argv[0]);
                return -1;
            }
            break;
        default:
            fprintf(stderr, "Flag desconocido\n");
            usage(argv[0]);
            return -1;
        }
    }

    if (argc - optind != 1) {
        fprintf(stderr, "Número de parámetros incorrecto\n\n");
        usage(argv[0]);
        return res;
    }

    if (logOpen() != 0) {
        return -1;
    }
    srand(time(NULL)); // Inicializa la semilla para la generación de números aleatorios

    // Determina qué prueba ejecutar basado en el argumento
    if (strcmp(argv[optind], "random") == 0) {
        printf("Iniciando prueba de Acceso Aleatorio...\n");
        testRandomAccess(400, queue_type); // NOLINT Test con acceso aleatorio de páginas
    } else if (strcmp(argv[optind], "locality") == 0) {
        printf("Iniciando prueba de Localidad Temporal...\n");
        // Test de localidad temporal con 1000 accesos, 256 páginas, 3 procesos
        // NOLINTNEXTLINE
        testTemporalLocality(256, 200, 3, queue_type);
    } else if (strcmp(argv[optind], "thrashing") == 0) {
        printf("Iniciando prueba de Thrashing...\n");
        testThrashing(3, queue_type); // Test de thrashing con 3 procesos
    } else {
        fprintf(stderr, "Prueba '%s' no reconocida.\n\n", argv[1]);
        usage(argv[0]);
        goto cleanup;
    }

    res = 0;

cleanup:
    logClose();
    return res;
}
