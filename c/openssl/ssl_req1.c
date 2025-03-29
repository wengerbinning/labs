
#include <time.h>
#include <stdio.h>
#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#include <arpa/inet.h>


#include "hexdump.h"

// https://manpages.debian.org/stretch/libssl-doc/BIO_do_handshake.3ssl.en.html

// #include <openssl/applink.h>

#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/tls1.h>
#include <openssl/x509v3.h>

#include "hexdump.h"
#define BUFIZE 512

/*
POST /api/cgi1 HTTP/1.1

// Accept: * /*
Host: 10.10.0.10:80
Accept-Encoding: gzip, deflate, br
Connection: keep-alive
Content-Type: multipart/form-data; boundary=--------------------------924685863710734922037384
Content-Length: 759

----------------------------924685863710734922037384
Content-Disposition: form-data; name="id"


----------------------------924685863710734922037384
Content-Disposition: form-data; name="action"


----------------------------924685863710734922037384
Content-Disposition: form-data; name="devMac"


----------------------------924685863710734922037384
Content-Disposition: form-data; name="file"; filename="Peer_client0 (1).conf"
Content-Type: text/plain
*/


#define IPV4 "127.0.0.1"
#define PORT "443"
#define PATH "/api/cgi1"

#define HTML_POST_REQ_HEAD_FMT \
    "POST %s HTTP/1.1\r\n"              \
    "User-Agent: FIOFCGIO\r\n"          \
    "Host: %s:%u\r\n"                   \
    "Content-Type: %s\r\n"              \
    "Content-Length: %d\r\n"            \
	"Cache-control: no-cache\r\n"       \
    "\r\n"

/*
Cache-Control:
max-age=180, public
no-cache
no-store

 */


SSL_CTX * ssl_new (void) {
	SSL_library_init();
	SSL_load_error_strings();

	return SSL_CTX_new(TLS_client_method());
}

BIO * ssl_connect (SSL_CTX *ctx, char *hostname) {
	BIO *web;
	SSL *ssl;

	web = BIO_new_ssl_connect(ctx);

	BIO_get_ssl(web, &ssl);
	SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);

	BIO_set_conn_hostname(web, hostname);

	/* */
	if (BIO_do_connect(web) <= 0) {
		fprintf(stderr, "Error connecting to server\n");
		ERR_print_errors_fp(stderr);
		return NULL;
	}

	/* */
	if (BIO_do_handshake(web) <= 0) {
		fprintf(stderr, "Error connecting to server\n");
		ERR_print_errors_fp(stderr);
		return NULL;
	}

	return web;
}

void ssl_close (BIO *web) {
	if (web)
		BIO_free_all(web);
}

void ssl_free (SSL_CTX *ctx) {
	if (ctx)
    	SSL_CTX_free(ctx);
}

int main(int argc, char * argv[]) {
	SSL_CTX *ctx = NULL;
	BIO *web = NULL;
	char buffer[BUFIZE + 1] = {0};
	int len;

	char ipv4[] = "127.0.0.1";
	char *host, hostname[] = "127.0.0.1:443";

	host = hostname;
	if (argv[1])
		host = argv[1];

	/* */
	openlog("ssl_req", LOG_PID | LOG_PERROR, LOG_USER);
	syslog(LOG_DEBUG, "init SSL context ...");
	ctx = ssl_new();

	/* default timeout: 7200 sec */
	SSL_CTX_set_timeout(ctx, 60);
	syslog(LOG_DEBUG, "connect server %s: %ld ...", host, SSL_CTX_get_timeout(ctx));
	if (!(web = ssl_connect(ctx, host))) {
		return -1;
	}

	BIO_set_nbio(web, 1);

	/* head */
	syslog(LOG_DEBUG, "send request ...");
	snprintf(buffer, BUFIZE, HTML_POST_REQ_HEAD_FMT, "/api/cgi", "127.0.0.1", 443, "text/plain", 3 * 10);
	BIO_write(web, buffer, strlen(buffer));

	/* write body */
	for (int i =0; i < 10; i++) {
		syslog(LOG_DEBUG, "wirte time %d ...", i);
		snprintf(buffer, BUFIZE, "123");
		if ( BIO_write(web, buffer, strlen(buffer)) <= 0) {
			syslog(LOG_DEBUG, "wirte error");
		}
		sleep(1);
	}



	/* delay for nginx proxy */
	syslog(LOG_DEBUG, "close connection ...");
	// len = BIO_read(web, buffer, BUFIZE);
	// hexdump(buffer, len);
	sleep(1);
	/* */
	ssl_close(web);



	/* */
	ssl_free(ctx);
	closelog();
	return 0;
}