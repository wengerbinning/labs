#include <stdio.h>

typedef struct eth_addr {
	union {
	unsigned char oui[3];
		struct {
			unsigned char _0:6;
			unsigned char ig:1;
			unsigned char ul:1;
		};
	};
	union {
		unsigned char eui[3];
	};
} eth_addr_t;




int main(int argc, char argv[]) {
	eth_addr_t addr = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };



	printf("%02X-%02X-%02X-%02X-%02X-%02X %s %s\n",
		addr.oui[0], addr.oui[1], addr.oui[2],
		addr.eui[0], addr.eui[1], addr.eui[2],
		(addr.oui[0] & 0x02) ? "私有" : "全球",
		(addr.oui[0] & 0x01) ? "组播" : "单播"
	);



	return 0;
}