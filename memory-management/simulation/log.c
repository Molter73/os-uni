// log.c
#include "log.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

void logEvent(const char* message) {
    FILE* logFile = fopen("simulation_log.txt", "a");
    if (logFile == NULL) {
        perror("Error opening log file");
        return;
    }
    time_t now = 0;
    time(&now);
    char* datetime = ctime(&now);
    datetime[strlen(datetime)-1] = '\0'; // Remove newline
    fprintf(logFile, "%s: %s\n", datetime, message);
    fclose(logFile);
}

void clearLogFiles() {
    // Abre y cierra inmediatamente los archivos en modo "w" para borrarlos
    FILE *logFile = fopen("simulation_log.txt", "w");
    if (logFile != NULL) {
        fclose(logFile);
    } else {
        perror("Error clearing log file");
    }

    FILE *memoryFile = fopen("memory_details.txt", "w");
    if (memoryFile != NULL) {
        fclose(memoryFile);
    } else {
        perror("Error clearing memory details file");
    }
}
