#include <stdio.h>

#define _GNU_SOURCE
#include <string.h>
#include <stdbool.h>

#include <libgen.h>
#include <unistd.h>

#include "hexdump.h"
#include "iostream.h"


#define BUFIZE 512





static int fio_init_multipart_form_data (struct context *ctx, const char *path)
{
    FILE *stream;
    char buffer[BUFIZE + 1];

    unlink(path);
    stream = fopen(path, "wb");
    ctx->stream = stream;

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

    snprintf(buffer, BUFIZE, "--%s\r\n", ctx->content_boundary);
    fwrite(buffer, 1, strlen(buffer), ctx->stream);

    fwrite(data, length, 1, ctx->stream);

    snprintf(buffer, BUFIZE, "\r\n");
    fwrite(buffer, 1, strlen(buffer), ctx->stream);
    return length;
}

static void
fio_exit_multipart_form_data(
    struct context *ctx
)
{
    char buffer[BUFIZE + 1];

    snprintf(buffer, BUFIZE, "--%s--\r\n", ctx->content_boundary);
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
        .content_boundary = CONTENT_BOUNDARY,
    };

    if (0 == strcmp(prog, "iostream")) {
        printf("Nothing\n");
    } else if (0 == strcmp(prog, "istream")) {
        istream = fopen(STREAM_TXT, "r");
        fio_init_multipart_form_data(&ctx, STREAM_BIN);
        while (!feof(istream)) {
            fgets(buffer, BUFIZE, istream);
            if (buffer[strlen(buffer) - 1] == '\n')
                buffer[strlen(buffer) - 1] = '\0';

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