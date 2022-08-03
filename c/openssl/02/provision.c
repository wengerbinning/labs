#include "develop.h"

#include "hexdump.h"




#define KEY   "37d6ae8bc920374649426438bde35493"


#define BUFIZE 64

int prov_func (void *data, size_t *dlen) {
    int i;
    unsigned char byte;
    unsigned int word;
    unsigned int buffer[BUFIZE + 1];
    unsigned int iv;

    memset(buffer, 0, sizeof(buffer));

    unsigned char *sptr = (unsigned char *)data;
    unsigned char *cptr = (unsigned char *)data;

    for (i = 0; i < 0x10; i++) {
        byte = *((unsigned char *)data + (i << 1));
        word = byte << 8;
        word += *((unsigned char *)data + (i << 1) + 1);

        buffer[i] = word;
        debug("data offset %02X,%02X", i << 1, (i << 1) + 1);
    }

    debug("buffer: ");
    hexdump(buffer, BUFIZE);

    return 0;
}
