#include <stdio.h>
#include <stdlib.h>


#include "retcode.h"
#include "test.h"

int main(int agrc, char *argv[]) {
    int code = atoi(argv[1]);
    printf("static error: %d - %s\n", code, _RET_GET_MSG(code1, code));

    return 0;
}