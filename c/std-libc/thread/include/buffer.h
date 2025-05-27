#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "list.h"

#define BUFIZE 512

typedef struct block_t {
	struct list_head list;
	char *data[BUFIZE + 1];
	unsigned int size;
} block_t;

typedef struct buffer_t {
	struct list_head blk_list;
	unsigned int count;
	unsigned int size;
	block_t *blk;
} buffer_t;




buffer_t *buffer_new (int size, int count);
void buffer_free (buffer_t *buffer);
void buffer_hexdump (buffer_t * buffer);

#endif /* __BUFFER_H__ */