#include <stdio.h>
#include <syslog.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>

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




#define HTML_POST_REQ_HEAD_FMT \
    "POST %s HTTP/1.1\r\n"        \
    "User-Agent: Test\r\n"    \
    "Host: %s:%u\r\n"             \
    "Content-Type: %s\r\n"        \
    "Content-Length: %d\r\n"      \
    "\r\n"





int main(int argc, char * argv[]) {
	int sock, ret;
	struct sockaddr_in addr;
    char buffer[BUFIZE + 1];

    openlog("request", LOG_PID | LOG_PERROR, LOG_USER);
	sock = socket(PF_INET, SOCK_STREAM, IPPROTO_IP);

	addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(80);

    syslog(LOG_DEBUG, "connect %s:%d ...", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	connect(sock, (struct sockaddr *)&addr, sizeof(addr));


    sprintf(buffer, HTML_POST_REQ_HEAD_FMT, "/api/cgi1",
        inet_ntoa(addr.sin_addr), ntohs(addr.sin_port),
        "multipart/form-data;boundary=4324", 10);

    hexdump(buffer, strlen(buffer));
    send(sock, buffer, strlen(buffer), 0);

    sleep(2);
    sprintf(buffer, "1234567890ABCDEFGH");
    hexdump(buffer, strlen(buffer));
    send(sock, buffer, strlen(buffer), 0);


    read(sock, buffer, BUFIZE);

    close(sock);
	return 0;
}
