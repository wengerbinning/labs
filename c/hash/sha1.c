#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/*
 * 1. 摘要为160比特（20字节）的
 * 2. 数据按照32比特处理。
 */

#define LEFTROTATE(x, c)  (((x) << (c)) | ((x) >> (32 - (c))))

typedef struct sha1_context {
	uint8_t data[64];
	uint32_t datalen;
	unsigned long long bitlen;
	uint32_t state[5];
} sha1_context_t;


uint32_t K[4] = { 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xCA62C1D6 };

void sha1_transform (sha1_context_t *ctx, const uint8_t data[]) {
	uint32_t a, b, c, d, e, f, k, temp;
	uint32_t w[80];
	int i;

	for (i = 0; i < 16; ++i)
		w[i] = (data[i * 4] << 24) | (data[i * 4 + 1] << 16) | (data[i * 4 + 2] << 8) | (data[i * 4 + 3]);

	for (; i < 80; ++i)
		w[i] = LEFTROTATE(w[i - 3] ^ w[i - 8] ^ w[i - 14] ^ w[i - 16], 1);

	a = ctx->state[0];
	b = ctx->state[1];
	c = ctx->state[2];
	d = ctx->state[3];
	e = ctx->state[4];

	for (i = 0; i < 80; ++i) {
		if (i < 20) {
			f = (b & c) | (~b & d);
			k = K[0];
		} else if (i < 40) {
			f = b ^ c ^ d;
			k = K[1];
		} else if (i < 60) {
			f = (b & c) | (b & d) | (c & d);
			k = K[2];
		} else {
			f = b ^ c ^ d;
			k = K[3];
		}

		temp = LEFTROTATE(a, 5) + f + e + k + w[i];
		e = d;
		d = c;
		c = LEFTROTATE(b, 30);
		b = a;
		a = temp;
	}

	ctx->state[0] += a;
	ctx->state[1] += b;
	ctx->state[2] += c;
	ctx->state[3] += d;
	ctx->state[4] += e;
}

void sha1_init (sha1_context_t *ctx) {
	ctx->datalen = 0;
	ctx->bitlen = 0;

	ctx->state[0] = 0x67452301;
	ctx->state[1] = 0xEFCDAB89;
	ctx->state[2] = 0x98BADCFE;
	ctx->state[3] = 0x10325476;
	ctx->state[4] = 0xC3D2E1F0;
}

void sha1_update (sha1_context_t *ctx, const uint8_t data[], size_t len) {
	for (size_t i = 0; i < len; ++i) {
		ctx->data[ctx->datalen] = data[i];
		ctx->datalen++;

		if (ctx->datalen == 64) {
			sha1_transform(ctx, ctx->data);
			ctx->bitlen += 512;
			ctx->datalen = 0;
		}
	}
}

void sha1_final (SHA1_CTX *ctx, uint8_t hash[]) {
	size_t i = ctx->datalen;

	// Pad whatever data is left in the buffer.
	if (ctx->datalen < 56) {
		ctx->data[i++] = 0x80;
		while (i < 56)
			ctx->data[i++] = 0x00;
	} else {
		ctx->data[i++] = 0x80;
		while (i < 64)
			ctx->data[i++] = 0x00;

		sha1_transform(ctx, ctx->data);
		memset(ctx->data, 0, 56);
	}

    // Append to the padding the total message's length in bits and transform.
	ctx->bitlen += ctx->datalen * 8;
	ctx->data[63] = ctx->bitlen;
	ctx->data[62] = ctx->bitlen >> 8;
	ctx->data[61] = ctx->bitlen >> 16;
	ctx->data[60] = ctx->bitlen >> 24;
	ctx->data[59] = ctx->bitlen >> 32;
	ctx->data[58] = ctx->bitlen >> 40;
	ctx->data[57] = ctx->bitlen >> 48;
	ctx->data[56] = ctx->bitlen >> 56;
	sha1_transform(ctx, ctx->data);

	// Since this implementation uses little endian byte ordering and MD uses big endian,
	// reverse all the bytes when copying the final state to the output hash.
	for (i = 0; i < 4; ++i) {
		hash[i] = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 4] = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 8] = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
	}
}

int main() {
	uint8_t hash[20];
	sha1_context_t ctx;
	char *message = "hello";

	sha1_init(&ctx);
	sha1_update(&ctx, (uint8_t*)message, strlen(message));
	sha1_final(&ctx, hash);

	printf("SHA1 hash: ");
	for (int i = 0; i < 20; i++) {
		printf("%02x", hash[i]);
	}
	printf("\n");

	return 0;
}
