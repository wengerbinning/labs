#include <stdio.h>
#include <unistd.h>

int main(int argc, const char *argv[]) {
    uid_t uid = 0, euid = 0;
    uid = getuid();
    printf("now uid is %d\n", uid);
    euid = geteuid();
    printf("now uid is %d, euid is %d\n", uid, euid);
    return 0;
}