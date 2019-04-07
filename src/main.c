#include <stdio.h>
#include <time.h>

#if defined _WIN64 || defined _WIN32
    #include <winsock2.h>
    #include <windows.h>
    #include <ws2tcpip.h>
    #include <strings.h>
#else
    #include <string.h>
#endif

#include "log.h"
#include "util.h"

#include "socket01.h"
#include "socket02.h"
#include "socket03.h"

int main(int argc, char* argv[]){
    struct timeval st, et;
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    getTick(&st);
    infoLog("Starting");
  
    //socket01();
    //socket02();
    socket03();

    getTick(&et);
    infoLog(ssprintf("Completed. ETA %s", getETA(st, et)));

    return 0;
}