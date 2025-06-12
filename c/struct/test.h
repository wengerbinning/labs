

/* URL FLAGS */
#define URLF_ATTR_MASK   0x0000FFFF
#define URLF_PROTO_MASK  0x000F0000
#define URLF_TYPE_MASK   0x00F00000
#define URLF_SCOPE_MASK  0x0F000000
union url_flags {
    struct {
        unsigned int attr: 16;
        unsigned int proto: 4;
        unsigned int type:  4;
        unsigned int scope: 4;
    };
    unsigned int values[1];
};

/* url resource attribute api */
#define URL_GET_ATTR(url)   (url->flags.attr)
#define URL_SET_ATTR(url, a)    (URL_GET_ATTR(url) |=  (a))
#define URL_CHECK_ATTR(url, a) ((URL_GET_ATTR(url) &   (a)) && 1)
#define URL_CLEAN_ATTR(url, a)  (URL_GET_ATTR(url) &= ~(a))
/* url protocol api */
#define URL_GET_PROTO(url) (url->flags.proto)
#define URL_SET_PROTO(url, p) (URL_GET_PROTO(url) = (p))
/* url resource type api */
#define URL_GET_TYPE(url)  (url->flags.type)
#define URL_SET_TYPE(url, t)  (URL_GET_TYPE(url) = (t))
/* url resource scope api */
#define URL_GET_SCOPE(url) (url->flags.scope)
#define URL_SET_SCOPE(url, s) (URL_GET_SCOPE(url) = (s))

/* URL ATTRIBUTE */
#define URL_VALID  (0x01 << 0) /* Flag the url is valid  */
#define URL_CHECK  (0x01 << 1) /* Flag the resource will check data */
#define URL_START  (0x01 << 2) /* Flag the resource start request */
#define URL_DATA   (0x01 << 3) /* Flag the resource finish request */
#define URL_AGENT  (0x01 << 4) /* Flag the url need agent */
#define URL_AUTH   (0x01 << 5) /* Flag the url need authority */
#define URL_PROXY  (0x01 << 6) /* Flag the url need proxy */
#define URL_CURL   (0x01 << 7) /* Flag the resource request by libcurl */
/* URL PROTOCOL TYPE */
#define URL_PROTO_TFTP      0
#define URL_PROTO_HTTP      1
#define URL_PROTO_HTTPS     2
#define URL_PROTO_FTP       3
#define URL_PROTO_FILE      4
#define URL_PROTO_SOCKET    5
/* URL RESOURCE TYPE */
#define URL_TYPE_DATA       0
#define URL_TYPE_CONFIG     1
#define URL_TYPE_FIRMWARE   2
#define URL_TYPE_RING       3
/* URL RESOURCE SCOPE */
#define URL_SCOPE_NONE     0
#define URL_SCOPE_DEVICE   1
#define URL_SCOPE_MODEL    2
#define URL_SCOPE_FAMIL    3
#define URL_SCOPE_PLATFORM 4


/* URL_FLAGS APIs */
static inline unsigned int url_flags (int attr, int proto, int type, int scope)
{
    union url_flags flags = {0};
    flags.attr = attr;
    flags.proto = proto;
    flags.type = type;
    flags.scope = scope;
    return flags.values[0];
}

                    #define URLF_NORMAL_CONFIG  url_flags(0, 0, URL_TYPE_CONFIG,   URL_SCOPE_NONE)
                    #define URLF_DEVICE_CONFIG  url_flags(0, 0, URL_TYPE_CONFIG, URL_SCOPE_DEVICE)
                    #define URLF_MODEL_FIRMWARE url_flags(0, 0, URL_TYPE_FIRMWARE, URL_SCOPE_MODEL)
                    #define URLF_FAMIL_FIRMWARE url_flags(0, 0, URL_TYPE_FIRMWARE, URL_SCOPE_FAMIL)