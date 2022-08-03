#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

extern char **environ;

int main(int argc, char *argv) {
    char buf[1024] = {0};

    readlink("/proc/self/fd/0", buf, sizeof(buf));
    printf("stdin : %s\n", buf);

    memset(buf, 0, sizeof(buf));
    readlink("/proc/self/fd/1", buf, sizeof(buf));
    printf("stdout: %s\n", buf);

    memset(buf, 0, sizeof(buf));
    readlink("/proc/self/fd/2", buf, sizeof(buf));
    printf("stderr: %s\n", buf);

    return 0;
}