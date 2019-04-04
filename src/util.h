#ifndef UTIL_H
#define UTIL_H

#include <stdarg.h>

char* ssprintf(char* format, ...);
char* sstrcat(char* first, char* second);
bool directoryExists(char* path);
int createPath(char* filepath);

char* getCurrentTimeStamp();
char* getCurrentTimeStampForFileName();

#endif