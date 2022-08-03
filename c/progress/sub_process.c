#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
int g_var;

int main(int argc, const char *argv[]) {
    pid_t pid;
    int var = 0;
    // char buffer[] = "abcdefghigklmnopqrstuvwsyz";
    // printf("[DEBUG] start host process: %d\n", getpid());

    // write(STDOUT_FILENO, buffer, sizeof(buffer));
    // printf("%s@",buffer);

    pid = fork();
    if(pid == 0) {
        printf("#\n");
        // write(STDOUT_FILENO, buffer, sizeof(buffer));
        printf("[DEBUG] fork a child process %d", getpid());
        g_var++; var++;
        printf("global variable: %d,\t local variable: %d", g_var, var);
        exit(0);
    }

    pid = wait(NULL);
    printf("[DEBUG] child process %d exit.\n", pid);
    printf("global variable: %d,\t local variable: %d", g_var, var);

    return 0;
}




