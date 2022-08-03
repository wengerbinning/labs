#include <stdio.h>
#include <sys/stat.h>
#include <libgen.h>

#define TMP_DIR "/tmp"

int main (int argc, const char *argv[]) {
    char path[64] = {0};
    // TODO You can start your work in here.
    sprintf(path,"%s/tcpdump/demo", TMP_DIR);
    if ( 0 == mkdir(path, S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH) ) {
        printf("[DEBUG] create a directory:%s\n", path);
    } else {
        printf("[ERROR] create directory is error\n");
    }
    printf("[DEBUG] %s\n", dirname(path));


    return 0;
}
