#include <stdio.h>
#include <unistd.h>

int main(int argc, const char*argv[]) {
    printf("has %d number of params.\n", argc);
    if( argc = 2) {
        execl(argv[1], argv[1], NULL);
    }
    else if( argc > 2 ) {
        execl(argv[1], argv[1], argv[2], NULL);
    }

    // execl("/usr/bin/pwd", "/usr/bin/pwd", NULL);
    return 0;
}