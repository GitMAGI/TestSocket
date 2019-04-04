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

void debugLog(char *msg){
	writeLog(msg, LDEBUG);
}

void infoLog(char *msg){
	writeLog(msg, LINFO);
}

void warningLog(char *msg){
	writeLog(msg, LWARNING);
}

void errorLog(char *msg){
	writeLog(ssprintf("%s. Error Code: %d. Error Description: %s", msg, errno, strerror(errno)), LERROR);
}

void criticalLog(char *msg){	
	writeLog(ssprintf("%s. Error Code: %d. Error Description: %s", msg, errno, strerror(errno)), LCRITICAL);
}

void writeLog(char* msg, LogType type){
	bool file = false;
	if(strlen(LOGFILEPATH) > 0)
		file = true;

	char* str = "DEBUG";
	switch(type)
    {
		case LDEBUG: 
			str = "DEBUG";
            fprintf(stdout, "%s | %s | %s | %s\n", getCurrentTimeStamp(), APPNAME, str, msg);
			break;
		case LINFO: 
			str = "INFO";
            fprintf(stdout, "%s | %s | %s | %s\n", getCurrentTimeStamp(), APPNAME, str, msg);
			break;
		case LWARNING: 
			str = "WARNING";
            fprintf(stdout, "%s | %s | %s | %s\n", getCurrentTimeStamp(), APPNAME, str, msg);
			break;
		case LERROR: 
			str = "ERROR";
            fprintf(stderr, "%s | %s | %s | %s\n", getCurrentTimeStamp(), APPNAME, str, msg);
			break;
		case LCRITICAL: 
			str = "CRITICAL";
            fprintf(stderr, "%s | %s | %s | %s\n", getCurrentTimeStamp(), APPNAME, str, msg);
			break;
        default:
            str = "DEBUG";
            fprintf(stdout, "%s | %s | %s | %s\n", getCurrentTimeStamp(), APPNAME, str, msg);
    }

    if(file == true){
        char* fileextension = "log";
        char* filepath = LOGFILEPATH;
        char* filejustname = getCurrentTimeStampForFileName();

        char* filename = ssprintf("%s%s.%s", filepath, filejustname, fileextension);
		if(!directoryExists(filepath))
			createPath(filepath);

        FILE *fp = fopen(filename, "ab+");
        if(fp){            
            fprintf(fp, "%s | %s | %s | %s\n", getCurrentTimeStamp(), APPNAME, str, msg);
        }
        else{
			fprintf(stderr, "Logging Error on file %s", filename);
        }
        
    }
}