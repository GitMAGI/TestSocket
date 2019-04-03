#include <stdio.h>

#define APPNAME "TestSocket"

int main(int argc, char* argv[]){
    setbuf(stdout, NULL);
    setbuf(stderr, NULL);

    fprintf(stdout, "Starting %s ...\n\n", APPNAME);


    

    fprintf(stdout, "%s completed!\n\n", APPNAME);

    return 0;
}