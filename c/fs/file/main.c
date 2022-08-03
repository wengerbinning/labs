#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/file.h>

#define LOCK "main.lock"
#define BUFIZE 64

int lock() {
	int lock, ret;
	char buffer[BUFIZE + 1];

	if ((lock = open(LOCK, O_CREAT | O_RDWR | O_TRUNC, 0600)) < 0) {
		return -1;
	}

	if ((ret = flock(lock, LOCK_EX | LOCK_NB))) {
		close(lock);
		return -1;
	}

	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "%lu\n", (long unsigned)getpid());
	write(lock, buffer, strlen(buffer));
	return 0;
}


int main (int argc, char *argv[]) {
	if( lock() < 0) {
		return -1;
	}

	sleep(10);

	return 0;
}