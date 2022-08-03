#include <stdio.h>
#include <stdint.h>

#define AUTHOR

#define STR(x) _STR(x)
#define _STR(x) #x



/* Device ID API
 *
 * DEV_ID(flags, famil, model, scope)
 * DEV_FLAGS(id)
 * DEV_FAMIL(id)
 * DEV_MODEL(id)
 * DEV_SCOPE(id)
 */
#define __MASK(id, m) ((id) & (m))
#define __DEV_GET_ID(id, o, m) (__MASK(((0ULL | (id)) >> (o)), (m)))
#define __DEV_SET_ID(em, o, m) (__MASK(((0ULL | (em))), (m)) << (o))
#define DEV_ID(flags, famil, model, scope) \
    (__DEV_SET_ID((flags),  0, 0xFFFF) | __DEV_SET_ID((famil), 16, 0xFFFF) | \
     __DEV_SET_ID((model), 32, 0xFFFF) | __DEV_SET_ID((scope), 32, 0xFFFF))
#define DEV_FLAGS(id) __DEV_GET_ID(id,  0, 0xFFFF)
#define DEV_FAMIL(id) __DEV_GET_ID(id, 16, 0xFFFF)
#define DEV_MODEL(id) __DEV_GET_ID(id, 32, 0xFFFF)
#define DEV_SCOPE(id) __DEV_GET_ID(id, 48, 0xFFFF)


#define GWN_DEV_ID(flags, famil, model) DEV_ID((flags), (famil), (model), 0)


int main(int argc, char *argv[]) {
    int i, j, k;
    uint64_t id;

    id = GWN_DEV_ID(0x1, 0x02, 0x03);
    // printf("0x%llX\n", id);


    for (i = 0; i < (0x1000 << 1); i++) {
        for (j = 0; j < (0x1000 << 1); j++) {
            for (k = 0; k < (0x1000 << 1); k++) {
                id = GWN_DEV_ID(i, j, k);

                if (DEV_FLAGS(id) != i || DEV_FAMIL(id) != j || DEV_MODEL(id) != k ) {
                    printf("%016llx %4X", id, DEV_SCOPE(id));
                    if (DEV_MODEL(id) != k)
                        printf(" \e[31m%4X\e[0m", k);
                    else
                        printf(" \e[32m%4X\e[0m", k);
                    if (DEV_FAMIL(id) != j)
                        printf(" \e[31m%4X\e[0m", j);
                    else
                        printf(" \e[32m%4X\e[0m", j);
                    if (DEV_FLAGS(id) != i)
                        printf(" \e[31m%4X\e[0m", i);
                    else
                        printf(" \e[32m%4X\e[0m", i);
                    printf("\n");
                // } else {
                //     printf("%016llX %4X %4X %4X %4X\n",
                //         id, DEV_SCOPE(id), k, j, i);
                }
            }
        }
    }



    // printf("0x%llX\n", _DEV_GET_ID(id, 32, 0xFFFFFFFF));
    return 0;
}
