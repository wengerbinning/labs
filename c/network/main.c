#include <pcap.h>

#include <stdio.h>
#include <stdlib.h>

/* 抓包回调函数 */

/*
DST MAC - 6
SRC MAC - 6
 */

struct device;
struct device_ops;

struct device {
	void *ctx, *priv;
	size_t count, bytes;
	const char *name;
	struct device_ops *ops;
	char emsg[PCAP_ERRBUF_SIZE];

	struct bpf_program bpf;
};

struct device_ops {
	int (*open)(struct device *dev);
	int (*filter)(struct device *dev, const char *pattern);
	int (*capture)(struct device *dev, pcap_handler callback);
	int (*stop)(struct device *dev);
	int (*close)(struct device *dev);
};

typedef struct device device_t;
typedef struct device_ops device_ops_t;



extern void hexdump (const void *data, unsigned int size);


struct ethernet {
	unsigned char dst[6];
	unsigned char src[6];
	unsigned short type;
} __attribute__((packed));

typedef struct ethernet ethernet_t;


void packet_parse (u_char *user, const struct pcap_pkthdr *h, const u_char *data) {
	device_t *dev = (device_t *)user;

	dev->count++;
	// h->len, h->ts.tv_sec, h->ts.tv_usec,
	ethernet_t *eth = (ethernet_t *)data;

	printf("#%04ld %04X\n", dev->count, eth->type);
	hexdump(data, h->len);
}



static int dev_open (device_t *dev) {
	pcap_t *pcap;
	pcap_if_t *devices;
	if (pcap_findalldevs(&devices, dev->emsg) < 0) {
		fprintf(stderr, "not found device: %s\n", dev->emsg);
		return -1;
	}
	dev->priv = devices;
	if (!(pcap = pcap_open_live(dev->name, BUFSIZ, 1, 1000, dev->emsg))) {
		fprintf(stderr, "open device error:%s\n", dev->emsg);
		return -1;
	}
	dev->ctx = pcap;

	// for (i = 0, dev = devices; dev != NULL; dev = dev->next) {
	// 	printf("#%02d %s", ++i, dev->name);
	// 	if (dev->description)
	// 		printf(": %s\n", dev->description);
	// 	else
	// 		printf("\n");

	// 	for (pcap_addr_t *a = dev->addresses; a; a = a->next) {
	// 		if (a->addr && a->addr->sa_family == AF_INET) {
	// 			struct sockaddr_in *saddr = (struct sockaddr_in *)a->addr;
	// 			printf("  IPv4 Addr: %s\n", inet_ntoa(saddr->sin_addr));
	// 		}
	// 	}
	// }
	return 0;
}

static int dev_filter (device_t *dev, const char *pattern) {
	pcap_t *pcap = (pcap_t *)dev->ctx;
	bpf_u_int32 net;

	if (pcap_compile(pcap, &dev->bpf, pattern, 0, net) < 0) {
		fprintf(stderr, "filter pattern error!\n");
		return -1;
	}
	if (pcap_setfilter(pcap, &dev->bpf) == -1) {
		fprintf(stderr, "filer failed!\n");
		return -1;
	}

	return 0;
}

static int dev_capture (device_t *dev, pcap_handler callback) {
	pcap_t *pcap = (pcap_t *)dev->ctx;
	return pcap_loop(pcap, -1, callback, (void *)dev);
}

static int dev_stop (device_t *dev) {
	return 0;
}

static int dev_close (device_t *dev) {

	// pcap_freecode(&dev->bpf);

	if (dev->ctx)
		pcap_close(dev->ctx);

	if (dev->priv)
		pcap_freealldevs(dev->priv);

	printf("捕获完成, 共 %ld 个包 \n", dev->count);
	return 0;
}

struct device_ops _devops = {
	.open = dev_open,
	.filter = dev_filter,
	.capture = dev_capture,
	.stop = dev_stop,
	.close = dev_close,
};

//
struct device _device;

int main() {
	device_t *dev = &_device;

	dev->name = "enp6s0";
	dev->ops = &_devops;

	if ((dev->ops->open(dev)) < 0) {
		fprintf(stderr, "无法打开设备: %s\n", dev->emsg);
		return -1;
	}

	printf("开始捕获...\n");
	dev->ops->capture(dev, &packet_parse);

	dev->ops->close(dev);
	return 0;
}
