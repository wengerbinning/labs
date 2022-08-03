#ifndef __URL_H__
#define __URL_H__

#include <stddef.h>
#include "list.h"

#define URL_MAXNAME     64
struct url_t {
    struct list_head list;
    char *url;
    unsigned int id;
    char name[URL_MAXNAME + 1];
    unsigned int flags; /* 32bit flags */

#define URLF_VALID  (0x01 << 0)
#define URLF_RANGE  (0x01 << 1)

#define URLF_PROTO  (0x01 << 9)
#define URLF_PROTO_HTTP    (0x01 << 10)
#define URLF_PROTO_HTTPS   (0x01 << 11)
#define URLF_PROTO_FTP     (0x01 << 12)
#define URLF_PROTO_TFTP    (0x01 << 13)
#define URLF_PROTO_FILE    (0x01 << 14)
};


struct url_t * url_new (char *url);
void url_free (struct url_t * url);

int url_check (struct url_t * url);

#endif /* __URL_H__ */