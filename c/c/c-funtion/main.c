#include <stdio.h>

#include "debug.h"
#include "functions.h"

int main(int argc, char * argv[]) {
    int a = 1, b = 2;

    debug(DEBUG_LEVEL_A, "run in %s", __func__);
    
    printf("a + b = %d\n", sum_int(a,b));

    debug(DEBUG_LEVEL_A, "return from %s", __func__);
    return 0;
}
