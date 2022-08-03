#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define VER_FIELD_MASK  0xFF
#define VER_MAJOR_OFST    24
#define VER_MINOR_OFST    16
#define VER_PHASE_OFST     8
#define VER_PATCH_OFST     0

#define VER_MAJOR(n)     ((n >> VER_MAJOR_OFST) & VER_FIELD_MASK)
#define VER_MINOR(n)     ((n >> VER_MINOR_OFST) & VER_FIELD_MASK)
#define VER_PHASE(n)     ((n >> VER_PHASE_OFST) & VER_FIELD_MASK)
#define VER_PATCH(n)     ((n >> VER_PATCH_OFST) & VER_FIELD_MASK)

#define VERSTR(n) VER_MAJOR(n),VER_MINOR(n), VER_PHASE(n), VER_PATCH(n)


#define HOME0 "0"
#define HOME1 "1"
#define BUFIZE 64

int main (int argc, char *argv[]) {
    int fd;
    char path0[BUFIZE + 1], path1[BUFIZE + 1];

    if (access(HOME0, F_OK) < 0) {
        mkdir(HOME0, 0755);
    }
    if (access(HOME1, F_OK) < 0) {
        mkdir(HOME1, 0755);
    }

    snprintf(path0, BUFIZE, "%s/%s", HOME0, "demo.txt");
    snprintf(path1, BUFIZE, "%s/%s", HOME1, "test.txt");


    if (0 <= (fd = open(path0, O_CREAT | O_RDWR | O_TRUNC, 0644))) {
        close(fd);
    }



    if (rename(path0, path1) < 0) {
        printf("error:\n", strerror(errno));
    }

    return 0;
}

