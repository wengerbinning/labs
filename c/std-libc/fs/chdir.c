#include<stdio.h>
#include<unistd.h>

int main(int argc, const char *argv[]) {
    pid_t pid_1 = 0, pid_2 = 0;

    pid_1 = fork();

    if( pid_1 == 0 ) {
        execl("/usr/bin/pwd", "/usr/bin/pwd", NULL);
    }
    
    chdir("/mnt/");

    pid_2 = fork();
    if( pid_2 == 0 ) {
        execl("/usr/bin/pwd", "/usr/bin/pwd", NULL);
    }

    printf("%d, %d\n", pid_1, pid_2);

    return 0;
}

