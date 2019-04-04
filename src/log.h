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

void debugLog(char *msg);
void infoLog(char *msg);
void warningLog(char *msg);
void errorLog(char *msg);
void criticalLog(char *msg);

void writeLog(char* msg, LogType type);

#endif