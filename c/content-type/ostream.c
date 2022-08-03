#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <libgen.h>
#include <unistd.h>

#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>
#include <glob.h>
#include <regex.h>
#include <fcntl.h>
#include <sys/un.h>


#include "hexdump.h"
#include "iostream.h"


#define BUFIZE 256
#define PATH


#define log_error(fmt, ...)     printf("\033[31m" fmt "\033[0m\n", ##  __VA_ARGS__ )
#define log_warning(fmt, ...)   printf("\033[33m" fmt "\033[0m\n", ## __VA_ARGS__ )
#define log_notice(fmt, ...)    printf("\033[32m" fmt "\033[0m\n", ## __VA_ARGS__ )
#define log_info(fmt, ...)      printf("\033[37m" fmt "\033[0m\n", ## __VA_ARGS__ )
#define log_debug(fmt, ...)     printf("\033[34m" fmt "\033[0m\n", ## __VA_ARGS__ )


struct form_data {
    int index;
    char *name, *value, *filename;
    char *content;
    int type;
    bool start;
    void *priv;

    size_t offset;
    bool end;
};

#define CTX_BUFZIE 2048
struct fio_context {
    int method;
    int action;

    bool auth;
    size_t offset;
    char buffer[CTX_BUFZIE + 1];

    void *priv;

    size_t ofst;
    bool proxy;

    int content_type;
    size_t content_length;
    char *content_charset;
    char *content_boundary;

    int form_count;
    struct form_data form;

    /* form data cache */
    bool cache;
    int cache_fd;

    /* sock */
    int fd;

    int retVal;
};


size_t form_write(struct context *ctx, struct form *form, char *buf, size_t len)
{
    form->index = ctx->form_count;
    // log_debug("%d offset:%d, %s", ctx->form_count, form->offset, buf);

    if (form->offset == 0 && form->end) {
        printf("[%02d] %s\n", form->index, buf);
    } else {
        snprintf(form->buffer + form->offset, FORM_BUFZIE - form->offset, "%s", buf);
        if ( form->end )
            printf("[%02d] %s\n", form->index, form->buffer);
    }

    return len;
}

int form_finish (struct context *ctx, struct form *form) {
    if (form->end) {
        printf("[%02d] %s\n", form->index, form->buffer);
    }
    return 0;
}

struct form _form_static;

// ============================================================================
int fcgi_init_form (struct fio_context *ctx, struct form_data *form)
// ============================================================================
{
    int fd;

    if (!ctx && !form)
        return -1;

    memset( form, 0, sizeof(struct form_data) );
    form->priv = ctx;
    if ((fd = open( "stream.bin", O_CREAT | O_RDWR, 0664 ))) {
        ctx->cache = true;
        ctx->cache_fd = fd;
    }

    return 0;
}


// ============================================================================
int form_data_init(struct form_data *form)
// ============================================================================
{
    struct fio_context *ctx = form->priv;
    ctx->form_count++;

    memset( form, 0, sizeof(struct form_data) );
    form->priv = ctx;
    form->index = ctx->form_count;
    return 0;
}



#undef BUFIZE
#define BUFIZE 2048

