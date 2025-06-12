
#include <stdio.h>
#include "test.h"



struct url {
    union url_flags flags;
};


int main (int argc, char *argv[]) {
    struct url url;



    printf("%08X\n", URLF_FAMIL_FIRMWARE);



    return 0;
}