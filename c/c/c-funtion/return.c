#include <stdio.h>

static char* defaul_lan_ifname[] = {
    "br-lan.1",
    "br-lan",
    "br-lan0_zone0",
};

#define DEFAULT_LAN_IFNAME_MAX 3




char *get_name (int a) {
    char *p;
    int i;

    for (i = 0; i < DEFAULT_LAN_IFNAME_MAX; i++) {
        if ( i == a)
            return defaul_lan_ifname[i];
    }

    return defaul_lan_ifname[DEFAULT_LAN_IFNAME_MAX - 1];
}


int main(int argc, char *argv[]) {

    printf("name: %s\n", get_name(3));

    return 0;
}
