#include <stdio.h>
#include <string.h>

#define BUFIZE 16

int main(int agrc, char *argv[]) {
    char buffer[BUFIZE + 1];


    snprintf(buffer, BUFIZE, "1234567890aecfefg");


    printf("%s, %d\n", buffer, strlen(buffer));




    return 0;
}