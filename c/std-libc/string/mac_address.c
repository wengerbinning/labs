#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "strlib.h"
#include "hexdump.h"

// #define MACFMT0 "%02x%02x%02x%02x%02x%02x"
// #define MACFMT1 "%02x:%02x:%02x:%02x:%02x:%02x"
#define MACFMT0 "%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx"      /* Example: c074ada1b2c3 */
#define MACFMT1 "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx" /* Example: c0:74:ad:a1:b2:c3 */
#define MACFMT2 "%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX"      /* Example: C074ADA1B2C3 */
#define MACFMT3 "%02hhX:%02hhX:%02hhX:%02hhX:%02hhX:%02hhX" /* Example: C0:74:AD:A1:B2:C3 */

#define _STR2MAC(m) &(m)[0], &(m)[1], &(m)[2], &(m)[3], &(m)[4], &(m)[5]
#define _MAC2STR(m)  (m)[0],  (m)[1],  (m)[2],  (m)[3],  (m)[4],  (m)[5]
#define STR2MAC(mac) _STR2MAC((unsigned char *)(mac))
#define MAC2STR(mac) _MAC2STR((unsigned char *)(mac))

int main (char *argc, char *argv[]) {
    int a = 0;
    unsigned short mac[3] = {0};

    if (argv[1])
        a = atoi(argv[1]);

    if (argv[2]) {
        switch (a) {
        case 0:
            sscanf(argv[2],  MACFMT0   , STR2MAC(mac));
            break;
    //     case 1:
    //         sscanf(argv[2], MACFMT1, STR2MAC(mac));
    //         break;
        default:
            printf("not %d type \n", a);
            return 0;
        }
    }

    printf("mac: %d\n", sizeof(mac));
    hexdump(mac, sizeof(mac));

    printf("MACFMT0: " MACFMT0 "\n", MAC2STR(mac));
    printf("MACFMT1: " MACFMT1 "\n", MAC2STR(mac));

    printf("MACFMT2: " MACFMT2 "\n", MAC2STR(mac));
    printf("MACFMT3: " MACFMT3 "\n", MAC2STR(mac));
    return 0;
}