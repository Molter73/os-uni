// log.c
#include "log.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

static FILE* log = NULL; // NOLINT

int logOpen() {
    log = fopen("simulation_log.txt", "w");
    if (log == NULL) {
        perror("Error opening log file");
        return -1;
    }
    return 0;
}

void logClose() {
    if (log != NULL) {
        fclose(log);
    }
}

void logEvent(const char* fmt, ...) {
    va_list args;
    time_t now = 0;
    time(&now);
    char* datetime = ctime(&now);

    datetime[strlen(datetime) - 1] = '\0'; // Remove newline

    fprintf(log, "%s: ", datetime);
    va_start(args, fmt);
    vfprintf(log, fmt, args);
    va_end(args);
    fprintf(log, "\n");
}
