#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>

#include "center.h"


int getfd(void) {
    int fds[2];
    pid_t pid;
    int ret, len;
    char rbuffer[RBUFIZE + 1];

    if (pipe(fds)) {
        printf("Failed to create pipe\n");
        return -1;
    }

    printf("Read %d, write %d\n", fds[0], fds[1]);

    signal(SIGPIPE, SIG_IGN);

    pid = fork();
    if ( pid == 0 ) {
        // TODO
        while(true) {

            printf("worker A is running %d...\n", getpid());
            memset(rbuffer, 0, sizeof(RBUFIZE));
            if ((len = read(fds[0], rbuffer, RBUFIZE)) <= 0) {
                printf("read error, exit!");
                break;
            }

            printf("read %dB data: %s", rbuffer);

            sleep(1);
        }

        printf("worker exit.\n");
        return 0;
    }

    return fds[1];
}


