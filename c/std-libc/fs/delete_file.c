#include <stdio.h>

int main(int argc, const char *argv[]) {
    char path[64] = {0};
    int ret;

    int fd;
    sprintf(path,"/tmp/tcpdump/tcpdump.pid");
    printf("[DEBUG] %s\n", path);
    
    ret = remove(path);

    printf("[DEBUG] %d\n", ret);

    return 0;
}
