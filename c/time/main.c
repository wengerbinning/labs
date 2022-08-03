#include <stdio.h>
#include <time.h>

int main(int agrc, char *argv[]) {
    int idx = 0;
    struct timespec t1, t2;

    clock_gettime(CLOCK_MONOTONIC, &t1);
    printf("sec=%ld nsec=%ld\n", t1.tv_sec, t1.tv_nsec);

    while (1) {
        time_t t;

        clock_gettime(CLOCK_MONOTONIC, &t2);

        t = t2.tv_sec - t1.tv_sec;
        if (t <= 0)
            continue;

        clock_gettime(CLOCK_MONOTONIC, &t1);
        printf("%8d - %ld.%08ld \n", idx++, t1.tv_sec, t1.tv_nsec);
    }

    return 0;
}