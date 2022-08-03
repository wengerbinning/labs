#ifndef __USER_SOCKET_H__
#define __USER_SOCKET_H__

struct socket_param_t {
	char *prog;
	int verbose;
	int server;
	char *iface;
	char address[64];
    char *file;
};



int pf_packet_packet_all (struct socket_param_t *param);

#endif /* __USER_SOCKET_H__ */