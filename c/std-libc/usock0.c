#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <signal.h>
#include <pthread.h>

#define error(fmt, ...)     printf(fmt"\n", ## __VA_ARGS__)
#define warning(fmt, ...)   printf(fmt"\n", ## __VA_ARGS__)
#define notice(fmt, ...)    printf(fmt"\n", ## __VA_ARGS__)
#define note(fmt, ...)      printf(fmt"\n", ## __VA_ARGS__)
#define debug(fmt, ...)     printf(fmt"\n", ## __VA_ARGS__)

#define BUFIZE 512
#define SRV_SOCK "server.sock"

static pthread_t thread;
void *thread_receive_worker (void *arg);

struct client_t {
	int sock;
	unsigned int flags;
} client_t;

void usock_handler (int sig) {
	exit(0);
}

int main (int argc, char *argv[]) {
	int sock;
	struct sockaddr_un srv_addr;

	srv_addr.sun_family = AF_UNIX;
	strcpy(srv_addr.sun_path, SRV_SOCK);


	if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
		error("failed to create socket");
		return -1;
	}

	signal(SIGINT, usock_handler);


	if ( bind(sock, (struct sockaddr *)&srv_addr, sizeof(srv_addr)) < 0) {
		error("failed to bind socket");
		return -1;
	}

	listen(sock, 5);

	// while (cli_sock = accept(sock, (struct sockaddr*)&cli_addr, &cli_len)) {
	// 	memset(rbuffer, 0, sizeof(rbuffer));
	// 	read(cli_sock, rbuffer, BUFIZE);

	// 	if ((strlen(rbuffer) == BUFIZE) && rbuffer[BUFIZE] != '\n')
	// 		rbuffer[BUFIZE] = '\n';
	// 	else if (rbuffer[strlen(rbuffer)] != '\n')
	// 		rbuffer[strlen(rbuffer) + 1] = '\n';

	// 	printf("recev: %s", rbuffer);

	// 	memset(wbuffer, 0, sizeof(wbuffer));
	// 	sprintf(wbuffer, "server say hello\n");
	// 	write(cli_sock, wbuffer, BUFIZE);
	// }

	while (1) {
		long cli_sock;
		int cli_len;
		struct sockaddr_un cli_addr;

		note("Wait for client connect ...");
		if ((cli_sock = accept(sock, (struct sockaddr*)&cli_addr, &cli_len)) < 0)
			continue;

		notice("accept client %d ...", cli_sock);

		pthread_create(&thread, NULL, thread_receive_worker, (void *)cli_sock);
	}

	// pthread_join(thread, NULL);
	note("running in here .... A");
	close(sock);
	unlink(SRV_SOCK);

	return 0;
}


void *thread_receive_worker (void *arg) {
	long sock = (long)arg;
	int ret, len;
	char rbuffer[BUFIZE + 1];
	char wbuffer[BUFIZE + 1];

	note("worker %d from %d start handling connect(%d)...", getpid(), getppid(), sock);

	while (1) {
		note("wait for read ...");
		sleep(1);

		memset(rbuffer, 0, sizeof(rbuffer));
		if ((ret = recv(sock, rbuffer, BUFIZE, 0)) <= 0) {
			if( ret == 0)
				note("close connect %d", sock);
			else
				error("read error");

			close(sock);
			return NULL;
		}

		if (len > 0) {
			if ((strlen(rbuffer) == BUFIZE) && rbuffer[BUFIZE] != '\n')
				rbuffer[BUFIZE] = '\n';
			else if (rbuffer[strlen(rbuffer)] != '\n')
				rbuffer[strlen(rbuffer) + 1] = '\n';

			printf("recev: %s", rbuffer);
		}

		sleep(3);

		//
		memset(wbuffer, 0, sizeof(wbuffer));
		sprintf(wbuffer, "server say hello\n");
		send(sock, wbuffer, BUFIZE, 0);
	}

	close(sock);

	note("worker %d finish handling connect(%d)...", getpid(), sock);
}