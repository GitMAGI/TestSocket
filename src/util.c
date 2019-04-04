#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdarg.h>
#include <errno.h>
#include <assert.h>
#include <sys/stat.h>
#include <time.h>

#include "util.h"

#if defined _WIN64 || defined _WIN32
    #include <windows.h>
    #include <strings.h>
    #include <stddef.h>
#else
    #include <string.h>
#endif

char* ssprintf(char* format, ...){
    va_list argptr;

    if(argptr == NULL)
        return format;

    va_start(argptr, format);
    ssize_t bufsz = vsnprintf(NULL, 0, format, argptr);
	char* buf = malloc(bufsz + 1);
    vsnprintf(buf, bufsz + 1, format, argptr);
    va_end(argptr);

    return buf;
}

char* sstrcat(char* first, char* second){
    //fprintf(stdout, "Data to concat %s > %s\n", first, second);
    ssize_t bufsz = snprintf(NULL, 0, "%s%s", first, second);
	char* buf = malloc(bufsz + 1);
    snprintf(buf, bufsz + 1, "%s%s", first, second);
    //fprintf(stdout, "Concat Result %s\n", buf);

    return buf;
}

bool directoryExists(char* path){
    //fprintf(stdout, "Checking if path: %s exists\n", path);

    bool res = false;

    struct stat s;
    int err = stat(path, &s);

    if(-1 == err) {
        if(ENOENT == errno) {
            //fprintf(stdout, "Path: %s doesn't exist\n", path);
            res = false;
        } else {
            //fprintf(stderr, "Error during Directory %s checking\n", path);
            res = false;
        }
    } else {
        if(S_ISDIR(s.st_mode)) {
            //fprintf(stdout, "Path: %s exists\n", path);
            res = true;
        } else {
            //fprintf(stdout, "Path: %s is not a directory\n", path);
            res = false;
        }
    }

    return res;
}

int createPath(char* filepath){
    int dir = -1;

    //fprintf(stdout, "Examining path: %s\n", filepath);

    if (!directoryExists(filepath))
    {
        //fprintf(stdout, "Creating path: %s\n", filepath);

        #if defined _WIN64 || defined _WIN32
            char sep = '\\';
            char *currentpath = "";

            unsigned int start = 0, stop, count = 0;
            for (stop = 0; stop < strlen(filepath); stop++) {
                //fprintf(stdout, "Current char %c (pos %d)\n", filepath[stop], stop);      

                if(filepath[stop] == sep && stop == start){
                    currentpath = (char*) malloc(2*sizeof(char));
                    currentpath[0] = '\\';
                    currentpath[1] = '\0';
                    start = stop + 1;
                    continue;
                }          

                if ((filepath[stop] == sep && stop > start) || stop == strlen(filepath) - 1) {

                    char *token = (char*)malloc(sizeof(char) * (stop - start));
                    memcpy(token, &filepath[start], stop - start);
                    token[stop - start] = '\0';
                    
                    //fprintf(stdout, "start:%d stop:%d \n", start, stop);

                    //fprintf(stdout, "Concatenazione %s%s \n", currentpath, token);
                    currentpath = sstrcat(currentpath, token);
                    char * _sep = (char*)malloc(2*sizeof(char));
                    _sep[0] = sep;
                    _sep[1] = '\0';
                    //fprintf(stdout, "Concatenazione %s%s \n", currentpath, _sep);
                    currentpath = sstrcat(currentpath, _sep);

                    //fprintf(stdout, "Current path %s \n", currentpath);

                    if (!directoryExists(currentpath)){
                        if(CreateDirectory(currentpath, NULL))
                        {
                            dir = 0;
                            //fprintf(stdout, "Directory %s created\n", currentpath);
                        }
                        else{
                            dir = -1;
                            fprintf(stderr, "Error code: %d, Error description: %s\n", errno, strerror(errno));
                            break;
                        }
                    }

                    start = stop + 1;
                }
            }
        #else
            dir = mkdir(filepath, 0755);
            if(dir < 0)
                fprintf(stderr, "Error code: %d, Error description: %s\n", errno, strerror(errno));
            else
                //fprintf(stdout, "Directory %s created\n", filepath);
        #endif
    }
    else
        //fprintf(stdout, "Path: %s exists already\n", filepath);

    return dir;
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