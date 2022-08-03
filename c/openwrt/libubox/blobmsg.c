#include <stdio.h>

#include <libubox/blobmsg.h>
#include <libubox/blobmsg_json.h>


#define error(fmt, ...)     printf(fmt"\n", ## __VA_ARGS__)
#define warning(fmt, ...)   printf(fmt"\n", ## __VA_ARGS__)
#define notice(fmt, ...)    printf(fmt"\n", ## __VA_ARGS__)
#define info(fmt, ...)      printf(fmt"\n", ## __VA_ARGS__)
#define debug(fmt, ...)     printf(fmt"\n", ## __VA_ARGS__)

struct blob_buf blob;



enum {
    CLIENT_ID,
    CLIENT_NAME,
    CLIENT_MAX
};

const struct blobmsg_policy client_policy[CLIENT_MAX] = {
    [CLIENT_ID]     = {.name = "id", .type = BLOBMSG_TYPE_INT32},
    [CLIENT_NAME]    = {.name = "name", .type = BLOBMSG_TYPE_STRING},
};

int main (int argc, char *argv[]) {
    struct blob_buf *b = &blob;
    void *arr1;
    void *tab1;
    struct blob_attr *attr1;
    struct blob_attr *cli[CLIENT_MAX];
    size_t rem;

    blob_buf_init(b, 0);

    /** Add array */
    arr1 = blobmsg_open_array(b, "clients");
    tab1 = blobmsg_open_table(b, NULL);
    blobmsg_add_u32(b, "id", 0);
    blobmsg_close_table(b, tab1);

    tab1 = blobmsg_open_table(b, NULL);
    blobmsg_add_u32(b, "id", 1);
    blobmsg_close_table(b, tab1);




    blobmsg_for_each_attr(attr1, b->head, rem) {
        blobmsg_parse(client_policy, CLIENT_MAX,
            cli, blobmsg_data(attr1), blobmsg_len(attr1));

        info("clientid: %d", blobmsg_get_u32(cli[CLIENT_ID]));
    }

    blobmsg_close_array(b, arr1);


    info("blob:%s", blobmsg_format_json_indent(b->head, true, -1));
    blob_buf_free(b);
    return 0;
}