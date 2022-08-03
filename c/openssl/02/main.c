#include <string.h>

#include "develop.h"

#include "hexdump.h"
#include "provision.h"

int main (int argc, char *argv[]) {
    FILE *ifile;
    size_t flen;

    image_head_t head;

    if (!argv[1])
        return 0;

    if (!(ifile = fopen(argv[1], "rb+"))) {
        printf("No File!\n");
        return 0;
    }

    fseek(ifile, 0, SEEK_END);
    flen = ftell(ifile);
    fseek(ifile, 0, SEEK_SET);

    memset(&head, 0, sizeof(head));
    fread(&head, sizeof(head), 1, ifile);
    debug("%s head %d Bytes", argv[1], sizeof(head));
    hexdump((void *)&head, sizeof(head));

    prov_func(&head, sizeof(head));

    fclose(ifile);
    return 0;
}