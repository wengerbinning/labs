
#include <time.h>
#include <stdio.h>
#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "hexdump.h"

typedef struct connect {
	int sock;
	void *ctx, *priv;
	size_t conn_timeout, send_timeout, recv_timeout;
} connect_t;

/* TCP APIs
 * Those apis for TCP connection
 */

int tcp_init (struct connect *conn) {
	int sock;

	if (!conn)
		return -1;

	if ((sock = socket(PF_INET, SOCK_STREAM | SOCK_NONBLOCK, IPPROTO_IP)) < 0) {
		return -2;
	}

	conn->sock = sock;
	return 0;
}

int tcp_exit (struct connect *conn) {
	int sock;

	if (!conn)
		return -1;

	if (0 < conn->sock)
		close(conn->sock);

	return 0;
}

int tcp_connect (struct connect *conn, char *addr, int port) {
	int ret, sock, loop;
	struct sockaddr_in saddr;

	if (!conn || conn->sock < 0)
		return -1;


	sock = conn->sock;
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = inet_addr(addr);
	saddr.sin_port = htons(port);

	loop = conn->conn_timeout ? conn->conn_timeout : 3;

	while( loop-- &&
		(ret = connect(sock, (struct sockaddr *)&saddr, sizeof(saddr))) < 0)
	{
		if (loop <= 0) {
			return -1;
		}
		sleep(1);
	}

	return 0;
}

int tcp_send (struct connect *conn, void *buf, size_t len) {
	int ret, sock, loop;

	if (!conn || conn->sock < 0)
		return -1;

	sock = conn->sock;
	loop = conn->send_timeout ? conn->send_timeout : 3;
	while (loop-- && ((ret = write(sock, buf, len)) <= 0)) {
		if (loop <= 0) {
			return -1;
		}
		sleep(1);
	}

	return len;
}

int tcp_recv (struct connect *conn, void *buf, size_t len) {
	int ret, sock, loop;

	if (!conn || conn->sock < 0)
		return -1;

	sock = conn->sock;
	loop = conn->recv_timeout ? conn->recv_timeout : 3;
	while (loop-- && ((ret = read(sock, buf, len)) <= 0)) {
		if (ret == 0)
			return 0;
		if (loop <= 0)
			return -1;
		sleep(1);
	}
	return len;
}

int tcp_close (struct connect *conn) {

	if (!conn || conn->sock < 0)
		return -1;

	close(conn->sock);

	return 0;
}


#define BUFIZE 1024 + 1

#define HTML_POST_REQ_HEAD_FMT \
	"POST %s HTTP/1.1\r\n"              \
	"User-Agent: FIOFCGIO\r\n"          \
	"Host: %s:%u\r\n"                   \
	"Content-Type: %s\r\n"              \
	"Content-Length: %d\r\n"            \
	"Cache-control: no-cache\r\n"       \
	"\r\n"

int main (int argc, char *argv[]) {
	connect_t connection, *conn = &connection;
	char *host;
    int ret, len;
	int port;
	char buffer[BUFIZE + 1] = {0};
	struct timeval tv;

	openlog("req", LOG_PID | LOG_PERROR, LOG_USER);
	syslog(LOG_DEBUG, "task [%d]: init TCP context ...", getpid());

	/* init connection conext */
	if ((ret = tcp_init(conn)) < 0) {
		syslog(LOG_ERR, "failed to TCP init!");
		return -1;
	}

	conn->conn_timeout = 3;
	conn->send_timeout = 3;
	conn->recv_timeout = 30;

	/* connect server */
	host = argv[1] ? argv[1] : "127.0.0.1";
	port = argv[2] ? atoi(argv[2]) : 80;
	if ((ret = tcp_connect(conn, host, port)) < 0) {
		syslog(LOG_ERR, "failed to connect %s:%d!", host, port);
		return -1;
	}
	syslog(LOG_DEBUG, "TCP connect finsihed");
	sleep(1);


	/* Start translate data
	 * dededewdewdewdwedewdewqdqwedwqefdwedff
	 * dfewfewfefweffwefwfwefwefew
	 */

	syslog(LOG_DEBUG, "send request ...");
	snprintf(buffer, BUFIZE, HTML_POST_REQ_HEAD_FMT,
		"/api/cgi", host, port, "text/plain", 1024 * 1024 );
	hexdump(buffer, strlen(buffer));
	if ((ret = tcp_send(conn, buffer, strlen(buffer))) < 0) {
		syslog(LOG_ERR, "wirte head error: %d", ret);
		return -1;
	}
	len = 0;
	for (int i = 0; i < 1024; i++) {
		syslog(LOG_DEBUG, "wirte time %4d/%d:%d ...", i, len, ret);

		snprintf(buffer, BUFIZE,
			"%04X123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF"
			"%04X123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF"
			"%04X123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF"
			"%04X123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF"
			"%04X123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF"
			"%04X123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF"
			"%04X123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF"
			"%04X123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF"
			"%04X123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF"
			"%04X123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF"
			"%04X123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF"
			"%04X123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF"
			"%04X123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF"
			"%04X123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF"
			"%04X123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF"
			"%04X123456789ABCDEF123456789ABCDEF123456789ABCDEF123456789ABCDEF"
			, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i, i );
		if ((ret = tcp_send(conn, buffer, strlen(buffer))) <= 0) {
			syslog(LOG_ERR, "wirte data error: %d", ret);
			return -1;
		}

		len += ret;
	}

	/* Start receive data
	 * dededewdewdewdwedewdewqdqwedwqefdwedff
	 * dfewfewfefweffwefwfwefwefew
	 */

	syslog(LOG_DEBUG, "close connection: %d ...", len);

	while (0 < (ret = tcp_recv(conn, buffer, 10))) {
		syslog(LOG_DEBUG, "read %d: ...", ret);
		hexdump(buffer, ret);
	}

	if (ret < 0) {
		syslog(LOG_DEBUG, "read error %d: ...", ret);
	}
	sleep(1);

	// Clean up
	tcp_close(conn);
	tcp_exit(conn);

	return 0;
}