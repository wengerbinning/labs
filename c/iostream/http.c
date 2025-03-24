


// ============================================================================
static int
fio_input_multipart_form_data(
    struct fio_context *ctx,
    void *data, size_t dlen
)
// ============================================================================
{

    size_t size, rlen;
    char *boundary;

    char buffer[BUFIZE + 1];
    size_t cofst1, cofst, wofst, wofst1;

    static bool cache;
    static size_t clen;
    static char cbuffer[BUFIZE + 1];


    char *p0, *p1;



    boundary = ctx->content_boundary;


    hexdump(data, dlen);
    Log(LOG_DEBUG, "#########################################");

    /* check cache */
    size_t cofst, dofst;
    // 如果存在cache, 则记录clen的位置;
    // 复制一部分数据到cache
    // 记录复制的数据偏移dofst
    p0 = data;
    rlen = dlen;
    if ( cache ) {
        cofst = clen;
        len = (dlen < (BUFIZE - clen)) ? dlen : (BUFIZE - clen);
        memcpy(cbuffer + cofst, data, len);
        clen += len;
        dofst = len;
        p0 = cbuffer;
        rlen = clen;
    }

    /* check boundary */
    check_boundary:
    while ( (p1 = (char *)memmem(p0, rlen, boundary, strlen(boundary))) ) {
        size  = p1 - p0;
        rlen -= size;

        /* */
        if ((4 <= size) && (0 == strncmp(p1 - 4, "\r\n--", 4))) {
            memset(p1 - 4, 0, 2);
            size -= 4;
        } else if ( (2 <= size) && 0 == strncmp(p1 - 2, "--", 2) ) {
            /* first data */
            size -= 2;
        }

        /* write form data end */
        if ( 0 < size ) {
            hexdump(p0, size);
        }

        /* next form */
        p0 = p1 + strlen(boundary);
        rlen -= strlen(boundary);
        if ((2 < len) && (0 == strncmp(p0, "\r\n", 2))) {
            /* skip boundary suffix: \r\n */
            p0 += 2; len -= 2;
        }

        /* update cache */
        if (cache && (p0 < (cbuffer + cofst))) {
            cofst = (cbuffer + cofst) - p0;
            len = (cbuffer + clen) - p0;
            memcpy(buffer, p0, len);
            memcpy(cbuffer, buffer, len);
            cache = true;
            clen = len;

            size = BUFIZE - clen;
            /* copy data s*/
            len = ((dlen - dofst) < size) ? ((dlen - dofst)) : size;
            memcpy(cbuffer + clen, (data + dofst), len);
            clen += len;
            /* update pointer */
            p0 = cbuffer;
            rlen = clen;
        } else if (cache) {
            dofst += p0 - (cbuffer + cofst);
            p0 = data + (dofst);
            rlen = dlen - (dofst);
            cache = false;
        }
    }

    /* check end */
    p0 = cache ? cbuffer : (data + dofst);
    rlen = cache ? clen : (dlen - dofst);
    if ( (rlen == 4) && 0 == strncmp(p0, "--\r\n", 4) ) {
        Log(LOG_DEBUG, "==== END ====");
    } else if (0 < len) {
        hexdump(p0, len);
    }

    /* update cache */
    if ( cache && BUFIZE <= clen ) {
        len = clen;
        if ( strlen(boundary) <= BUFIZE ) {
            len -= strlen(boundary);
        }
        /* */
        if ( form_data_write(&form, cbuffer, clen) != clen ) {
            form_data_exit( &form );
            ret = ctx->retVal;
            Log(LOG_DEBUG, "return code %d", ret);
            goto exit0;
        }

        form.start = false;
        cache = false;
        /* */
        if ( strlen(boundary) <= BUFIZE ) {
            len = strlen(boundary);
            ofst = clen - len;

            memcpy(buffer, cbuffer + ofst, len);
            memcpy(cbuffer, buffer, len);
            cache = true;
            clen = len;
        }
        cofst = clen;
    }

    /* cache data */
    cofst = cache ? clen : 0;
    size = cache ? BUFIZE - cofst : BUFIZE;
    len = (size < (dlen - dofst)) ? size : (dlen - dofst);
    if (0 < len) {
        memcpy((cbuffer + cofst), (data + dofst), len);
        cache = true;
        clen += len;
        dofst += len;
        p0 = cbuffer;
        len = clen;
        goto check_boundary;
    }

    return 0;
}