#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CTX_DATA_CACHE 0x01
typedef struct data {
	unsigned int flags;
	size_t size, offset, cache;
	void *buf;
} data_t;


#define CTX_CACHE_ENABLE 0x01
typedef struct cache {
	unsigned int flags;
	size_t size, maxsize;
	size_t length, offset;
	void *buf;
} cache_t;

typedef int (*handle_t) (void *priv, void *data, size_t size);

#define CTX_START	0x01
typedef struct context {
	unsigned int flags;
	size_t size, length;
	void *priv;
	handle_t handle;
	data_t data;
	cache_t cache;
} context_t;


#define ctx_flags(ctx) ctx->flags
#define ctx_data_cache(ctx) (ctx->data.flags & CTX_DATA_CACHE)
#define ctx_test_cache(ctx) (ctx->cache.flags & CTX_CACHE_ENABLE)

static int ctx_data_init (context_t *ctx, void *data, size_t size) {
	if (!(ctx_flags(ctx) | CTX_START)) {
		ctx_flags(ctx) |= CTX_START;
	}

	memset(&ctx->data, 0, sizeof(ctx->data));
	ctx->data.buf = data;
	ctx->data.size = size;

	return 0;
}

static int ctx_handle (context_t *ctx, void *data, size_t size) {
	int ret = size;
	size_t offset = 0;



	if (ctx && ctx->handle) {
		while ((ret = ctx->handle(ctx->priv, (data + offset), size)) != size) {
			if (ret <= 0)
				break;
			offset = ret;
			data += offset;
			size -= offset;
			if (ctx_test_cache(ctx)) {
				ctx->cache.offset += offset;
			} else {
				ctx->data.offset += offset;
			}
			ctx->length += offset;
		}
	}

	if (0 < ret) {
		offset = ret;
		if (ctx_test_cache(ctx)) {
			ctx->cache.offset += offset;


			printf("cache %ld/%ld \n", offset, ctx->cache.offset);



		} else {
			ctx->data.offset += offset;
		}
		ctx->length += offset;
	}




	return ret;
}

static void *ctx_data (context_t *ctx) {
	if (ctx) {
		if (ctx_test_cache(ctx))
			return ctx->cache.buf;
		return ctx->data.buf + ctx->data.offset;
	}
	return NULL;
}

static size_t ctx_size (context_t *ctx) {
	if (ctx) {
		if (ctx_test_cache(ctx))
			return ctx->cache.size;
		return (ctx->data.size - ctx->data.offset);
	}
	return 0;
}

static int ctx_sync_cache (context_t *ctx) {
	void *src, *dst;

	if (ctx_test_cache(ctx)) {
		data_t *data = &ctx->data;
		cache_t *cache = &ctx->cache;
		size_t size, dsize, csize;
		dsize = data->size - data->offset;
		csize = cache->maxsize - cache->length;
		if (0 < dsize && 0 < csize) {
			size = (csize < dsize) ?  dsize : dsize;
			if (cache->size < (cache->length + size)) {
				cache->buf = realloc(cache->buf, (cache->length + size + 1));
				cache->size = cache->length + size;
			}
			src = data->buf + data->offset;
			dst = cache->buf + cache->length;
			memcpy(src, dst, size);
			data->cache = data->offset + size;
		}
	}

	if (ctx_data_cache(ctx)) {
		size_t size, csize, dsize;
		data_t *data = &ctx->data;
		cache_t *cache = &ctx->cache;

		if (!ctx_test_cache(ctx)) {
			cache->offset = 0;
			cache->length = 0;
		}

		size = data->size - data->cache;
		if (cache->maxsize && (cache->maxsize - cache->length) < size) {
			return -1;
		}

		if (!cache->buf) {
			// printf("malloc %d memory \n", size + 1);
			cache->buf = malloc(size + 1);
			cache->size = size;
			memset(cache->buf, 0, size + 1);
		} else if (cache->size < (cache->length + size)) {
			cache->buf = realloc(cache->buf, (cache->length + size + 1));
			cache->size = (cache->length + size);
		}
		dst = data->buf + data->cache;
		src = cache->buf + cache->length;
		memcpy(dst, src, size);
		cache->flags |= CTX_CACHE_ENABLE;
		cache->size = cache->length + size;
		cache->length = cache->size;
	}

	return 0;
}


static int ctx_cache (context_t *ctx, size_t offset) {
	data_t *data;

	if (!ctx)
		return -1;

	data = &ctx->data;
	data->flags |= CTX_DATA_CACHE;
	data->cache  = offset;
	ctx_sync_cache(ctx);

	return 0;
}


// CTX_CACHE_TEST(ctx)
// CTX_CACHE_SYNC(ctx)
// CTX_CACHE_SECR(ctx) security
// CTX_CACHE_DATA(ctx)
// CTX_CACHE_SIZE(ctx)

// CTX_DATA(ctx)
// CTX_SIZE(ctx)
// CTX_CACHE(ctx, offset)

int context_handle (void *priv, void *data, size_t size) {
	int ret;
	size_t offset = 0;
	context_t *ctx;


	if (!priv)
		return -1;

	ctx = (context_t *)priv;
	ctx_data_init(ctx, data, size);

	while (ctx_test_cache(ctx)) {
		// if (!CTX_CACHE_SECR(ctx)) {
		// 	return -1;
		// }
		if ((ret = ctx_handle(ctx, ctx_data(ctx), ctx_size(ctx))) <= 0) {
			return ret;
		}
		ctx_sync_cache(ctx);
	}

	// if ((ret = ctx_handle(ctx, ctx_data(ctx), ctx_size(ctx))) <= 0) {
	// 	printf("%s:%d: RUNNING IN HERE %p\n", __func__, __LINE__, data);
	// 	return ret;
	// }

	ctx_cache(ctx, offset);

	return size;
}




int main (int agrc, char *argv[]) {
	int ret;
	context_t context = { 0 }, *ctx=&context;


	ret = context_handle(ctx, (void *)&context, 10);

	printf("handle:%ld/%ld, return code : %d\n", ctx->length, ctx->size, ret);
	printf(" -  data: %14p (%04ld) - %04ld/%04ld:%04ld %s\n",
		ctx->data.buf, ctx->data.size,
		ctx->data.offset, ctx->data.cache, ctx->data.size,
		ctx_data_cache(ctx) ? "true" : "false");
	printf(" - cache: %14p (%04ld) - %04ld/%04ld:%04ld %s\n",
		ctx->cache.buf, ctx->cache.maxsize,
		ctx->cache.offset, ctx->cache.length, ctx->cache.size,
		ctx_test_cache(ctx) ? "true" : "false");


	ret = context_handle(ctx, (void *)&context, 10);

	printf("handle:%ld/%ld, return code : %d\n", ctx->length, ctx->size, ret);
	printf(" -  data: %14p (%04ld) - %04ld/%04ld:%04ld %s\n",
		ctx->data.buf, ctx->data.size,
		ctx->data.offset, ctx->data.cache, ctx->data.size,
		ctx_data_cache(ctx) ? "true" : "false");
	printf(" - cache: %14p (%04ld) - %04ld/%04ld:%04ld %s\n",
		ctx->cache.buf, ctx->cache.maxsize,
		ctx->cache.offset, ctx->cache.length, ctx->cache.size,
		ctx_test_cache(ctx) ? "true" : "false");
	return 0;
}