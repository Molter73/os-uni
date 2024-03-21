#include "proceso.h"

#include <assert.h>
#include <stdio.h>

procesos_t* procesos_new(size_t numProcesos) {
    procesos_t* procesos = (procesos_t*)malloc(sizeof(procesos_t));
    if (procesos == NULL) {
        return NULL;
    }

    procesos->ptr = calloc(numProcesos, sizeof(Proceso));
    if (procesos->ptr == NULL) {
        free(procesos);
        return NULL;
    }

    procesos->size = numProcesos;
    return procesos;
}

void procesos_free(procesos_t* p) {
    if (p != NULL) {
        free(p->ptr);
        free(p);
    }
}

bool procesos_is_valid(const procesos_t* p) {
    return p != NULL && p->ptr != NULL && p->size > 0;
}

Proceso* procesos_get(const procesos_t* p, size_t i) {
    assert(procesos_is_valid(p));

    if (i < p->size) {
        return &p->ptr[i];
    }
    return NULL;
}

procesos_t* leerProcesosDesdeArchivoBinario(const char* nombreArchivo) {
    procesos_t* procesos = NULL;
    FILE* archivo        = fopen(nombreArchivo, "rb");

    if (!archivo) {
        perror("Error al abrir el archivo binario");
        return NULL;
    }

    fseek(archivo, 0, SEEK_END);
    long fileSize = ftell(archivo);
    rewind(archivo);

    int numProcesos = fileSize / sizeof(Proceso);
    if (numProcesos <= 0) { // Verificación adicional para el tamaño del archivo
        fprintf(stderr, "El archivo está vacío o tiene un formato incorrecto.\n");
        goto error;
    }

    procesos = procesos_new(numProcesos);
    if (procesos == NULL) {
        fprintf(stderr, "No se pudo asignar memoria para los procesos.\n");
        goto error;
    }

    size_t leidos = fread(procesos->ptr, sizeof(Proceso), numProcesos, archivo);
    if (leidos != numProcesos) {
        fprintf(stderr, "Error al leer los datos del archivo.\n");
        goto error;
    }

    fclose(archivo);
    return procesos;

error:
    procesos_free(procesos);
    fclose(archivo);
    return NULL;
}

void guardarProcesosEnCSV(const char* nombreArchivo, procesos_t* procesos) {
    FILE* archivo = fopen(nombreArchivo, "w");
    if (!archivo) {
        perror("Error al abrir el archivo CSV");
        return;
    }

    fprintf(archivo, "ID Proceso,Tiempo de Llegada,Duración de la Ráfaga,Inicio de Ejecución,Fin de Ejecución\n");

    int tiempoActual = 0;
    size_t i         = 0;
    Proceso* p       = procesos_get(procesos, i);
    for (; p != NULL; p = procesos_get(procesos, ++i)) {
        if (tiempoActual < p->tiempoLlegada) {
            tiempoActual = p->tiempoLlegada;
        }
        int inicioEjecucion = tiempoActual;
        int finEjecucion    = tiempoActual + p->duracionRafaga;
        tiempoActual        = finEjecucion;

        fprintf(archivo, "%d,%d,%d,%d,%d\n", p->id, p->tiempoLlegada, p->duracionRafaga, inicioEjecucion, finEjecucion);
    }

    fclose(archivo);
}
