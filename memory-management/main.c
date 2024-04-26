//main.c
#include "simulation/simulation.h"
#include "simulation/log.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int res = -1;
    if (argc != 2) {
        fprintf(stderr, "Uso: %s [random|locality|thrashing]\n", argv[0]);
        return res;
    }

    if (logOpen() != 0) {
        return -1;
    }
    srand(time(NULL));  // Inicializa la semilla para la generación de números aleatorios

    // Determina qué prueba ejecutar basado en el argumento
    if (strcmp(argv[1], "random") == 0) {
        printf("Iniciando prueba de Acceso Aleatorio...\n");
        testRandomAccess(400);  // Test con acceso aleatorio de páginas
    } else if (strcmp(argv[1], "locality") == 0) {
        printf("Iniciando prueba de Localidad Temporal...\n");
        testTemporalLocality(256, 200, 3);  // Test de localidad temporal con 1000 accesos, 256 páginas, 3 procesos
    } else if (strcmp(argv[1], "thrashing") == 0) {
        printf("Iniciando prueba de Thrashing...\n");
        testThrashing(3);  // Test de thrashing con 3 procesos
    } else {
        fprintf(stderr, "Prueba '%s' no reconocida. Las pruebas disponibles son: random, sequential, locality, thrashing.\n", argv[1]);
        goto cleanup;
    }

    res = 0;

cleanup:
    //logClose();
    return res;
}
