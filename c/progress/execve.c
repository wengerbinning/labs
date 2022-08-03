#include <unistd.h>

int main(int argc, char *argv[])
{
    char *cmd_argv[] = {NULL, NULL, NULL};
    char *cmd_envp[] = {"PATH=/usr/lib/demo", NULL};

    if( argc < 2 ) {
        return -1;
    }

    cmd_argv[0] = argv[1];

    if(argc >= 2) {
        cmd_argv[1] = argv[2];
    }

    execve(argv[1], cmd_argv, cmd_envp);
    
    return 0;
}
