#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#define PROV_TMPFS_HOME "/tmp/provision"

int main (int argc, char *argv[]) {

    if (access(PROV_TMPFS_HOME, F_OK) < 0) {
        printf("running in here\n");
        mkdir(PROV_TMPFS_HOME, 0755);
        printf("errno %d, %s\n", errno, strerror(errno));
    }


    return 0;
}