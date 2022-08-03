#include <stdio.h>


#include "flags.h"


unsigned int flags0 = 0;
unsigned int flags1 = TYPE2FLAGS(defw);

int main (int argc, char *argv[]) {
    unsigned int flags;

    flags = 0x00020203;

    printf("Flags: %08X, flags size: %d\n", flags, sizeof(test_flags_t));

    printf("Attr: %08X, Tpye: %08x\n", FLAGS_GET_ATTR(&flags), FLAGS_GET_TYPE(&flags));
    FLAGS_CLEAN_ATTR(&flags, 0x1);




    FLAGS_SET_TYPE(&flags, 0xFF);
    printf("Flags: %08X, type: %08X\n", flags, ((flags_t *)&flags)->type);
}