#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>


#define container_of(ptr, type, member) \
    ({								                            \
        const typeof(((type *) NULL)->member) *__mptr = (ptr);	\
        (type *) ((char *) __mptr - offsetof(type, member));	\
    })

struct url_t {
    struct url_t *list;
    char *url;
};


struct url_t url = {NULL, "hello"};

int main(int agrc, char *argv[]) {
    struct url_t *p;


    p = container_of(&url.list, struct url_t, list);

    printf("url: (%s)\n", p->url);



    return 0;
}