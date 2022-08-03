#include <stdio.h>

int main(int argc, char *argv[]) {
    int a, b;
    
    sscanf(argv[1], "%d", &a);
    sscanf(argv[2], "%d", &b);
    
    if (a == 3 && b == 4) {
        printf("[ INFO ] 3^2 + 4^2 = %d\n", a^2+b^2);
    } else if (a > 0) {
        printf("[ INFO ] a is positive\n");
    } else {
        printf("[ INFO ] a= %d, b= %d\n", a, b);
    }

    return 0;
}
