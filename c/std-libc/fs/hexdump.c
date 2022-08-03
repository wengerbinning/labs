#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "hexdump.h"

#define HEXDUMP_COLUM	16
#define HEXDUMP_CHAR1	' '
#define HEXDUMP_ASCII	false
#define HEXDUMP_CHAR2	' '

void hexdump (const void *ptr, unsigned int size) {
	int i;
	char *buf1, *p;
	char *buf2, *q;
	const char *env;
	char chr1, chr2;
	int col;
	bool ascii __attribute__((unused));

	if (!ptr)
		return;

	env = getenv("HEXDUMP_COLUM");
	col = env ? atoi(env) : HEXDUMP_COLUM;
	env = getenv("HEXDUMP_CHAR1");
	chr1 = env ? *(char *)env : HEXDUMP_CHAR1;
	env = getenv("HEXDUMP_ASCII");
	ascii = env ? str2bool(env) : HEXDUMP_ASCII;
	env = getenv("HEXDUMP_CHAR2");
	chr2 = env ? *(char *)env : HEXDUMP_CHAR2;

	buf1 = malloc(col * (2 + 1));
	buf2 = malloc(col * (1 + 1));
	memset(buf1, 0, col * 3);
	memset(buf2, 0, col * 2);
	p = buf1;
	q = buf2;

	for (i = 0; i < size; i++) {
		sprintf(p, "%02X", *(unsigned char *)(ptr + i));
		sprintf(q, "%c", printchar(*(unsigned char *)(ptr + i)));
		p = p + 2; q++;

		if (((i + 1) % col)) {
			sprintf(p, "%c", printchar(chr1));
			sprintf(q, "%c", printchar(chr2));
			p++; q++;
		}

		if (!((i + 1) % col)) {
			if (ascii)
				note("%s | %s", buf1, buf2);
			else
				note("%s", buf1);

			memset(buf1, 0, col * 3);
			memset(buf2, 0, col * 2);
			p = buf1; q = buf2;
		}
	}

	if (size % col) {
		for (i = 0; i < (col - (size % col)); i++) {
			sprintf(p, "  "); p = p + 2;
			sprintf(q, "%c", printchar(' ')); q++;
			if ((i != (col - (size % col) - 1))) {
				sprintf(p, "%c", chr1);
				sprintf(q, "%c", chr2);
				p++; q++;
			}
		}

		if (ascii)
			note("%s | %s", buf1, buf2);
		else
			note("%s", buf1);
	}

	free(buf1);
	free(buf2);
}