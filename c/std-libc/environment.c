#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;

int main(int argc, char *argv) {
    int i;
    char buf[1024] = {0};

    for (i = 0; environ[i] != NULL; i++) {
        printf("%s\n", environ[i]);
    }

    return 0;
}