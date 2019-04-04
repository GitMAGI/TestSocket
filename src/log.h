#ifndef LOG_H
#define LOG_H

#include <stdbool.h>
#include <stdarg.h>
#include <config.h>

typedef enum 
{
	LDEBUG = 0, 
	LINFO = 1,  
	LWARNING = 2,
	LERROR = 3,
	LCRITICAL = 4
} LogType;

char* getCurrentTimeStamp();
char* getCurrentTimeStampForFileName();

void debugLog(char *msg, bool file);
void infoLog(char *msg, bool file);
void warningLog(char *msg, bool file);
void errorLog(char *msg, bool file);
void criticalLog(char *msg, bool file);

void writeLog(char* msg, LogType type, bool file);

#endif