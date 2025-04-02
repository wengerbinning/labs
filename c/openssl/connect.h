#ifndef __CONNECT_H__
#define __CONNECT_H__


int tls_init (struct connect *conn);
int tls_exit (struct connect *conn);

int tls_connect (struct connect *conn, char *host, unsigned int port);
int tls_write (struct connect *conn, void *buf, size_t len);
int tls_read (struct connect *conn, void *buf, size_t len);
int tls_close(struct connect *conn);

#endif /* __CONNECT_H__ */