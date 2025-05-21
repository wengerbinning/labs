
#include <stdio.h>
#include <unistd.h>
#include <time.h>

extern int file_open_read_A (void);



int main (int argc, const char *argv[]) {
    int val;
    time_t stamp1 = time(NULL);
    time_t stamp2 = time(NULL);

    while (stamp2 - stamp1 < 10) {
        stamp2 = time(NULL);
        printf("current timestamp: %ld\n", stamp2);
        val = file_open_read_A();
        printf("read value: %d\n", val);
        
        if (val)
            break;

        sleep(1);
    }

    return 0;
}

