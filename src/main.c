#include <stdio.h>

#include "log.h"

int main(int argc, char* argv[]){
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    //infoF("Starting");S

    createPath(LOGFILEPATH);   

    //infoF("Completed!");

    return 0;
}