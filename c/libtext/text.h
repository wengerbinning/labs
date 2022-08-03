


struct blobmsg_policy {
	const char *name;
	enum blobmsg_type type;
};



struct blobmsg_hdr {
	uint16_t namelen;
	uint8_t name[];
} __packed;


struct blob_attr {
	uint32_t id_len;
	char data[];
} __packed;







unsigned int blob_id (const struct blob_attr *attr)
bool blob_is_extended (const struct blob_attr *attr)
size_t blob_len (const struct blob_attr *attr)
size_t blob_raw_len (const struct blob_attr *attr)
size_t blob_pad_len (const struct blob_attr *attr)

void *blob_data (const struct blob_attr *attr)
uint8_t blob_get_u8 (const struct blob_attr *attr)
uint16_t blob_get_u16 (const struct blob_attr *attr)
uint32_t blob_get_u32 (const struct blob_attr *attr)
uint64_t blob_get_u64 (const struct blob_attr *attr)
int8_t blob_get_int8 (const struct blob_attr *attr)
int16_t blob_get_int16 (const struct blob_attr *attr)
int32_t blob_get_int32 (const struct blob_attr *attr)
int64_t blob_get_int64 (const struct blob_attr *attr)
const char * blob_get_string (const struct blob_attr *attr)

struct blob_attr *blob_next (const struct blob_attr *attr)




int blobmsg_buf_init(struct blob_buf *buf)
int blobmsg_add_blob(struct blob_buf *buf, struct blob_attr *attr)



struct blob_buf {
	struct blob_attr *head;
	bool (*grow)(struct blob_buf *buf, int minlen);
	int buflen;
	void *buf;
};

static void blob_init (struct blob_attr *attr, int id, unsigned int len)
static bool blob_buffer_grow (struct blob_buf *buf, int minlen) - - buf大小对256向上取整并重新分配。
static struct blob_attr *offset_to_attr (struct blob_buf *buf, int offset)
static struct blob_attr *blob_add (struct blob_buf *buf, struct blob_attr *pos, int id, int payload)
static int attr_to_offset (struct blob_buf *buf, struct blob_attr *attr)
static int blob_parse_attr (struct blob_attr *attr, size_t attr_len, struct blob_attr **data, const struct blob_attr_info *info, int max)

void blob_buf_free (struct blob_buf *buf)
void blob_nest_end (struct blob_buf *buf, void *cookie)
void blob_set_raw_len (struct blob_attr *attr, unsigned int len)
void blob_fill_pad (struct blob_attr *attr)
bool blob_buf_grow (struct blob_buf *buf, int required)
bool blob_attr_equal (const struct blob_attr *a1, const struct blob_attr *a2)
bool blob_check_type (const void *ptr, unsigned int len, int type)
int blob_buf_init (struct blob_buf *buf, int id)
int blob_parse_untrusted (struct blob_attr *attr, size_t attr_len, struct blob_attr **data, const struct blob_attr_info *info, int max)
int blob_parse (struct blob_attr *attr, struct blob_attr **data, const struct blob_attr_info *info, int max)
void *blob_nest_start (struct blob_buf *buf, int id)e
struct blob_attr *blob_new (struct blob_buf *buf, int id, int payload)
struct blob_attr *blob_put (struct blob_buf *buf, int id, const void *ptr, unsigned int len)
struct blob_attr *blob_put_raw (struct blob_buf *buf, const void *ptr, unsigned int len)
struct blob_attr *blob_memdup (const struct blob_attr *attr)




blob_buf_init
blob_buf_free

blob_nest_start
blob_nest_end
blob_put

blob_parse