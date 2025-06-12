#include <stdio.h>


int main (int argc, char *argv[]) {
    int a, b;

    printf("a = %d, b = %d\n", a, b);

    b = 0 ? a : 0;
    a = 1 ? : 0 ;
    printf("a = %d, b = %d\n", a, b);

    return 0;
}