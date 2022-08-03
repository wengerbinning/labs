#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


int func (char *buf, size_t size) {
    int fd = open("result", O_RDWR);
    read(fd, buf, size);
    close(fd);
    return 0;
}

#define BUFIZE 32

int main (int argc, char *argv[]) {

    char buffer[BUFIZE + 1], *str;

    func(buffer, BUFIZE);

    if (strlen(buffer))
        printf("%s\n", buffer);
        str = buffer;
        if ('/' == *(str + strlen(str) - 1)) {
            *(str + strlen(str) - 1) = 0x0;
            printf("demdo\n");
        }

    return 0;
}