
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





int main(int argc, char * argv[]) {
    SSL_CTX *ctx;
    BIO *bio;
    int ret, len;
	int port;
	char *host;

	struct timeval tv;
	char buffer[BUFIZE] = {0};
	char server[BUFIZE];

	openlog("req", LOG_PID | LOG_PERROR, LOG_USER);
	syslog(LOG_DEBUG, "task [%d]: init SSL context ...", getpid());

    // Initialize OpenSSL
    SSL_library_init();
    // SSL_load_error_strings();
    // OpenSSL_add_all_algorithms();

    // Create SSL context with TLS client method
    ctx = SSL_CTX_new(TLS_client_method());
    if (ctx == NULL) {
        ERR_print_errors_fp(stderr);
        return 1;
    }

	SSL_CTX_set_timeout(ctx, 1);

    // Create an SSL BIO connection
    bio = BIO_new_ssl_connect(ctx);
    if (bio == NULL) {
        ERR_print_errors_fp(stderr);
        return 1;
    }



	host = argv[1] ? argv[1] : "127.0.0.1";
	port = argv[2] ? atoi(argv[2]) : 443;
    // Set the target server and port for the connection
	snprintf(server, BUFIZE, "%s:%d", host, port);
	syslog(LOG_DEBUG, "start connect %s/%ld BIO: %d...", server, SSL_CTX_get_timeout(ctx), BIO_get_buffer_num_lines(bio));
    BIO_set_conn_hostname(bio, server); // Using a secure server

    // Enable non-blocking mode for the BIO (socket stream)
    BIO_set_nbio(bio, 1);

    // Attempt to connect using the BIO
    ret = BIO_do_connect(bio);
    if (ret <= 0) {
        if (BIO_should_retry(bio)) {
            fprintf(stderr, "Connection blocked, retrying...\n");
        } else {
            ERR_print_errors_fp(stderr);
            BIO_free_all(bio);
            SSL_CTX_free(ctx);
            return 1;
        }
    }

    // Perform SSL handshake (essential for secure connection)
    ret = BIO_do_handshake(bio);
    if (ret <= 0) {
        if (BIO_should_retry(bio)) {
            fprintf(stderr, "Handshake blocked, retrying...\n");
        } else {
            ERR_print_errors_fp(stderr);
            BIO_free_all(bio);
            SSL_CTX_free(ctx);
            return 1;
        }
    }

    // Successfully connected and handshaked
    printf("SSL/TLS connection established!\n");


	sleep(1);

	syslog(LOG_DEBUG, "send request ...");
	snprintf(buffer, BUFIZE, HTML_POST_REQ_HEAD_FMT, "/api/cgi", host, port, "text/plain", 1024 * 1024 );
	hexdump(buffer, strlen(buffer));
	if ((ret = BIO_write(bio, buffer, strlen(buffer))) <= 0) {
		syslog(LOG_DEBUG, "wirte head error: %d", ret);
	}

	int ofst = 0;
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

		int loop=3;
		while (loop-- && ((ret = BIO_write(bio, buffer, strlen(buffer))) <= 0)) {
			ERR_print_errors_fp(stderr);
			syslog(LOG_DEBUG, "wirte data error %d: %d", ret, loop);

			if (loop <= 0) {
				return -1;
			}
			sleep(1);

			// usleep(1000);
		}
	}

	ofst =+ ret;

	gettimeofday(&tv, NULL);
	syslog(LOG_DEBUG, "close connection: %d/%ld ...", ofst, tv.tv_sec);

	int i = 0;
	while ((len = BIO_read(bio, buffer, BUFIZE)) < 0) {
		if (BIO_should_retry(bio)) {
			syslog(LOG_ERR, "read error, retrying %3d ...\n", ++i);
			sleep(1);
        } else {
            ERR_print_errors_fp(stderr);
            BIO_free_all(bio);
            SSL_CTX_free(ctx);
            return 1;
		}
	}
	gettimeofday(&tv, NULL);
	syslog(LOG_DEBUG, "response %d: %ld ...", len, tv.tv_sec);
	hexdump(buffer, len);
	sleep(1);



    // Clean up
    BIO_free_all(bio);
    SSL_CTX_free(ctx);

    return 0;
}
