#include <stdio.h>

#include "hexdump.h"

#define TMP_FILE "./temp.txt"

#define BUFIZE 512

int main (int argc, const char *argv[]) {
    FILE *file = NULL;
    int rlen;
    char buffer[BUFIZE + 1];
    char *buf;

    file = fopen(TMP_FILE, "r");
    if ( NULL == file) {
        return -1;
    }

    buf = malloc(BUFIZE + 1);

    rlen = fread(buf, BUFIZE, 1, file);

    printf("read %d bytes: \n%s\n", rlen, buf);
    hexdump(buf, BUFIZE);

    fclose(file);
    return 0;
}

