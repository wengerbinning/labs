#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define TMP_FILE "./temp.txt"


int file_open_read_A (void)
{
    int fd, ret = 0;
    char buffer[2];

    if ((fd = open(TMP_FILE, O_RDONLY)) < 0) {
        return ret;
    }

    read(fd, buffer, sizeof(buffer));

    close(fd);
    remove(TMP_FILE);

    if (atoi(buffer))
        ret = 1;

    return ret;
}