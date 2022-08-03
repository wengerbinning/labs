#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define BUFIZE 1024

int main(int argc, char * argv[]) {
	int sock, ret;
	struct sockaddr_in addr;
    char buffer[BUFIZE + 1];

	int recvlen, clilen;
	char recvbuf[1024], sendbuf[1024];


	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);

	addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_LOOPBACK;
	addr.sin_port = 80;

	connect(sock, (struct sockaddr *)&addr, sizeof(addr));


    send(sock, buffer, strlen(buffer), 0);

    read(sock, buffer, BUFIZE);

    close(sock);
	return 0;
}
