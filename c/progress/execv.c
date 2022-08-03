#include<unistd.h>

int main(int argc, const char *argv[]) {
    // char *cmd_argv[] = {"/usr/bin/pwd", NULL};
    char *cmd_argv[] = {NULL, NULL, NULL};

    if( argc < 2 ) {
        return -1;
    }

    cmd_argv[0] = argv[1];

    if(argc >= 2) {
        cmd_argv[1] = argv[2];
    }

    // execv("/usr/bin/pwd", cmd_argv);
    execv(argv[1],cmd_argv); 
        
    return 0;
}