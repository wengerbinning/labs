#include <stdio.h>






int main (int argc, char *argv[]) {
	char iface[512];
	unsigned int metric = 0, weight = 0;
	if ( !argv[1] ) {
		printf("please enter parameter!\n");
		return -1;
	}

	int ret = sscanf(argv[1], "%2[^/]/%u/%u", iface, &metric, &weight);


	printf("ret %d, iface: %s, metric: %u, weight: %u\n",ret, iface, metric, weight);

	return 0;
}