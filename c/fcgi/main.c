#include <stdio.h>
#include <syslog.h>
#include <libgen.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include <arpa/inet.h>

#include <fastcgi.h>
#include <fcgiapp.h>
#include <fcgimisc.h>

#include "hexdump.h"


#include <openssl/bio.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/tls1.h>
#include <openssl/x509v3.h>

#define FCGI_RES_HTML_FMT \
    "Content-Type: text/html\r\n"                                             \
    "\r\n"                                                                    \
    "<!DOCTYPE html>\n<html>\n"                                               \
    "<head>\n"                                                                \
    "<title>FastCGI Test API</title>\n"                                       \
    "</head>\n"                                                               \
    "<body>\n"                                                                \
    "<p>%s</p>\n"                                                             \
    "</body>\n</html>\n"

#define FCGI_RES_HTML_FMT_HEADER \
    "Content-Type: text/html\r\n"                                             \
    "\r\n"                                                                    \
    "<!DOCTYPE html>\n<html>\n"                                               \
    "<head>\n"                                                                \
    "<title>FastCGI Test API</title>\n"                                       \
    "</head>\n"                                                               \
    "<body>\n"                                                                \
    "<p>"
#define FCGI_RES_HTML_FMT_FOOTER \
    "</p>\n"                                                                  \
    "</body>\n</html>\n"

#define HTML_POST_REQ_HEAD_FMT \
    "POST %s HTTP/1.1\r\n"       \
    "User-Agent: FIOFCGIO\r\n"   \
    "Host: %s:%u\r\n"            \
    "Content-Type: %s\r\n"       \
    "Content-Length: %d\r\n"     \
    "\r\n"

static FCGX_Request fcgx_req;

#define PATHIZE 64
#define BUFIZE 1024




#define STRCMP(s1, s2) (s1 && s2 && 0 == strncmp(s1, s2, strlen(s2)))


#define GET     1
#define POST    2







int tcp_connect (char *ipaddr, int port) {
	int sock;
	struct sockaddr_in addr;
    char buffer[BUFIZE + 1];

    sock = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ipaddr);
    addr.sin_port = htons(port);

    // syslog(LOG_DEBUG, "%d connect %s:%d %s %d ...", sock,
    //     inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
    connect(sock, (struct sockaddr *)&addr, sizeof(addr));

    return sock;
}

size_t tcp_send (int sock, char *buf, size_t len) {
    send(sock, buf, len, 0);
    return len;
}

void tcp_close (int sock) {
    close(sock);
}


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

size_t ssl_send(BIO *web, char *buf, size_t len) {
    BIO_write(web, buf, len);
    return len;
}

void ssl_close (BIO *web) {
	if (web)
		BIO_free_all(web);
}

void ssl_free (SSL_CTX *ctx) {
	if (ctx)
    	SSL_CTX_free(ctx);
}

struct device {
    char *ipaddr;
    unsigned int port;
    bool ssl;

    /**/
    SSL_CTX *ctx;
    BIO *session;
    int sock;

};

int dev_connect (struct device *dev) {
    char path[PATHIZE + 1];

    syslog(LOG_DEBUG, "connect %s:%d ...", dev->ipaddr, dev->port);
    if (dev->ssl) {
        dev->ctx = ssl_new();
        sprintf(path, "%s:%u", dev->ipaddr, dev->port);
        dev->session = ssl_connect(dev->ctx, path);
    } else {
        dev->sock = tcp_connect(dev->ipaddr, dev->port);
    }

    return 0;
}

size_t dev_send (struct device *dev, char *buf, size_t len) {

    hexdump(buf, len);
    if (dev->ssl) {
        return ssl_send(dev->session, buf, len);
    }

    return tcp_send(dev->sock, buf, len);
}

void dev_close (struct device *dev) {
    if (dev->ssl) {
        ssl_close(dev->session);
        ssl_free(dev->ctx);
    } else {
        tcp_close(dev->sock);
    }
}




int server_main (char *name, int port, char *dst, int argc, char *argv[]) {
	char *buf, *buf1;

	int sock;
	FCGX_Request *req = &fcgx_req;
	const char *str;
	char path[PATHIZE + 1];
	char buffer[BUFIZE + 1];
	size_t length, len;
	int dev;

	/* */
	openlog((name ? name : "server"), LOG_PID | LOG_PERROR, LOG_USER);
	FCGX_Init();

	/* */
	sprintf(path, "127.0.0.1:%u", port);
	if ((sock = FCGX_OpenSocket(path, 20)) < 0) {
		syslog(LOG_ERR, "Failed to create sock\n");
		return -1;
	}
	FCGX_InitRequest(req, sock, 0);

	/* */
	while (1) {
		int rlen;
		int fiofcgi = 0;
		struct device device = {0}, *dev;
		char *caddr, *cport, *method;

		dev = &device;
		sprintf(path, "127.0.0.1:%u", port);
		syslog(LOG_DEBUG, "listen %s ...", path);
		FCGX_Accept_r(req);

		caddr = FCGX_GetParam("REMOTE_ADDR", req->envp);
		cport = FCGX_GetParam("REMOTE_PORT", req->envp);
		method = FCGX_GetParam("REQUEST_METHOD", req->envp);
		str = FCGX_GetParam("CONTENT_LENGTH", req->envp);
		/* */
		syslog(LOG_DEBUG, "accept %s:%s %s %s...", caddr, cport, method, str);


		/* read */
		len = FCGX_GetStr(buffer, BUFIZE, req->in);
		syslog(LOG_DEBUG, "read %d ...", len);
		hexdump(buffer, len);




		/* response */
		FCGX_FPrintF(req->out, FCGI_RES_HTML_FMT_HEADER);
		FCGX_FPrintF(req->out, "From %s:%s %s Request:\n", caddr, cport, method);
		FCGX_FPrintF(req->out, FCGI_RES_HTML_FMT_FOOTER);

		/* close */
		FCGX_Finish_r(req);
	}

	closelog();
	return 0;
}




int main (int argc, char *argv[]) {

    if (STRCMP(basename(argv[0]), "iostream0")) {
        return server_main("cgi1", 2007, "cgi2", argc, argv);
    } else if (STRCMP(basename(argv[0]), "iostream1")) {
        return server_main("cgi2", 2008, "cgi1", argc, argv);
    } else {
        return server_main("cgi", 2006, "cgi", argc, argv);
    }
}