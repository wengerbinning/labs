#include <stdio.h>

#define _GNU_SOURCE
#include <string.h>

#include <stdbool.h>

#include <libgen.h>
#include <unistd.h>

#include "hexdump.h"


#define STREAM_TXT "data.txt"
#define STREAM_BIN "data.bin"
#define BUFIZE 512

struct context {
    char *boundary;
    FILE *stream;
};



// ============================================================================
static int
fio_input_multipart_form_data(
    struct context *ctx,
    void *data,
    size_t length
)
// ============================================================================
{
    char *p0, *p1;
    size_t len, clen, size;
    size_t dofst, wofst, cofst1;
    char *boundary;
    char  buffer[BUFIZE + 1];
    static bool cache;
    static size_t  cofst;
    static char cbuffer[BUFIZE + 1];
    size_t ofst;

    boundary = ctx->boundary;

    p0 = data;
    dofst = 0;
    len = length;


    /* check cache */
    if (cache) {
        size = BUFIZE - cofst;
        clen = (size < length) ? size : length;
        memcpy(cbuffer + cofst, (data + dofst), clen);
        cofst1 = cofst;
        cofst += clen;

        dofst = wofst;
        wofst = clen;

        p0 = cbuffer;
        len = cofst;
    }

check_boundary:
    while ( (p1 = (char *)memmem(p0, len, boundary, strlen(boundary))) ) {
        size = p1 - p0;
        len -= size;

        if ((4 <= size) && (0 == strncmp(p1 - 4, "\r\n--", 4))) {
            memset(p1 - 4, 0, 2);
            size -= 4;
        } else if ( (2 <= size) && 0 == strncmp(p1 - 2, "--", 2) ) {
            size -= 2;
        }

        /* */
        if ( 0 < size ) {
            hexdump(p0, size);
        }

        /* */
        p0 = p1 + strlen(boundary);
        len -= strlen(boundary);

        /* */
        if ((2 < len) && (0 == strncmp(p0, "\r\n", 2))) {
            p0 += 2; len -= 2;
        }

        /* */
        // if (cache && ((p0 - cbuffer) < cofst1) ) {
        //     ofst = (p0 - cbuffer);
        //     clen = cofst - ofst;
        //     cofst1 -= ofst;
        //     cofst -= ofst;
        //     memcpy(buffer, p0, clen);
        //     memcpy(cbuffer, buffer, clen);
        //     p0 = cbuffer;
        //     len = cofst;
        // } else {
        //     if (cache) {
        //         ofst = (p0 - cbuffer) - cofst1;
        //         p0 = data + (dofst + ofst);
        //         len = length - (dofst + ofst);
        //     }

        //     cache = false;
        //     cofst = 0;
        //     wofst = p0 - data;
        // }


        // dofst = p0 - ;
    }

    p0 = cache ? cbuffer : (data + dofst);
    len = cache ? cofst : (length - dofst);
    if ( len <= 0) {
        return length; // next;
    } else if ( (4 == len) && 0 == strncmp(p0, "--\r\n", 4) ) {
        printf("==== END ====\n");
        // break;
    }

    /* write cache */
    if (cache && BUFIZE <= cofst) {
        cache = false;
        clen = (strlen(boundary) <= BUFIZE) ? cofst : cofst - strlen(boundary);

        hexdump(cbuffer, clen);
        // form.start = false;

        if (strlen(boundary) <= BUFIZE) {
            size = strlen(boundary);
            memcpy(buffer, cbuffer + clen, size);
            memcpy(cbuffer, buffer, size);
            cache = true;
            cofst = size;
        }
    }

    /* cache data */
    cofst1 = cache ? cofst : 0;
    cofst = cofst1;
    ofst = cache ? wofst : dofst;
    size = cache ? (BUFIZE - cofst) : BUFIZE;
    clen = size < (length - ofst) ? size : (length - ofst);
    if (0 < clen) {
        memcpy(cbuffer + cofst, data + ofst, clen);
        cache = true;
        cofst += clen;

        p0 = cbuffer;
        len = cofst;
        wofst += clen;

        goto check_boundary;
    }

    return length;
}

static int fio_init_multipart_form_data (struct context *ctx, const char *path)
{
    FILE *stream;
    char buffer[BUFIZE + 1];

    unlink(path);
    stream = fopen(path, "wb");
    ctx->stream = stream;
    snprintf(buffer, BUFIZE, "--%s", ctx->boundary);
    fwrite(buffer, 1, strlen(buffer), stream);

    return 0;
}

static int
fio_output_multipart_form_data(
    struct context *ctx,
    void *data,
    size_t length
)
{
    char buffer[BUFIZE + 1];
    fwrite(data, length, 1, ctx->stream);
    snprintf(buffer, BUFIZE, "\r\n--%s", ctx->boundary);
    fwrite(buffer, 1, strlen(buffer), ctx->stream);

    return 0;
}

static void
fio_exit_multipart_form_data(
    struct context *ctx
)
{
    char buffer[BUFIZE + 1];

    snprintf(buffer, BUFIZE, "\r\n");
    fwrite(buffer, 1, strlen(buffer), ctx->stream);

    if (ctx->stream)
        fclose(ctx->stream);

}

int main (int argc, char *argv[]) {
    FILE *istream, *ostream;
    char buffer[BUFIZE +1];
    size_t rlen, wlen;
    char *prog = basename(argv[0]);
    struct context ctx = {
        .boundary = "boundary",
    };

    if (0 == strcmp(prog, "stream")) {
        printf("Nothing\n");
    } else if (0 == strcmp(prog, "istream")) {
        istream = fopen(STREAM_TXT, "r");
        fio_init_multipart_form_data(&ctx, STREAM_BIN);
        while (!feof(istream)) {
            fgets(buffer, BUFIZE, istream);
            hexdump(buffer, strlen(buffer));
            fio_output_multipart_form_data(&ctx, buffer, strlen(buffer));
        }
        fio_exit_multipart_form_data(&ctx);
    } else if (0 == strcmp(prog, "ostream")) {
        istream = fopen(STREAM_BIN, "rb");

        while (!feof(istream)) {
            rlen = fread(buffer, 1, BUFIZE, istream);
            hexdump(buffer, rlen);
            // wlen = fio_input_multipart_form_data(&ctx, buffer, rlen);
        }

    }

    return 0;
}