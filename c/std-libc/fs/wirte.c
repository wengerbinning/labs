#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
//#include <sys/stat.h>

#define TCPDUMP_TMP_DIR "/tmp/tcpdump"

int write_to_file(const char *path, char *content, size_t size);

int main(int argc, const char *argv[]) {
    char buffer[1024] = {0};
    char path[64] = {0};
    int ret;
    // TODO write some content to a file.

    sprintf(path, "%s/duration", TCPDUMP_TMP_DIR);
    sprintf(buffer, "hello, I am a test content.\n");

    ret = write_to_file(path, buffer, sizeof(buffer));
    if ( ret == -1 ){
        printf("[ERROR] write is error!\n");
        return -1;
     } else {
        printf("[DEBUG] write %d Byte ot %s.\n", ret, path);
    }

    return 0;
}

int write_to_file(const char *path, char *content, size_t size) {
    int fd;
    int ret;
    fd = open(path, O_RDWR|O_CREAT|O_TRUNC, S_IRUSR|S_IWUSR);
    if ( fd == -1 )
        return -1;
    ret = write(fd,content, size);
    close(fd);
    return ret;
}
