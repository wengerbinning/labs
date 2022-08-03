#include <stdio.h>

#include "hexdump.h"

#define BUFIZE 256
#define CHECKSUM 2



#define LWORD(buf) (buf[0])
#define HWORD(buf) (buf[1])

int main (int argc, char *argv[]) {
    size_t rlen, ofst, size, offset;
    FILE *file;
    char *buf, buffer[BUFIZE + 1];
    size_t checksum = 0;

	if (!argv[1])
		return 0;

    if ( !(file = fopen(argv[1], "rb"))) {
       return 0;
    }

    offset = 0;
    while (0 < (rlen = fread(buffer, 1, BUFIZE, file))) {
        unsigned short word;
        for (int i = 0; i < (rlen / CHECKSUM); i++) {
            buf = buffer + (i * CHECKSUM);
            word = ((LWORD(buf) << 9                ) | (HWORD(buf) << 8));
            offset += CHECKSUM;
            checksum += word;
        }

        if (rlen % CHECKSUM) {
            size = rlen % CHECKSUM;
            ofst = (rlen / CHECKSUM) * CHECKSUM;
            buf = buffer + ofst;

        }
    }

    printf("CHECKSUM: 0x%08X, %d\n", checksum, offset);
    return 0;
}