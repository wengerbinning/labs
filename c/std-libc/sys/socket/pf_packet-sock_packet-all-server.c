#include <linux/if_ether.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include "develop.h"
#include "tools.h"

#include "ethertype.h"

int main (int argc, char argv[]) {
	int sock, clisock;
	struct sockaddr addr;
	struct sockaddr_in cliaddr;
	int addrlen;
	int ret;
	char buffer[1024];

	


	if ((sock = socket(PF_PACKET, SOCK_PACKET, htons(ETH_P_ALL))) < 0) {
		error("Failed to create socket!");
		exit(sock);
	}

	strcpy(addr.sa_data, "eth0");

	if (bind(sock, (struct sockaddr *)&addr, sizeof(addr))) {
		error("Failed to bind address!");
		exit(0);
	}

	debug("Running in Server ...");

	while (1) {
		if ((ret = recv(sock, buffer, sizeof(buffer), 0)) < 0) {
			error("Failed to Receive!");
			close(sock);
			exit(-1);
		}

		debug("receive %d data", ret);
	}

	
	return 0;
}
