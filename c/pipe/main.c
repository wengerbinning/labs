#include <stdio.h>
#include <unistd.h>

#include "center.h"




int main(int artgc, char *argv[]) {
    int fd;
    char buffer[WBUFIZE + 1];
    int wlen;

    if ((fd = getfd()) < 0) {
        printf("Failed to error\n");
        return 0;
    }

    sleep(3);
    printf("Write buffer from %d\n", getpid());
    sprintf(buffer, "ABCDEFG\n");
    if ((wlen = write(fd, buffer, WBUFIZE)) <=0)
        printf("write error");

    printf("Write %d to %d\n", wlen, fd);
    sleep(10);


    return 0;
}