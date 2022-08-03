#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int fd;

    fd = open("/var/run/demo.pid", O_WRONLY|O_CREAT|O_EXCL, S_IRUSR|S_IWUSR);
    if ( fd == -1 ) {
        printf("[ERROR] open /var/run/demo/pid is error!\n");
        return -1;
    }
    fwrite()

    write(fd, "1234", sizeof("1234"));
    close(fd);
    fread()
    return 0;
}
