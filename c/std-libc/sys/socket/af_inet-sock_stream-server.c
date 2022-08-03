#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "develop.h"

int main(int argc, char *argv[]) {
	int fd, clifd;
	struct sockaddr_in address;
	int opt, addrlen;
	char buffer[1024];
	int ret;
	
	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		error("Failed to create socket!(error: %d)", fd);
		exit(fd);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	address.sin_port = htons(9000);

	if ((ret = bind(fd, (struct sockaddr *)&address, sizeof(address)))) {
		error("Failed to bind address! (%d)", ret);
		exit(0);
	}

	if (listen(fd, 3) < 0) {
		error("Failed to listen address!");
		exit(0);
	}

	while (1) {

		if ((clifd = accept(fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
			error("Failed to accept!");
			close(fd);
			exit(-1);
		}

		read(clifd, buffer, sizeof(buffer));

		info("receive from client[](%d bytes): %s",strlen(buffer), buffer);

		close(clifd);
	}

	close(fd);

	return 0;
}
