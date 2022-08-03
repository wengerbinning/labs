#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>



int main(int argc, char * argv[]) {
	int fd, ret, exit=0;
	struct sockaddr_in addr;
	struct sockaddr_in cliaddr;
	int recvlen, clilen;
	char recvbuf[1024], sendbuf[1024]; 

	fd = socket(PF_INET, SOCK_DGRAM, 0);
	if (fd == -1) {
		return -1;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(6000);
	addr.sin_addr.s_addr = INADDR_ANY;

	ret = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
	if (ret == -1) {
		return -1;
	}

	while (!exit) {
		recvlen = recvfrom(fd, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *)&cliaddr, &clilen);
		

		printf("recvive client message: %s\n", recvbuf);

		sendto(fd, sendbuf, strlen(sendbuf)+1,0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
	}

	close(fd);

	return 0;
}

