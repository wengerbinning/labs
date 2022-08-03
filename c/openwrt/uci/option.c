#include <stdio.h>
#include <string.h>
#include <stdlib.h>



enum {
    A,
    B,
    C,
    D,
    MAX
};


int main(int argc, char *argv[]) {
    char buffer[2048] = "A2313131";
    char *server = NULL;
    //buffer[1] = '\0';
    printf("val: %d\n", atoi(buffer));


    server = strdup("dedeo");

    printf("%d: %s\n", strlen(server), server);


    printf("A:%d, B:%d, C:%d, D:%d\n", A, B, C, D);
    for (int i= 0; i< MAX; i++)
        printf("%d\n", i);





    return 0;
}