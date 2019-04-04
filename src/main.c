#include <stdio.h>

#include "log.h"

int main(int argc, char* argv[]){
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    infoLog("Starting");
  

    infoLog("Completed!");

    return 0;
}