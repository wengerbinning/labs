#include <stdio.h>
#include <stdlib.h>
#include <string.h>





struct session {
    int id;
};

struct conn {
    int id;
    struct session *s;
};


int main (int argc, char *argv[]) {
    struct session *s;
    struct conn c;

    s = malloc(sizeof(struct session));
    memset(s, 0, sizeof(struct session));
    s->id = 10;


    c.id = 1;
    c.s = s;

    printf("conn id %d, s(%p) id %d; session(%p)\n", c.id, c.s, c.s->id, s);

    free(s);
    s = NULL;

    printf("conn id %d, s(%p) id %d; session(%p)\n", c.id, c.s, c.s->id, s);


    return 0;
}