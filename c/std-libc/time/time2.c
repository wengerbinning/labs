#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define STR(s) _STR(s)
#define _STR(s) #s
#define TYPE_PRINT(t) printf("%s -> %d, \t%s * -> %d\n",  STR(t), sizeof(t), STR(t), sizeof(t*))

int main (int argc, char *argv[]) {
    struct timespec _ts0, _ts1, _ts2;
    struct timespec  ts0,  ts1,  ts2;


    TYPE_PRINT(char);
    TYPE_PRINT(short);
    TYPE_PRINT(int);
    TYPE_PRINT(long);
    TYPE_PRINT(long long);
    TYPE_PRINT(time_t);
    TYPE_PRINT(void);

    /* system time */
    clock_getres(CLOCK_REALTIME, &_ts0);
    clock_gettime(CLOCK_REALTIME, &ts0);
    printf("resolution: %ld %09ld, time: %10ld %09ld, CLOCK_REALTIME\n", _ts0.tv_sec, _ts0.tv_nsec, ts0.tv_sec, ts0.tv_nsec);
    clock_getres(CLOCK_REALTIME_ALARM, &_ts0);
    clock_gettime(CLOCK_REALTIME_ALARM, &ts0);
    printf("resolution: %ld %09ld, time: %10ld %09ld, CLOCK_REALTIME_ALARM\n", _ts0.tv_sec, _ts0.tv_nsec, ts0.tv_sec, ts0.tv_nsec);
    clock_getres(CLOCK_TAI, &_ts0);
    clock_gettime(CLOCK_TAI, &ts0);
    printf("resolution: %ld %09ld, time: %10ld %09ld, CLOCK_TAI\n", _ts0.tv_sec, _ts0.tv_nsec, ts0.tv_sec, ts0.tv_nsec);
    /* boot time */
    clock_getres(CLOCK_MONOTONIC, &_ts0);
    clock_gettime(CLOCK_MONOTONIC, &ts0);
    printf("resolution: %ld %09ld, time: %10ld %09ld, CLOCK_MONOTONIC\n", _ts0.tv_sec, _ts0.tv_nsec, ts0.tv_sec, ts0.tv_nsec);
    clock_getres(CLOCK_MONOTONIC_COARSE, &_ts0);
    clock_gettime(CLOCK_MONOTONIC_COARSE, &ts0);
    printf("resolution: %ld %09ld, time: %10ld %09ld, CLOCK_MONOTONIC_COARSE\n", _ts0.tv_sec, _ts0.tv_nsec, ts0.tv_sec, ts0.tv_nsec);
    clock_getres(CLOCK_MONOTONIC_RAW, &_ts0);
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts0);
    printf("resolution: %ld %09ld, time: %10ld %09ld, CLOCK_MONOTONIC_RAW\n", _ts0.tv_sec, _ts0.tv_nsec, ts0.tv_sec, ts0.tv_nsec);
    clock_getres(CLOCK_BOOTTIME, &_ts0);
    clock_gettime(CLOCK_BOOTTIME, &ts0);
    printf("resolution: %ld %09ld, time: %10ld %09ld, CLOCK_BOOTTIME\n", _ts0.tv_sec, _ts0.tv_nsec, ts0.tv_sec, ts0.tv_nsec);
    clock_getres(CLOCK_BOOTTIME_ALARM, &_ts0);
    clock_gettime(CLOCK_BOOTTIME_ALARM, &ts0);
    printf("resolution: %ld %09ld, time: %10ld %09ld, CLOCK_BOOTTIME_ALARM\n", _ts0.tv_sec, _ts0.tv_nsec, ts0.tv_sec, ts0.tv_nsec);
    /* task time */
    clock_getres(CLOCK_PROCESS_CPUTIME_ID, &_ts0);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts0);
    printf("resolution: %ld %09ld, time: %10ld %09ld, CLOCK_PROCESS_CPUTIME_ID\n", _ts0.tv_sec, _ts0.tv_nsec, ts0.tv_sec, ts0.tv_nsec);
    clock_getres(CLOCK_PROCESS_CPUTIME_ID, &_ts0);
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts0);
    printf("resolution: %ld %09ld, time: %10ld %09ld, CLOCK_PROCESS_CPUTIME_ID\n", _ts0.tv_sec, _ts0.tv_nsec, ts0.tv_sec, ts0.tv_nsec);


    return 0;
}