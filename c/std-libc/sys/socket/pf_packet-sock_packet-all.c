#include <linux/if_ether.h>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include "socket.h"
#include "packet.h"
#include "develop.h"
#include "tools.h"

#include "ethertype.h"



int pf_packet_packet_all (struct socket_param_t *param) {
	int sock, len;
	struct sockaddr addr;
	// char buffer[1024];
    void *packet;

	if ((sock = socket(PF_PACKET, SOCK_PACKET, htons(ETH_P_ALL))) < 0) {
		error("Failed to create socket!");
		exit(sock);
	}

	strcpy(addr.sa_data, param->iface);


	// debug("#%02x", CHAR(ETHER_TYPE_8021Q_CVID, 0));

	// memset(buffer, 0, sizeof(buffer));
//	char buffer[64] = {
//	// DSTMAC
//	0xC0, 0x74, 0xAD, 0xB9, 0xF0, 0x4C,
//	// SRCMAC
//	0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
//	// TYPEi <https://www.iana.org/assignments/ieee-802-numbers/ieee-802-numbers.xhtml>
//
//	// CHAR(ETHER_TYPE_8021Q_CVID, 1), CHAR(ETHER_TYPE_8021Q_CVID, 0),
//	// 0x00, 0x0A,
//
//	*//* IPv4 *//*
//
//	CHAR(ETHER_TYPE_IPV4, 1), CHAR(ETHER_TYPE_IPV4, 0),
//
//
//	// IPv4, 64B, UDP,
//	// 0x0045,0x00,  0x00,0x40, 0x00,0x00, 0x00,0x00, 0x40,0x11,  0x00,0x00,
//	// IPv4, TCP
//	// 0x0045,0x00,  0x01,0x50, 0x00,0x00, 0x00,0x00, 0x40,0x11,  0x00,0x00,
//
//	// SRC ADDR
//	// 0x00,0x00,0x00,0x00,
//	// DST ADDR
//	// 0x00,0x00,0x00,0x00,
//
//	*//* UDP *//*
//
//	// 0x00,0x02, 0x00,0x01, 0x00, 0x20,
//
//	*//* DATA */
//
//	0x0
//	};len

    len = new_packet(&packet, param->file);
	if (len < 0) {
        close(sock);
        return -1;
    }
	
    hexdump(packet, len);

	sendto(sock, packet, len, 0, &addr, sizeof(addr));

    free_packet(packet);
	close(sock);
	
	return 0;
}
