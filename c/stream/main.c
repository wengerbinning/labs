#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

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
	int fd = 1;
	bool create = false;
	size_t i, wid, max, size;
	char buffer[1024 + 1];
	char fmt[256] = {0};

	if (argv[1]) {
		sscanf(argv[1], "%llu", &size);
	}

	/* */
	if (argv[2] && 0 != strcmp(argv[2], "-")) {
		if ((fd = open( argv[2], O_CREAT | O_TRUNC | O_RDWR, 0664 )) < 0) {
			return -1;
		}
		create = true;
	}

	if (size <=0) {
		if (create)
			close(fd);
		return 0;
	}

	/* */
	wid = 1;
	max = size - 1;
	while ((max >>= 8)) wid++;
	sprintf(fmt, "%%0%dX%%c", (wid * 2));
	if (create) {
		sprintf(buffer, "STREAM;WIDTH=%llu,SIZE=%llu\r\n", wid, size);
		write(fd, buffer, strlen(buffer));
	}
	for (i = 0; i< size; i++) {
		char chr;
		chr = (i && !((i + 1) % 16)) ? '\n': ' ';
		sprintf(buffer, fmt, i, chr);
		write(fd, buffer, strlen(buffer));
	}
	if (i && (i % 16))
		write(fd, "\n", 1);

	if (create)
		close(fd);
	return 0;
}