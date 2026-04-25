#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define min(a, b) ((b) < (a) ? (b) : (a))
#define max(a, b) ((b) < (a) ? (a) : (b))
#define ceil(m, n) ((((m) + (n) - 1)/(n)) * (n))
#define floor(m, n) (((m)/(n)) * (n))


/*
 * 最大比特： 42亿(4294967296)
 * 消耗内存： 512M(536870912)

 */
#define BITSIZE 32
typedef unsigned int bits_t;
typedef struct bitmap {
	size_t size;
	size_t used;
	bits_t data[];
} bitmap_t;

bitmap_t *bitmap_init (bitmap_t *map, size_t n) {
	bitmap_t *tmp;
	size_t oldcnt, newcnt;

	newcnt = ceil(n, BITSIZE) / BITSIZE;
	oldcnt = map ? ceil(map->size, BITSIZE) / BITSIZE : 0;
	newcnt = ((newcnt * BITSIZE)  < n) ? floor(n, BITSIZE) / BITSIZE : newcnt;

	if (map && oldcnt < newcnt) {
		tmp = realloc(map, sizeof(bitmap_t) + newcnt * sizeof(bits_t));
	} else if (!map) {
		tmp = malloc(sizeof(bitmap_t) + newcnt * sizeof(bits_t));
	}
	if (!tmp)
		return NULL;

	memset(tmp, 0, sizeof(bitmap_t) + newcnt * sizeof(bits_t));
	tmp->size = max(oldcnt, newcnt) * BITSIZE;
	tmp->used = n;
	return tmp;
}

void bitmap_dump (bitmap_t *map) {
	if (map) {
		int count;
		size_t bit = ceil(map->size, BITSIZE);
		size_t cnt = bit / BITSIZE;
		printf("%p:map[%lu] %lu/%lu\n", map, cnt, map->used, map->size);
		count = min((map->size / BITSIZE), 16);
		for (size_t i = 0; i < count; i++) {
			printf(" - %02lu: %08X", i, map->data[i]);
			printf("\n");
		}
		if (16 < (map->size / BITSIZE))
			printf("   ...\n");
	} else {
		printf("%p:map[0]\n", map);
	}
}


int main (int argc, char *argv[]) {
	bitmap_t *map;

	map = bitmap_init(NULL, 54);
	bitmap_dump(map);
	map = bitmap_init(map, 125);
	bitmap_dump(map);
	map = bitmap_init(map, 0x100000000);
	bitmap_dump(map);




	printf("size_t %ld %lu\n", sizeof(size_t), (size_t)0xFFFFFFFFFFFFFFFF);
	return 0;
}
