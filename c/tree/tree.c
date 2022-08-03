#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>






struct tree;
typedef struct tree tree_t;

#define TREE_LBORDER 0x01
#define TREF_RBORDER 0x02

struct tree {
	tree_t *prev, *next;
	tree_t *left, *right;

	unsigned int flags, count;
};

tree_t *tree_lborder (tree_t *t) {
	tree_t *temp = t;
	do {
		if (temp->flags | TREE_LBORDER)
			break;
	} while (temp->left && (temp = temp->left));
	return temp;
}

tree_t *tree_rborder (tree_t *t) {
	tree_t *temp = t;
	do {
		if (temp->flags | TREF_RBORDER)
			break;
	} while (temp->right && (temp = temp->right));
	return temp;
}

tree_t *tree_new (void) {
	tree_t *tree;

	if (!(tree = malloc(sizeof(tree_t))))
		return NULL;

	memset(tree, 0, sizeof(tree_t));
	tree->flags = TREE_LBORDER | TREF_RBORDER;

	return tree;
}

tree_t *tree_add (tree_t *t1, tree_t *t2) {
	tree_t *temp;

	// TODO

	//
	t2->right = t1->right;
	t1->right = t2;

	//
	if (t1->next) {
		temp = tree_rborder(t1->next);
		temp->flags &=  ~TREF_RBORDER;
		t2->flags &= ~TREE_LBORDER;
		t2->flags |= TREF_RBORDER;
	} else {
		t1->next = t2;
		t2->flags |= TREE_LBORDER;
	}


	return NULL;
}

//
tree_t *tree_first (tree_t *tree) {
	while (tree && tree->left) {
		tree = tree->left;
	}
	return tree;
}
tree_t *tree_last (tree_t *tree) {
	while (tree && tree->right) {
		tree = tree->right;
	}
	return tree;
}


//
tree_t *tree_prev (tree_t *t) {
	return t->left;
}
tree_t *tree_next (tree_t *t) {
	return t->right;
}

//
void tree_dump (tree_t *t) {
	tree_t *temp = t;
	// TODO - check


	do {
		if (!(temp->flags & TREE_LBORDER))
			printf(" ");

		printf("%p", temp);

		if (temp->flags & TREF_RBORDER)
			printf("\n");

	} while ((temp = tree_next(temp)));

	// printf("%p - prev:%p, next:%p, left:%p, right:%p flags:0x%08X, count:%d\n", tree,
	// 	tree->prev, tree->next, tree->left, tree->right, tree->flags, tree->count);



}

tree_t *root;

int main (int agrc, char *argv[]) {

	tree_t *tree, *t1, *t2;

	tree = tree_new();
	root = tree;

	tree = tree_new();
	tree_add (root, tree);
	t1 = tree;

	tree = tree_new();
	tree_add (root, tree);
	t2 = tree;

	tree = tree_new();
	tree_add (t1, tree);
	tree = tree_new();
	tree_add (t1, tree);

	tree = tree_new();
	tree_add (t2, tree);
	tree = tree_new();
	tree_add (t2, tree);

	tree_dump(root);




	return 0;
}