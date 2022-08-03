



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
