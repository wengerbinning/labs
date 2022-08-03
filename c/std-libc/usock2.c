#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>

#define error(fmt, ...)     printf(fmt"\n", ## __VA_ARGS__)
#define warning(fmt, ...)   printf(fmt"\n", ## __VA_ARGS__)
#define notice(fmt, ...)    printf(fmt"\n", ## __VA_ARGS__)
#define note(fmt, ...)      printf(fmt"\n", ## __VA_ARGS__)
#define debug(fmt, ...)     printf(fmt"\n", ## __VA_ARGS__)

#define BUFIZE 512
#define SRV_SOCK "server.sock"

int main (int argc, char *argv[]) {
	int sock, cli_sock;
	int cli_len;
	struct sockaddr_un srv_addr, cli_addr;
	char rbuffer[BUFIZE + 1];
	char wbuffer[BUFIZE + 1];


	srv_addr.sun_family = AF_UNIX;
	strcpy(srv_addr.sun_path, SRV_SOCK);

	if ((sock = socket(AF_UNIX, SOCK_DGRAM, 0)) < 0) {
		error("failed to create socket");
		return -1;
    }

	if ( bind(sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0) {
		error("failed to bind socket");
		return -1;
	}

	listen(sock, 5);

	while (cli_sock = accept(sock, (struct sockaddr*)&cli_addr, &cli_len)) {
		memset(rbuffer, 0, sizeof(rbuffer));
		read(cli_sock, rbuffer, BUFIZE);

		if ((strlen(rbuffer) == BUFIZE) && rbuffer[BUFIZE] != '\n')
			rbuffer[BUFIZE] = '\n';
		else if (rbuffer[strlen(rbuffer)] != '\n')
			rbuffer[strlen(rbuffer) + 1] = '\n';

		printf("recev: %s", rbuffer);
		
		memset(wbuffer, 0, sizeof(wbuffer));
		sprintf(wbuffer, "server say hello\n");
		write(cli_sock, wbuffer, BUFIZE);
	}



	close(sock);
	unlink(SRV_SOCK);

	return 0;
}