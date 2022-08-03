#include <stdio.h>



int trace(char *name, char *msg) {
    printf("[%s] - %s\n", name, msg);
    return 0;
}

