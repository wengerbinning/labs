#include <stdio.h>
#include <stdbool.h>


typedef union switch_t {
    unsigned char value;
    struct {
        unsigned char enable: 1;
        unsigned char states: 7;
    };
    struct {
        unsigned char recv: 6;
        unsigned char keep: 1;
        unsigned char flag: 1;
    };
} switch_t;

#define _switch(s, f, v) do {(s).states = (f); (s).enable = (v);} while (0)
#define switch_value(s) ((s).flag ? (s).enable : false)
#define switch_(s) ((s).flag ? (s).keep : false)
#define switch_check(s, v) do { if (!(s).flag) _switch(s, 0x60, v);} while (0)
#define switchtoggle(s, v) _switch(s, 0x40, v)

int main(int agrc, char *argv[]) {

    switch_t enable;

    switchtoggle(enable, true);
    switch_check(enable, false);
    printf("bool %d, %d, %d\n", sizeof(bool), sizeof(bool), sizeof(switch_t));
    printf("switch 0X%02X, %s\n", enable, switch_value(enable)? "true": "false");

    return 0;
}