// main.c
#include "defs.h"
#include "simulation/log.h"
#include "simulation/simulation.h"
#include <assert.h>
#include <errno.h>
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
    printf(" -r\tCantidad de peticiones a realizar\n");
}

typedef struct {
    const char* prog_name;
    const char* method;
    QueueType queue_type;
    int num_accesses;
} opts_t;

int run(const opts_t* opts) {
    assert(opts != NULL);
    TestConfiguration config = {
        .num_processes = 20,
        .num_pages     = MAX_PAGES,
        .num_accesses  = opts->num_accesses,
        .queue_type    = opts->queue_type,
    };
    // Determina qué prueba ejecutar basado en el argumento
    if (strcmp(opts->method, "random") == 0) {
        printf("Iniciando prueba de Acceso Aleatorio...\n");
        testRandomAccess(&config); // Test con acceso aleatorio de páginas
    } else if (strcmp(opts->method, "locality") == 0) {
        printf("Iniciando prueba de Localidad Temporal...\n");
        // Test de localidad temporal con 1000 accesos, 256 páginas, 3 procesos
        testTemporalLocality(&config);
    } else if (strcmp(opts->method, "thrashing") == 0) {
        printf("Iniciando prueba de Thrashing...\n");
        testThrashing(&config); // Test de thrashing con 3 procesos
    } else {
        fprintf(stderr, "Prueba '%s' no reconocida.\n\n", opts->method);
        usage(opts->prog_name);
        return -1;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    static const int DEFAULT_REQUESTS = 1000;

    int res     = -1;
    opts_t opts = {
        .prog_name    = argv[0],
        .method       = NULL,
        .queue_type   = FIFO,
        .num_accesses = DEFAULT_REQUESTS,
    };
    int opt = -1;

    while ((opt = getopt(argc, argv, "q:r:")) != -1) {
        switch (opt) {
        case 'q':
            if (strcasecmp("FIFO", optarg) == 0) {
                opts.queue_type = FIFO;
            } else if (strcasecmp("LRU", optarg) == 0) {
                opts.queue_type = LRU;
            } else {
                fprintf(stderr, "Tipo de cola desconocido: %s\n", optarg);
                usage(argv[0]);
                return -1;
            }
            break;

        case 'r':
            opts.num_accesses = atoi(optarg);
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

    opts.method = argv[optind];

    if (logOpen() != 0) {
        fprintf(stderr, "Fallo al abrir log: (%d) %s\n", errno, strerror(errno));
        return -1;
    }
    srand(time(NULL)); // Inicializa la semilla para la generación de números aleatorios

    res = run(&opts);

    logClose();
    return res;
}
