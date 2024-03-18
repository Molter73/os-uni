#include <stdio.h>
#include <stdlib.h> // Para malloc, free y funciones relacionadas con archivos
#include "proceso.h"

int leerProcesosDesdeArchivoBinario(const char* nombreArchivo, Proceso** procesos) {
    FILE *archivo = fopen(nombreArchivo, "rb");
    if (!archivo) {
        perror("Error al abrir el archivo binario");
        return -1; // Cambiado de 0 a -1 para indicar error
    }

    fseek(archivo, 0, SEEK_END);
    long fileSize = ftell(archivo);
    rewind(archivo);

    int numProcesos = fileSize / sizeof(Proceso);
    if (numProcesos <= 0) { // Verificación adicional para el tamaño del archivo
        printf("El archivo está vacío o tiene un formato incorrecto.\n");
        fclose(archivo);
        return -2; // Nuevo código de error para archivo vacío o formato incorrecto
    }

    *procesos = (Proceso*)malloc(numProcesos * sizeof(Proceso));
    if (*procesos == NULL) {
        printf("No se pudo asignar memoria para los procesos.\n");
        fclose(archivo);
        return -3; // Nuevo código de error para falla en asignación de memoria
    }

    size_t leidos = fread(*procesos, sizeof(Proceso), numProcesos, archivo);
    if (leidos != numProcesos) {
        printf("Error al leer los datos del archivo.\n");
        free(*procesos);
        fclose(archivo);
        return -4; // Nuevo código de error para lectura incompleta
    }

    fclose(archivo);
    return numProcesos;
}

void guardarProcesosEnCSV(const char* nombreArchivo, Proceso* procesos, int numProcesos) {
    FILE *archivo = fopen(nombreArchivo, "w");
    if (!archivo) {
        perror("Error al abrir el archivo CSV");
        return;
    }

    fprintf(archivo, "ID Proceso,Tiempo de Llegada,Duración de la Ráfaga,Inicio de Ejecución,Fin de Ejecución\n");

    int tiempoActual = 0;
    for (int i = 0; i < numProcesos; i++) {
        if (tiempoActual < procesos[i].tiempoLlegada) {
            tiempoActual = procesos[i].tiempoLlegada;
        }
        int inicioEjecucion = tiempoActual;
        int finEjecucion = tiempoActual + procesos[i].duracionRafaga;
        tiempoActual = finEjecucion;

        fprintf(archivo, "%d,%d,%d,%d,%d\n", procesos[i].id, procesos[i].tiempoLlegada, procesos[i].duracionRafaga, inicioEjecucion, finEjecucion);
    }

    fclose(archivo);
}

