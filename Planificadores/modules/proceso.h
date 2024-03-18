#ifndef PROCESO_H
#define PROCESO_H

typedef struct {
    int id;
    int tiempoLlegada;
    int duracionRafaga;
    int prioridad;
} Proceso;


int leerProcesosDesdeArchivoBinario(const char* nombreArchivo, Proceso** procesos);
void guardarProcesosEnCSV(const char* nombreArchivo, Proceso* procesos, int numProcesos);

#endif