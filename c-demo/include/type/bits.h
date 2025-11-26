#ifndef __BITS_H__
#define __BITS_H__

typedef union bit {
	unsigned char v;
	struct {
		unsigned char v1: 1;
		unsigned char v2: 1;
		unsigned char v3: 1;
		unsigned char v4: 1;
		unsigned char v5: 1;
		unsigned char v6: 1;
		unsigned char v7: 1;
		unsigned char v8: 1;
	};
} bit_t;

typedef struct bitmap {
	size_t size;
	bit_t data[];
} bitmap_t;



// bitmap_set(map, n, v)
// bitmap_get(map, n)
// bitmap_dup(map, n)
// bitmap_init(map, n)
// bitmap_exit(map)
// bitmap_dump(map)
// bitmap_copy(src, dst)
// bitmap_find(map, mode)
// bitmap_test(map, n)
// bitmap_flush(map)


typedef unsigned int bitmap_t;
#define bitmap(name) bitmap_ ## name

#define bitmap_chunk (8 * sizeof(bitmap_t))
#define bitmap_size(n) (((n) + (bitmap_chunk - 1)) / bitmap_chunk)
#define BITMAP_INIT(name, size) bitmap_t bitmap(name) [bitmap_size(size)] = { 0 }


#define bitmap_set(name, n, v) bitmap(name)[(n)/bitmap_chunk] |
#define bitmap_get(name, n)

#define bitmap_flush(name)
#define bitmap_count(name) (sizeof(bitmap(name)) * 8)

#endif /* __BITS_H__ */
