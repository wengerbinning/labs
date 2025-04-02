#include <stdio.h>
#include <syslog.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "hexdump.h"


#define BUFIZE 1024

int main (int agrc, char *argv[]) {
	int sock, len;
	struct sockaddr_in addr;
	char buffer[BUFIZE + 1];

	/* */
	openlog("req", LOG_PID | LOG_PERROR, LOG_USER);

	/* socket */
	if ((sock = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_TCP)) < 0) {
		syslog(LOG_ERR, "failed to create socket!");
		return -1;
	}

	/* address */
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(443);

	/* connect */
	syslog(LOG_DEBUG, "start connect server ...");
	if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		syslog(LOG_ERR, "failed to connect server!");
		return -1;
	}


	/* */
	syslog(LOG_DEBUG, "start write data ...");
	write(sock, "1234", 2);




	/* */
	syslog(LOG_DEBUG, "start read data ...");
	len = read(sock, buffer, BUFIZE);
	hexdump(buffer, strlen(buffer));

	/* finish */
	syslog(LOG_DEBUG, "finnish connect.");
	close(sock);

	return 0;
}