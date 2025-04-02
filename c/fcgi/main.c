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
		char *caddr, *cport, *method;

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
		int i = 0;
		while (0 < (len = FCGX_GetStr(buffer, BUFIZE, req->in))) {
			syslog(LOG_DEBUG, "read %4d -> %d ...", ++i, len);
			// hexdump(buffer, len);
		}

		if (len == 0 ) {
			syslog(LOG_DEBUG, "read finish ...");
		}




		sleep(5);

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