#include <stdio.h>

struct res_flags {
    union {
        struct {
            unsigned int attr:  8;  /* resource attibute */
            unsigned int scope: 8;  /* resource scope */
        };
        unsigned int value;
    };
};

#define RES_GET_ATTR(res)  (res->flags.attr)
#define RES_SET_ATTR(res, a)    (RES_GET_ATTR(res) |=  (a))
#define RES_CHECK_ATTR(res, a) ((RES_GET_ATTR(res) &   (a)) && 1)
#define RES_CLEAR_ATTR(res, a)  (RES_GET_ATTR(res) &= ~(a))

#define RES_GET_SCOPE(res)  (res->flags.scope)
#define RES_SET_SCOPE(res, s)    (RES_GET_SCOPE(res) |=  (a))
#define RES_CHECK_SCOPE(res, s) ((RES_GET_SCOPE(res) &   (a)) && 1)
#define RES_CLEAR_SCOPE(res, s)  (RES_GET_SCOPE(res) &= ~(a))




typedef struct resource {
    struct res_flags flags;
} resource_t;




int main(int agrc, char *argv[]) {
    resource_t resource, *res;

    res = &resource;

    printf("flags: %08X, attr: %02X, scope: %02X, state: %s\n",
        res->flags.value, RES_GET_ATTR(res), RES_GET_SCOPE(res),
        RES_CHECK_ATTR(res, 1) ? "true" : "false");

    if ( RES_CHECK_ATTR(res, 1))
        RES_CLEAR_ATTR(res, 1);
    else
        RES_SET_ATTR(res, 1);

    printf("flags: %08X, attr: %02X, scope: %02X, state: %s\n",
        res->flags.value, RES_GET_ATTR(res), RES_GET_SCOPE(res),
        RES_CHECK_ATTR(res, 1) ? "true" : "false");

    return 0;
}

