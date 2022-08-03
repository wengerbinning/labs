#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "tools.h"
#include "develop.h"

// FIXME: malloc size need free.
int new_packet (void **packet, const char *path) {
    FILE *fp;
    int len;
    void *buf;

    fp = fopen(path, "rb");
    if (!fp) {
        error("Failed to open %s", path);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);

    buf = (char  *) malloc(len);
    
    fseek(fp, 0, SEEK_SET);
    fread(buf, len, 1, fp);

    fclose(fp);

    *packet = buf;

    return len;
}


void free_packet (char *packet) {
    free(packet);
}


int hexdump (void *ptr, int size) {
    int i;
    unsigned char *p;

    content("Print packet data(%dB): ", size);
    
    for (i = 0; i < size; i++) {
        p = (unsigned char *)(ptr + i);
        if ((i % 16) == 0) {
            content("\n0x%04X: %02X", i, *p);
        } else if ((i % 2) == 0) {
            content(" %02X", *p);
        } else {
            content("%02X", *p);
        }
    }
    info("");

    return 0;
}
