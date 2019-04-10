#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#if defined _WIN64 || defined _WIN32
    #include <strings.h>
    #include <windows.h>
#else
	#include <string.h>
#endif

#include "log.h"
#include "util.h"
#include "config.h"

void debugLog_(const char* caller, char* format, ...){
	char *msg;
	va_list argptr;
    if(argptr == NULL)
        msg = format;
    va_start(argptr, format);
    ssize_t bufsz = vsnprintf(NULL, 0, format, argptr);
	msg = malloc(bufsz + 1);
    vsnprintf(msg, bufsz + 1, format, argptr);
    va_end(argptr);

	writeLog(msg, caller, LDEBUG);
}

void infoLog_(const char* caller, char* format, ...){
	char *msg;
	va_list argptr;
    if(argptr == NULL)
        msg = format;
    va_start(argptr, format);
    ssize_t bufsz = vsnprintf(NULL, 0, format, argptr);
	msg = malloc(bufsz + 1);
    vsnprintf(msg, bufsz + 1, format, argptr);
    va_end(argptr);
	
	writeLog(msg, caller, LINFO);
}

void warningLog_(const char* caller, char* format, ...){
	char *msg;
	va_list argptr;
    if(argptr == NULL)
        msg = format;
    va_start(argptr, format);
    ssize_t bufsz = vsnprintf(NULL, 0, format, argptr);
	msg = malloc(bufsz + 1);
    vsnprintf(msg, bufsz + 1, format, argptr);
    va_end(argptr);
	
	writeLog(msg, caller, LWARNING);
}

void errorLog_(const char* caller, char* format, ...){
	char *msg;
	va_list argptr;
    if(argptr == NULL)
        msg = format;
    va_start(argptr, format);
    ssize_t bufsz = vsnprintf(NULL, 0, format, argptr);
	msg = malloc(bufsz + 1);
    vsnprintf(msg, bufsz + 1, format, argptr);
    va_end(argptr);

	writeLog(ssprintf("%s. Error Code: %d. Error Description: %s", msg, errno, strerror(errno)), caller, LERROR);
}

void criticalLog_(const char* caller, char* format, ...){
	char *msg;
	va_list argptr;
    if(argptr == NULL)
        msg = format;
    va_start(argptr, format);
    ssize_t bufsz = vsnprintf(NULL, 0, format, argptr);
	msg = malloc(bufsz + 1);
    vsnprintf(msg, bufsz + 1, format, argptr);
    va_end(argptr);

	writeLog(ssprintf("%s. Error Code: %d. Error Description: %s", msg, errno, strerror(errno)), caller, LCRITICAL);
}

void writeLog(char* msg, const char* scope, LogType type){
	bool file = false;
	if(strlen(LOGFILEPATH) > 0)
		file = true;

	char* str = "DEBUG";
	switch(type)
    {
		case LDEBUG: 
			str = "DEBUG";        
			break;
		case LINFO: 
			str = "INFO";
			break;
		case LWARNING: 
			str = "WARNING";
			break;
		case LERROR: 
			str = "ERROR";
			break;
		case LCRITICAL: 
			str = "CRITICAL";
			break;
        default:
            str = "DEBUG";
    }

    char* toLog = toLog = ssprintf("%s | %s | %s | %s() >>> %s\n", getCurrentTimeStamp(), str, APPNAME, scope, msg);
    fprintf(stdout, toLog);

    if(file == true){
        char* fileextension = "log";
        char* filepath = LOGFILEPATH;
        char* filejustname = getCurrentTimeStampForFileName();

        char* filename = ssprintf("%s%s.%s", filepath, filejustname, fileextension);
		if(!directoryExists(filepath))
			createPath(filepath);

        FILE *fp = fopen(filename, "ab+");
        if(fp){            
            fprintf(fp, toLog);
        }
        else{
			fprintf(stderr, "Logging Error on file %s", filename);
        }
        
    }
}