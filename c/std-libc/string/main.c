#include <stdio.h>

#include "strlib.h"
#include "hexdump.h"







int main (int argc, char *argv[]) {
    char *str;

    str = argv[1];
    printf("ORIGIN: %s\n", str);
    STR2UPPER(str);
    printf("UPPERS: %s\n", str);
    STR2LOWER(str);
    printf("LOWERS: %s\n", str);

if ('/' == *(str + strlen(str) - 1)) {
        *(str + strlen(str) - 1) = 0x0;
    }

    hexdump(str, strlen(str));




    hexdump(str + strlen(str), 1);

    return 0;
}




