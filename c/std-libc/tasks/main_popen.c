#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main (int agrc, char *argv[]) {
    int ret;
    printf("start running task %d ...\n", getpid());
    ret = system("echo $$; sleep 1; exit 1");

    printf("finished task: %d.\n", WEXITSTATUS(ret));
    return 0;
}



/*

#include <stdlib.h>
int system(const char *command);

system - 会执行shell命令,执行结束后再执行后续的任务.

*/