// ============================================================================
static int
fcgi_input_multipart_form_data(
    struct form_data *form,
    char *data, size_t dlen
)
// ============================================================================
{
    int ret;
    char *boundary, *p0, *p1;
    size_t dofst = 0, rofst = 0;
    size_t ofst, size, len, cofst, plen;
    char buffer[BUFIZE + 1];

    struct fio_context *ctx = form->priv;
    boundary = ctx->content_boundary;

    static bool cache;
    static size_t clen;
    static char cbuffer[BUFIZE + 1];
    if (ctx->offset == 0) {
        cache = false; clen = 0;
    }
    ctx->offset += dlen;
    /* check cache */

    if (cache) {
        log_notice( "==== handling (cache:%ld, data:%ld/%ld) ====",
            clen, dlen, ctx->offset);
    } else {
        log_notice( "==== handling (data:%ld/%ld) ====",
            dlen, ctx->offset);
    }

    p0 = data; plen = dlen;
    if (cache) {
        cofst = clen;
        len = (dlen < (BUFIZE - clen)) ? dlen : (BUFIZE - clen);
        memcpy(cbuffer + cofst, data, len);
        clen += len;  rofst = len;
        p0 = cbuffer; plen = clen;
        log_debug("cache0 %ld (cache:%d/%d, data:%ld/%ld/%ld)",
            len, cofst, clen, dofst, rofst, dlen);
    }
    /* check boundary */
    check_boundary:
    while ( (p1 = (char *)memmem(p0, plen, boundary, strlen(boundary))) ) {
        bool suffix = false;
        size = (p1 - p0); plen -= size;
        if ((4 <= size) && (0 == strncmp(p1 - 4, "\r\n--", 4))) {
            memset(p1 - 4, 0, 4);
            size -= 4;
        } else if ( (2 <= size) && 0 == strncmp(p1 - 2, "--", 2) ) {
            size -= 2;;
            log_debug( "==== START %d ====", ctx->form_count);

        }
        /* write form data end */
        if ( 0 < size ) {
            form->end = true;
            log_info("[%02d] form: offset:%d, len: %d",
                form->index, form->offset, size);
            hexdump(p0, size);
            write(ctx->cache_fd, p0, size);

            // if ( form_data_write(form, p0, size) != size ) {
            //     form_data_exit( form );
            //     ret = ctx->retVal;
            //     Log(LOG_DEBUG, "return code0 %d", ret);
            //     return 0;
            // }
            // form_data_finish( form );
        }
        /* next form */

        form_data_init( form );
        log_notice("Start %d form ....", form->index );
        p0 = (p1 + strlen(boundary)); plen -= strlen(boundary);
        if (plen == 4 && 0 == strncmp(p0, "--\r\n", 4)) {
            log_debug(  "==== END1 ====");
            return dlen;
        } else if (2 <= plen && 0 == strncmp(p0, "\r\n", 2)) {
            p0 += 2; plen -= 2;
            suffix = true;
        }

        /* update pointer */
        if (cache && (p0 < (cbuffer + cofst))) {
            ofst = ((cbuffer + cofst) - p0); len = ((cbuffer + clen) - p0);
            memcpy(buffer, p0, len); memcpy(cbuffer, buffer, len);
            cache = true; cofst = ofst; clen = len;
            /* copy data */
            ofst = clen; size = (BUFIZE - clen);
            len = ((dlen - rofst) < size) ? (dlen - rofst) : size;
            memcpy(cbuffer + ofst, (data + rofst), len);
            rofst += len; clen += len;
            /* update pointer */
            p0 = cbuffer; plen = clen;
        } else if (cache) {
            dofst += (p0 - (cbuffer + cofst));
            p0 = (data + dofst); (plen = dlen - dofst);
            if (plen == 4 && 0 == strncmp(p0, "--\r\n", 4)) {
                log_debug(  "==== END2 ====");
                return dlen;
            } else if (2 <= plen && 0 == strncmp(p0, "\r\n", 2)) {
                dofst += 2; p0 += 2; plen -= 2; suffix = true;
            }
            cache = false;
            rofst = dofst;
        }


        /* cache suffix */
        if (!cache) {
            dofst = p0 - data; rofst = dofst;
            if (!suffix) {
                len = plen + strlen(boundary);
                memcpy(cbuffer, (p0 - strlen(boundary)), len);
                cache = true; cofst = 0; clen = len; rofst += plen;
                log_debug("cache1 %ld, (cache:%d/%d, data:%ld/%ld)", len, cofst, clen, dofst, rofst);
            }
        }
    }

    /* check end */
    p0 = cache ? cbuffer : (data + dofst);
    plen = cache ? clen : (dlen - dofst);
    if ( plen == 4 && 0 == strncmp(p0, "--\r\n", 4) ) {
        log_debug(  "==== END3 ====");
        return dlen;
    }

    /* update cache */
    if ( cache && BUFIZE <= clen ) {
        len = clen; size = (strlen(boundary) + 4 + 3);
        len -= ((size < BUFIZE) ? size : 0);
        memset(buffer, 0, sizeof(buffer));
        memcpy(buffer, cbuffer, len);
        form->end = false;
        log_info("#[%02d] form: offset:%d, len: %d",
            form->index, form->offset, len);
        // hexdump(buffer, len);
        write(ctx->cache_fd, buffer, len);
        // if ( form_data_write(form, buffer, len) != len ) {
        //     form_data_exit( form );
        //     ret = ctx->retVal;
        //     Log(LOG_DEBUG, "return code1 %d", ret);
        //     return ret;
        // }
        form->offset += len;
        cache = false;
        if ( size < BUFIZE ) {
            ofst = (clen - size);
            memcpy(buffer, cbuffer + ofst, size);
            memcpy(cbuffer, buffer, size);
            cofst = ((ofst < cofst) ? cofst - ofst : 0);
            cache = true; clen = size;
            log_debug("cache2 %ld (cache:%d/%d, data:%ld/%ld/%ld)",
                size, cofst, clen, dofst, rofst, dlen);
            p0 = cbuffer; plen = clen;
        }
        cofst = clen;
    }

    /* cache data */
    ofst = cache ? cofst : 0;
    len = cache ? clen : 0; clen = len;
    size = cache ? (BUFIZE - clen) : BUFIZE;
    len = (size < (dlen - rofst)) ? size : (dlen - rofst);
    if (0 < len) {
        memcpy((cbuffer + ofst), (data + rofst), len);
        cache = true; cofst = ofst; clen += len;
        dofst = rofst; rofst += len;
        log_debug("cache3 %ld (cache:%d/%d, data:%ld/%ld/%ld)",
            len, cofst, clen, dofst, rofst, dlen);
        p0 = cbuffer; plen = clen;
        goto check_boundary;
    }
    return dlen;
}

int main(int agrc, char *argv[])
{
    FILE *istream, *ostream;
    char buffer[BUFIZE +1];
    size_t rlen, wlen;
    char *prog = basename(argv[0]);
    struct fio_context ctx = {
        .content_boundary = CONTENT_BOUNDARY,
    };

    struct form_data form;

    fcgi_init_form( &ctx, &form );
    ctx.offset = 0;
    if (0 == strcmp(prog, "iostream")) {
        printf("Nothing\n");
    } else if (0 == strcmp(prog, "ostream")) {
        istream = fopen(STREAM_BIN, "rb");
        while (!feof(istream)) {
            rlen = fread(buffer, 1, BUFIZE, istream);
            wlen = fcgi_input_multipart_form_data(&form, buffer, rlen);
        }

    }



    return 0;
}