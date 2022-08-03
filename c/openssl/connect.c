#include <time.h>
#include <stdio.h>
#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <arpa/inet.h>
#include <sys/spcket.h>

#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <openssl/err.h>

#define PATHIZE 1024

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

/* TLS APIs
 * Those apis for TLS connection
 */
int tls_init (struct connect *conn) {
	SSL_CTX *ctx;
	BIO *bio;

	if (!conn) {
		return -1;
	}

	/* init OpenSSL conext */
	SSL_library_init();
	// SSL_load_error_strings();
	// OpenSSL_add_all_algorithms();

	if (!(ctx = SSL_CTX_new(TLS_client_method()))) {
		return -1;
	}
	// SSL_CTX_set_timeout(ctx, 30);

	/* create BIO buffer */
	if (!(bio = BIO_new_ssl_connect(ctx))) {
		SSL_CTX_free(ctx);
		return -1;
	}

	conn->ctx = ctx;
	conn->priv = bio;
	return 0;
}

int tls_exit (struct connect *conn) {
	SSL_CTX *ctx;

	if (!conn || !conn->ctx)
		return -1;

	ctx = (SSL_CTX *)conn->ctx;
	if (ctx)
		SSL_CTX_free(ctx);
}

int tls_connect (struct connect *conn, char *addr, unsigned int port) {
	BIO *bio;
	int ret, loop;
	char path[PATHIZE + 1];

	if (!conn || !conn->priv)
		return -1;

	bio = (BIO *)conn->priv;
	snprintf(path, PATHIZE, "%s:%u", addr, port);
	BIO_set_conn_hostname(bio, path);
	BIO_set_nbio(bio, 1);

	/* connect with server */

	loop = conn->conn_timeout ? conn->conn_timeout : 3;
	while (loop-- && (ret = BIO_do_connect(bio)) <= 0) {
		if (0 < loop && BIO_should_retry(bio)) {
			sleep(1);
		} else {
			return -2;
		}
	}

	/* handshake with server */
	loop = conn->conn_timeout ? conn->conn_timeout : 3;
	while (loop-- && (ret = BIO_do_handshake(bio)) <= 0) {
		if (0 < loop && BIO_should_retry(bio)) {
			sleep(1);
		} else {
			return -3;
		}
	}

	return 0;
}

int tls_send (struct connect *conn, void *buf, size_t len) {
	BIO *bio;
	int ret, loop;

	if (!conn || !conn->priv)
		return -1;

	bio = (BIO *)conn->priv;
	loop = conn->send_timeout ? conn->send_timeout : 3;
	while (loop-- && ((ret = BIO_write(bio, buf, len)) <= 0)) {
		if (0 < loop && BIO_should_retry(bio)) {
			sleep(1);
		} else {
			return 0;
		}
	}

	return len;
}

int tls_recv (struct connect *conn, void *buf, size_t len) {
	BIO *bio;
	int ret, loop;

	if (!conn || !conn->priv)
		return -1;

	bio = (BIO *)conn->priv;
	loop = conn->recv_timeout ? conn->recv_timeout : 3;
	while (loop-- && ((ret = BIO_read(bio, buf, len)) <= 0)) {
		if (ret == 0)
			return 0;
		if (loop <= 0 && !BIO_should_retry(bio))
			return -1;
		sleep(1);
	}

	return len;
}

int tls_close (struct connect *conn) {
	BIO *bio = (BIO *)conn->priv;

	if (!conn || !conn->priv)
		return -1;

	bio = (BIO *)conn->priv;
	if (bio)
		BIO_free_all(bio);
}
