#include <stdio.h>
#include <time.h>

#include "log.h"
#include "util.h"

int main(int argc, char* argv[]){
    struct timeval st, et;
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    getTick(&st);
    infoLog("Starting");
  
    for(int i=0; i < 700004000; i++){
        continue;
    }

    getTick(&et);
    infoLog(ssprintf("Completed. ETA %s", getETA(st, et)));

    return 0;
}