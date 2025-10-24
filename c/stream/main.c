
#include <stdio.h>
#include <string.h>

typedef int (*stream_handle_t) (void *ctx, void *data, size_t size);
typedef struct stream_context stream_context_t;

#define SCTX_EOF   0x01 /* RW - 标识数据流结束 */
#define SCTX_SYNC  0x02 /* RW - 标识同步数据流 */
#define SCTX_FLUSH 0x03 /* RW - 标识刷新数据流 */
#define SCTX_CACHE 0x04 /* RW - 标识缓存数据流 */
#define SCTX_TRUNC 0x05 /* RW - 标识截断数据流 */

#define STREAM_BLKIZE 6
#define STREAM_BUFIZE STREAM_BLKIZE * 1
struct stream_context {
	unsigned int flags;
	void *priv;
	stream_handle_t handle;
	size_t offset, length;
	char buffer[STREAM_BUFIZE + 1];
	char *buf;
};

#define min(a, b) ((a) < (b) ? (a) : (b))
#define stream_cache_size(ctx) (((ctx)->buf - (ctx)->buffer))
#define stream_cache_pool(ctx) (STREAM_BUFIZE - stream_cache_size(ctx))


int demo_handle (void *ctx, void *data, size_t size) {
	char buffer[STREAM_BLKIZE + 1];

	memcpy(buffer, data, STREAM_BLKIZE);
	printf("handle %p:%02ld/%02ld - %s ...\n", data, STREAM_BLKIZE, size, buffer);


	return STREAM_BLKIZE;
}

int _stream_cache (stream_context_t *ctx, void *data, size_t size) {
	char *buf;
	size_t len;
	size_t pool;

	ctx->buf = ctx->buf ? ctx->buf : ctx->buffer;

	buf = data;
	len = min(size, stream_cache_pool(ctx));
	memcpy(ctx->buf, buf, len);
	ctx->buf += len;
	// printf (" cache %p:%02ld/%02ld - %s...\n",
	// 	ctx->buffer, len, size, ctx->buffer);

	return len;
}


int stream_cache (stream_context_t *ctx, void *data, size_t size) {
	return _stream_cache(ctx, data, size);
}

#define stream_sync(ctx) do { \
	ctx->flags |= SCTX_SYNC; \
	stream_flush_cache(ctx, NULL, 0); \
	ctx->flags &= !SCTX_SYNC; \
} while(0)

int stream_flush_cache (stream_context_t *ctx, void *data, size_t size) {
	int ret;
	char *buf;
	size_t len;
	size_t cache;

	//
	ctx->buf = ctx->buf ? ctx->buf : ctx->buffer;
	if ((len = stream_cache_size(ctx)) <= 0)
		return 0;

	//
	if (data && 0 < size && (ret = _stream_cache(ctx, data, size)) < 0)
		return ret;

	//
	cache = ret;
	if (0 < stream_cache_pool(ctx)) {
		if (ctx->flags & SCTX_EOF)
			ctx->flags |= SCTX_FLUSH;
		if (ctx->flags & SCTX_SYNC)
			ctx->flags |= SCTX_FLUSH;
		if (!(ctx->flags & SCTX_FLUSH))
			return cache;
	}

	//
	buf = ctx->buffer;
	ctx->flags &= ~SCTX_FLUSH;
	len = stream_cache_size(ctx);
	if (ctx->handle && (ret = ctx->handle(ctx->priv, buf, len)) != len) {
		int i;
		buf += ret;
		len -= ret;
		for (i = 0; i < len; i++) {
			ctx->buffer[i] = buf[i];
		}
		ctx->buffer[i] = 0;
		ctx->buf = ctx->buffer + i;
		// printf("flush1 %p:%02ld/%02ld - %s\n",
		// 	ctx->buffer, (buf - ctx->buffer), stream_cache_size(ctx), ctx->buffer);
	} else {
		ctx->buffer[0] = 0;
		ctx->buf = ctx->buffer;
		// printf("flush2 %p:%02ld/%02ld - %s\n",
		// 	ctx->buffer, stream_cache_size(ctx), len, ctx->buffer);
	}

	return cache;
}

int stream_handle (stream_context_t *ctx, void *data, size_t size) {
	int ret;
	char *buf;
	size_t len;

	buf = data;
	len = size;
	while (stream_cache_size(ctx) && 0 < len) {
		if ((ret = stream_flush_cache(ctx, buf, len)) < 0)
			return -1;
		buf += ret;
		len -= ret;
	}

	if (ctx->handle && 0 < len) {
		if ((ret = ctx->handle(ctx->priv, buf, len)) <= 0)
			return ret;

		buf += ret;
		len -= ret;
		if (0 < len && (ctx->flags & SCTX_CACHE)) {
			if ((ret = stream_cache(ctx, buf, len)) <= 0) {
				return ret;
			}
			buf += ret;
			len -= ret;
		}
	}

	ctx->offset += (size - len);
	return (size - len);
}


int funcs (stream_context_t *ctx, void *data, size_t size) {
	int ret;
	void *buf;
	size_t len;

	printf("- %02ld:%s\n", size, data);
	buf = data;
	len = size;

	/* A. */
	// if ((ret = stream_handle(ctx, buf, len)) < 0) {
	// 	printf("error: stream handle fail: %d\n", ret);
	// 	return ret;
	// } else {
	// 	buf += ret;
	// 	len -= ret;
	// 	if (0 < len)
	// 		printf("ignore %p:%02ld/%02ld - %s\n", buf, ret, len, buf);
	// }

	/* B. */
	while (0 <= (ret = stream_handle(ctx, buf, len))) {
		buf += ret;
		len -= ret;
		if (len <= 0)
			break;
	}
	if (ret < 0) {
		printf("error: stream handle fail: %d\n", ret);
		return ret;
	}

	printf("stream: %2ld/%02ld\n", ctx->offset - stream_cache_size(ctx), ctx->offset);
	return 0;
}


int main (int argc, char *argv[]) {
	int ret;
	void *buf;
	size_t len;
	stream_context_t context = { 0 }, *ctx = &context;
	char *str1 = "0123456789";
	char *str2 = "abcdefghijklmnopqrstuvwxyz";


	ctx->handle = demo_handle;
	ctx->flags |= SCTX_CACHE;

	printf("stream: %08X\n", ctx->flags);



	funcs(ctx, str1, strlen(str1));
	funcs(ctx, str2, strlen(str2));
	stream_sync(ctx);

	printf("stream: %ld/%ld\n", ctx->offset - stream_cache_size(ctx), ctx->offset);
	return 0;
}