// log.h
#ifndef LOG_H
#define LOG_H

int logOpen();
void logClose();
void logEvent(const char* fmt, ...);
void clearLogFiles();

#endif // LOG_H
