#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef union frame_flags {
	unsigned int value;
	struct {
		unsigned int root: 1;
	};
} frame_flags_t;


typedef struct frame {
    struct frame *prev, *next;
	frame_flags_t flags;
    unsigned int number, size;
} frame_t;


#define FRAME_INIT(frame) do { \
	(frame)->prev = (frame);   \
	(frame)->next = NULL;      \
} while (0)

frame_t *frame_create () {
	frame_t *frame;

	if (!(frame = malloc(sizeof(frame_t))))
		return NULL;

	memset(frame, 0, sizeof(frame_t));
	FRAME_INIT(frame);

	return frame;
}

void frame_delete (frame_t * frame) {

	if (!frame)
		return;

	if (frame->prev && frame->prev != frame) {
		frame->prev->next = frame->next;
		frame->next->prev = frame->prev;
	}

	frame->prev = frame;
	frame->next = NULL;

	free(frame);
}


#define frame_add(root, frame) do {   \
	if (root) {                       \
		(root)->prev->next = (frame); \
		(frame)->prev = (root)->prev; \
		(root)->prev = (frame);       \
	} else {                          \
		(root) = (frame);             \
	}                                 \
} while(0)



#define frame_pop(root, frame) do { \
	if (root == frame)                   \
		root = frame->next;              \
	else                                 \
		frame->prev->next = frame->next; \
	frame->next->prev = frame->prev;     \
	frame->prev = frame;             \
	frame->next = NULL;              \
} while(0)

#define frame_foreach(frame, root) \
	for (frame_t *frame = root; frame; frame = frame->next)

void frame_dump (frame_t *root) {
	printf("\n");
	frame_foreach(frame, root) {
		printf("Frame: %p prev:%p, next:%p\n", frame, frame->prev, frame->next);
	}
}



int main (int agrc, char *argv[]) {
    frame_t *root = NULL, *frame;
	frame_t *f1, *f2, *f3, *f4, *f5;

	frame = frame_create();
	frame_add(root, frame);
	f1 = frame;

	frame = frame_create();
	frame_add(root, frame);
	f2 = frame;

	frame = frame_create();
	frame_add(root, frame);
	f3 = frame;

	frame = frame_create();
	frame_add(root, frame);
	f4 = frame;

	frame = frame_create();
	frame_add(root, frame);
	f5 = frame;

	frame_dump(root);

	frame_pop(root, f2);

	frame_dump(root);

	frame_add(root, f2);
	frame_dump(root);
    return 0;

}






