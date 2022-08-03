#include <stdio.h>
#include <sys/random.h>

int main()
{
    unsigned char r;
    puts("Waiting for random number generator to initialise...");
    return getentropy(&r, sizeof r);
}