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

void debugLog(char *msg, bool file){
	writeLog(msg, LDEBUG, file);
}

void infoLog(char *msg, bool file){
	writeLog(msg, LINFO, file);
}

void warningLog(char *msg, bool file){
	writeLog(msg, LWARNING, file);
}

void errorLog(char *msg, bool file){
	writeLog(ssprintf("%s. Error Code: %d. Error Description: %s", msg, errno, strerror(errno)), LERROR, file);
}

void criticalLog(char *msg, bool file){	
	writeLog(ssprintf("%s. Error Code: %d. Error Description: %s", msg, errno, strerror(errno)), LCRITICAL, file);
}

void writeLog(char* msg, LogType type, bool file){
	char* str = "LDEBUG";
	switch(type)
    {
		case LDEBUG: 
			str = "LDEBUG";
            fprintf(stdout, "%s | %s | %s | %s\n", getCurrentTimeStamp(), APPNAME, str, msg);
			break;
		case LINFO: 
			str = "LINFO";
            fprintf(stdout, "%s | %s | %s | %s\n", getCurrentTimeStamp(), APPNAME, str, msg);
			break;
		case LWARNING: 
			str = "LWARNING";
            fprintf(stdout, "%s | %s | %s | %s\n", getCurrentTimeStamp(), APPNAME, str, msg);
			break;
		case LERROR: 
			str = "LERROR";
            fprintf(stderr, "%s | %s | %s | %s\n", getCurrentTimeStamp(), APPNAME, str, msg);
			break;
		case LCRITICAL: 
			str = "LCRITICAL";
            fprintf(stderr, "%s | %s | %s | %s\n", getCurrentTimeStamp(), APPNAME, str, msg);
			break;
        default:
            str = "LDEBUG";
            fprintf(stdout, "%s | %s | %s | %s\n", getCurrentTimeStamp(), APPNAME, str, msg);
    }

    if(file == true){
        char* fileextension = "log";
        char* filepath = LOGFILEPATH;
        char* filejustname = getCurrentTimeStampForFileName();

        char* filename = ssprintf("%s%s.%s", filepath, filejustname, fileextension);

        FILE *fp = fopen(filename, "ab+");
        if(fp){
            debugC(ssprintf("File %s Opened", filename));
            fprintf(fp, "%s | %s | %s | %s\n", getCurrentTimeStamp(), APPNAME, str, msg);
        }
        else{
            errorC(ssprintf("Logging Error on file %s", filename));
        }
        
    }
}


char* getCurrentTimeStamp(){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	char *format = "%d-%02d-%02d %02d:%02d:%02d";	
	ssize_t bufsz = snprintf(NULL, 0, format, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	char* buf = malloc(bufsz + 1);
	snprintf(buf, bufsz + 1, format, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	
	return buf;
}

char* getCurrentTimeStampForFileName(){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	char *format = "%d-%02d-%02d";	
	ssize_t bufsz = snprintf(NULL, 0, format, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	char* buf = malloc(bufsz + 1);
	snprintf(buf, bufsz + 1, format, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	
	return buf;
}