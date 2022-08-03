#include <stdio.h>

#include "debug.h"

int main(int agrc, char **argv) {
    int value;
    
    debug(DEBUG_LEVEL_A, "Compile from %s, %s %s", __FILE__, __DATE__, __TIME__);
    debug(DEBUG_LEVEL_A, "run in %s", __func__);
    value = 'b';
    switch (value) {
        case 1:
            printf("A\n");
            break;
        case 2:
            printf("B\n");
        default:
            warnning(WARNNING_LEVEL_B, "the value is not number");
            case 'a':
                printf("a\n");
                break;
            case 'b':
                printf("b\n");
    }


    debug(DEBUG_LEVEL_A, "return form %s", __func__);
    return 0;
}
