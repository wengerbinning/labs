#include

typedef struct tree {
	tree_t *parent, *childs;
	tree_t *prev, *next;

	unsigned int flags, count;
} tree_t;