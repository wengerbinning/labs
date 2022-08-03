#include <stdio.h>



static inline unsign int func(unsigned int idx, const ch)




#define PPE1_BASE   1
#define PPE2_BASE   1
#define PPE_BASE(x)  PPE1_BASE
#define _PPE_BASE(x)     PPE ## x ## _BASE









#define PPESTR(x)  _STR(ppe ## x)
#define _STR(s)   # s

int main ( int argc, char *argv[]) {
    int i = 1;

    printf("line: %s\n", PPE_BASE(i));
    return 0;
}