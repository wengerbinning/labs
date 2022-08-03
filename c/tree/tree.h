#include <stdio.h>

struct tree;
typedef struct tree tree_t;

struct tree {
	tree_t *parent, *childs;
	tree_t *prev, *next;

	unsigned int flags, count;
};






* Blance
*



int main (int agrc, char *argv[]) {

	return 0;
}