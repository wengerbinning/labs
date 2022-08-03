#include <stdio.h>

#include "debug.h"

struct block {
    unsigned char *data;
    int len;

} packed;



int main(int argc, char **argv) {
    struct block block[10];

    debug(DEBUG_LEVEL_A, "run in %s", __func__);
    
    info(INFO_LEVEL_A, "block has %d element, per element size %d, total size%d",(block), sizeof(struct block), sizeof(block));

    return 0;
}
