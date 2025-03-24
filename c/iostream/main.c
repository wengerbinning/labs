#include <stdio.h>
#include <stdlib.h>

/*
0-9A-F

16
16 * 16
16 * 16 * 16


00-FF

0000 - FFFF
000000 - FFFFFF
 */



int main(int argc, char *argv[]) {
	int i, max, wid, size;
	char fmt[256] = {0};

	if (argv[1])
		size = atoi(argv[1]);


	/* */
	wid = 1;
	max = size - 1;
	while ((max >>= 8)) wid++;
	sprintf(fmt, "%%0%dX%%c", (wid * 2));
	for (i = 0; i< size; i++) {
		char chr;
		chr = (i && !((i + 1) % 16)) ? '\n': ' ';
		printf(fmt, i, chr);
	}
	if (i && (i % 16))
		printf("\n");

	return 0;
}