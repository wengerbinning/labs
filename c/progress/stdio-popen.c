#include <stdio.h>



int main(int argc, char *argv[]) {
    FILE * file;
    char bufer[64];
    int number;

    file = popen("ls -l | wc -l", "r");
    if ( file == NULL ) {
        return -1;
    }

    fscanf(file, "%d", &number);
    
    pclose(file);


    printf("Has %d number\n", number);

    return 0;
}