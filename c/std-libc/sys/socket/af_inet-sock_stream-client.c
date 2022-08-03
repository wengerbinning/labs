#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "develop.h"

int main(int argc, char *argv[]) {
	int sock, csock;
	struct sockaddr_in saddr;
	char buffer[1024];


	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		error("Failedto create socket!");
		exit(sock);
	}
	
	saddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &saddr.sin_addr);
	saddr.sin_port = htons(9000);

	if ((csock = connect(sock, (struct sockaddr *)&saddr, sizeof(saddr)))) {
		error("Failed to connect server!");
		close(sock);
		exit(-1);
	}

	memset(buffer, 0, sizeof(buffer));
	sprintf(buffer, "Hello, world!");
	
	send(sock, buffer, strlen(buffer), 0);
	
	read(sock, buffer, sizeof(buffer));

	close(csock);
	close(sock);

	return 0;
}
