// defs.h
// Constantes y estructuras para la simulación de la gestión de memoria.

#ifndef DEFS_H
#define DEFS_H

// Tamaño de página estándar utilizado en la simulación.
#define PAGE_SIZE 4096

// Número de marcos de página disponibles en la memoria física.
#define NUM_FRAMES 100

// Tamaño total de la memoria física calculado en base a la cantidad y tamaño de los marcos de página.
#define MEMORY_SIZE (NUM_FRAMES * PAGE_SIZE)

// Estructura que representa una página de memoria con su ID, el ID del marco en el que se encuentra (si está en
// memoria) y un indicador de validez.
typedef struct {
    int page_id;  // Identificador único de la página.
    int frame_id; // ID del marco que contiene la página; -1 si la página no está en memoria.
    int valid;    // Indica si la página está cargada en memoria (1) o no (0).
} Page;

// Estructura que representa un marco de memoria física.
typedef struct {
    int frame_id; // Identificador único del marco de memoria.
    int page_id;  // ID de la página actualmente ocupando este marco; -1 si el marco está libre.
    int occupied; // Estado de ocupación del marco: 1 si está ocupado, 0 si está libre.
} Frame;

// Tabla de páginas que asocia páginas de memoria virtual a marcos de memoria física.
typedef struct {
    Page* pages;   // Array de páginas.
    int num_pages; // Número total de páginas en la tabla de páginas.
} PageTable;

// Estructura que representa una solicitud de página por un proceso.
typedef struct {
    int page_id;    // ID de la página que se está solicitando.
    int process_id; // ID del proceso que realiza la solicitud.
} PageRequest;

// Contenedor para las tablas de página de todos los procesos en simulación.
typedef struct {
    PageTable* tables; // Array de tablas de página, una por proceso.
    int num_processes; // Cantidad total de procesos simulados.
} ProcessPageTables;

#endif // DEFS_H
