
#include <time.h>
#include <stdio.h>
#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "hexdump.h"

#define BUFIZE 1024 + 1

#define HTML_POST_REQ_HEAD_FMT \
	"POST %s HTTP/1.1\r\n"              \
	"User-Agent: FIOFCGIO\r\n"          \
	"Host: %s:%u\r\n"                   \
	"Content-Type: %s\r\n"              \
	"Content-Length: %d\r\n"            \
	"Cache-control: no-cache\r\n"       \
	"\r\n"

#define PATHIZE 1024
typedef struct connect {
	void *ctx, *priv;
	size_t connect_timeout, write_timeout, read_timeout;
} connect_t;


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

void tls_exit (struct connect *conn) {
	SSL_CTX *ctx;

	if (!conn && !conn->ctx)
		return;

	ctx = (SSL_CTX *)conn->ctx;
	if (ctx)
		SSL_CTX_free(ctx);
}

int tls_connect (struct connect *conn, char *host, unsigned int port) {
	BIO *bio;
	int ret, loop;
	char path[PATHIZE + 1];

	if (!conn && !conn->priv)
		return -1;

	bio = (BIO *)conn->priv;
	snprintf(path, PATHIZE, "%s:%u", host, port);
	BIO_set_conn_hostname(bio, path);
	BIO_set_nbio(bio, 1);

	/* connect with server */

	loop = conn->connect_timeout ? conn->connect_timeout : 3;
	while (loop-- && (ret = BIO_do_connect(bio)) <= 0) {
		if (0 < loop && BIO_should_retry(bio)) {
			sleep(1);
		} else {
			return -2;
		}
	}

	/* handshake with server */
	loop = conn->connect_timeout ? conn->connect_timeout : 3;
	while (loop-- && (ret = BIO_do_handshake(bio)) <= 0) {
		if (0 < loop && BIO_should_retry(bio)) {
			sleep(1);
		} else {
			return -3;
		}
	}

	return 0;
}

int tls_write (struct connect *conn, void *buf, size_t len) {
	BIO *bio;
	int ret, loop;

	if (!conn && !conn->priv)
		return -1;

	bio = (BIO *)conn->priv;
	loop = conn->read_timeout ? conn->write_timeout : 3;
	while (loop-- && ((ret = BIO_write(bio, buf, len)) <= 0)) {
		if (0 < loop && BIO_should_retry(bio)) {
			sleep(1);
		} else {
			return 0;
		}
	}

	return len;
}

int tls_read (struct connect *conn, void *buf, size_t len) {
	BIO *bio;
	int ret, loop;

	if (!conn && !conn->priv)
		return -1;

	bio = (BIO *)conn->priv;
	loop = conn->read_timeout ? conn->read_timeout : 3;
	while (loop-- && ((ret = BIO_read(bio, buf, len)) <= 0)) {
		if (0 < loop && BIO_should_retry(bio)) {
			sleep(1);
		} else {
			return 0;
		}
	}

	return len;
}

void tls_close(struct connect *conn) {
	BIO *bio = (BIO *)conn->priv;

	if (!conn && !conn->priv)
		return;

	bio = (BIO *)conn->priv;
	if (bio)
		BIO_free_all(bio);
}




















int main(int argc, char * argv[]) {
	connect_t connection, *conn = &connection;
	char *host;
    int ret, len;
	int port;
	char buffer[BUFIZE + 1] = {0};
	struct timeval tv;

	openlog("req", LOG_PID | LOG_PERROR, LOG_USER);
	syslog(LOG_DEBUG, "task [%d]: init SSL context ...", getpid());

	/* init connection conext */
	if ((ret = tls_init(conn)) < 0) {
		syslog(LOG_ERR, "failed to TLS init!");
		return -1;
	}

	conn->connect_timeout = 3;
	conn->write_timeout = 3;
	conn->read_timeout = 10;

	/* connect server */
	host = argv[1] ? argv[1] : "127.0.0.1";
	port = argv[2] ? atoi(argv[2]) : 443;
	if ((ret = tls_connect(conn, host, port)) < 0) {
		syslog(LOG_ERR, "failed to connect %s:%d!", host, port);
		return -1;
	}
	syslog(LOG_DEBUG, "TLS connect finsihed");
	sleep(1);

	/* Start translate data
	 * dededewdewdewdwedewdewqdqwedwqefdwedff
	 * dfewfewfefweffwefwfwefwefew
	 */

	syslog(LOG_DEBUG, "send request ...");
	snprintf(buffer, BUFIZE, HTML_POST_REQ_HEAD_FMT,
		"/api/cgi", host, port, "text/plain", 1024 * 1024 );
	hexdump(buffer, strlen(buffer));
	if ((ret = tls_write(conn, buffer, strlen(buffer))) < 0) {
		syslog(LOG_ERR, "wirte head error: %d", ret);
		return -1;
	}

	for (int i = 0; i < 1024; i++) {
		syslog(LOG_DEBUG, "wirte time %2d ...", i);
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
		if ((ret = tls_write(conn, buffer, strlen(buffer))) <= 0) {
			syslog(LOG_ERR, "wirte data error: %d", ret);
			return -1;
		}

		len =+ ret;
	}

	/* Start receive data
	 * dededewdewdewdwedewdewqdqwedwqefdwedff
	 * dfewfewfefweffwefwfwefwefew
	 */

	gettimeofday(&tv, NULL);
	syslog(LOG_DEBUG, "close connection: %d/%ld ...", len, tv.tv_sec);

	while (0 < (ret = tls_read(conn, buffer, 10))) {
		syslog(LOG_DEBUG, "read %d: ...", ret);
		hexdump(buffer, ret);
	}

	sleep(1);

	// Clean up
	tls_close(conn);
	tls_exit(conn);

	return 0;
}
