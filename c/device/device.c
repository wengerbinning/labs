#include <stdio.h>
#include <string.h>

typedef union dev_mac {
    char addr[6];
    struct {
        char oui[3];
        char eui[3];
    };
} dev_mac_t;

#define PROC_DEV_MAC_FILE  "device.txt"



#define MACFMT0 "%02hhx%02hhx%02hhx%02hhx%02hhx%02hhx"      /* Example: c074ada1b2c3      */
#define MACFMT1 "%02hhx:%02hhx:%02hhx:%02hhx:%02hhx:%02hhx" /* Example: c0:74:ad:a1:b2:c3 */
#define MACFMT2 "%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX"      /* Example: C074ADA1B2C3      */
#define MACFMT3 "%02hhX:%02hhX:%02hhX:%02hhX:%02hhX:%02hhX" /* Example: C0:74:AD:A1:B2:C3 */
#define _STR2MAC(m) &(m)[0], &(m)[1], &(m)[2], &(m)[3], &(m)[4], &(m)[5]
#define _MAC2STR(m)  (m)[0],  (m)[1],  (m)[2],  (m)[3],  (m)[4],  (m)[5]
#define STR2MAC(mac) _STR2MAC((unsigned char *)(mac))
#define MAC2STR(mac) _MAC2STR((unsigned char *)(mac))

//=============================================================================
int utils_get_cur_dev_mac(dev_mac_t *mac)
//=============================================================================
{
    int len;
    FILE *fp;
    char buffer[16 + 1] = {0};

    if (!mac)
        return -1;

    memset(mac, 0, sizeof(dev_mac_t));
    if (!(fp = fopen(PROC_DEV_MAC_FILE, "r" ))) {
        return -1;
    }
    if ((len = fread(buffer, 1, 16, fp)) < 0) {
        fclose(fp);
        return -1;
    }
    fclose(fp);
    /**/
    sscanf(buffer, MACFMT1,  STR2MAC(mac->addr));
    return 0;
}



int main(int argc, char *argv[]) {
    int ret;
    dev_mac_t mac = {01};

     printf("MAC:" MACFMT3 "\n", MAC2STR(mac.addr));

    ret = utils_get_cur_dev_mac(&mac);


    printf("return: %d, MAC:" MACFMT3 "\n", ret, MAC2STR(mac.addr));

}



dev_connect() {

}

#define PATHIZE 1024
typedef struct connect {
	void *ctx, *priv;
} connect_t;


int tls_init (struct connect *conn) {
	SSL_CTX *ctx;
	BIO *bio;

	if (!conn) {
		return -1;
	}

	SSL_library_init();
	// SSL_load_error_strings();
	// OpenSSL_add_all_algorithms();

	if (!(ctx = SSL_CTX_new(TLS_client_method()))) {
		return -1;
	}

	// SSL_CTX_set_timeout(ctx, 30);

	if (!(bio = BIO_new_ssl_connect(ctx))) {
		SSL_CTX_free(ctx);
		return -1;
	}

	conn->ctx = ctx;
	conn->priv = bio;
	return 0;
}

void tls_exit (struct connect *conn) {
	SSL_CTX *ctx =conn->ctx;
	SSL_CTX_free(ctx);
}

int tls_connect (struct connect *conn, char *host, unsigned int port) {
	char path[PATHIZE + 1];
	BIO *bio = conn->priv;
	int ret, loop;

	snprintf(path, PATHIZE, "%s:%u", host, port);
	BIO_set_conn_hostname(bio, path);

	BIO_set_nbio(bio, 1);

	/* */
	loop = 3;
	while (loop-- && (ret = BIO_do_connect(bio)) <= 0) {
		if (0 < loop && BIO_should_retry(bio)) {
			continue;
		} else {
			return -1;
		}
	}

	/* */
	loop = 3;
	while (loop-- && (ret = BIO_do_handshake(bio)) <= 0) {
		if (0 < loop && BIO_should_retry(bio)) {
			continue;
		} else {
			return -1;
		}
	}

	return 0;

}

size_t tls_write (struct connect *conn, void *buf, size_t len) {
	BIO *bio = conn->priv;
	int ret, loop;

	while (loop-- && ((ret = BIO_write(bio, buf, len)) <= 0)) {
		syslog(LOG_DEBUG, "wirte data error %d: %d", ret, loop);
		if (0 < loop && BIO_should_retry(bio)) {
			continue;
		} else {
			return 0;
		}
	}

	return len;
}

size_t tls_read (struct connect *conn, void *buf, size_t len) {
	BIO *bio = conn->priv;
	int ret, loop;

	while (loop-- && ((ret = BIO_read(bio, buf, len)) <= 0)) {
		syslog(LOG_DEBUG, "read data error %d: %d", ret, loop);
		if (0 < loop && BIO_should_retry(bio)) {
			continue;
		} else {
			return 0;
		}
	}

	return len;

}

void tls_close(struct connect *conn) {
	BIO *bio = conn->priv;

	BIO_free_all(bio);
}


