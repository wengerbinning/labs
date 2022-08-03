#include <stdio.h>
#include <time.h>
#include <unistd.h>


int main (int argc, char *argv) {
    time_t start = time(NULL);
    printf("start %ld\n", start);
    sleep(2);
    time_t end = time(NULL);
    double diff = difftime(end, start);
    printf("end   %ld spend %ld second\n", end, end - start);
    return 0;
}