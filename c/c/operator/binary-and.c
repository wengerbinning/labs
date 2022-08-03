#include <stdio.h>

int main(int argc, const char **argv) {
    int a = 1;
    if (a && (a=3,a==3) ) {
        printf("%d\n", a);
    }

    return 0;
}