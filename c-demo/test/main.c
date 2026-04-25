#include <stdio.h>

//
#include "device.h"

device_t device, *dev = &device;

#define BUFIZE 1024
char buffer[BUFIZE + 1];



char *func0 (uint64_t stat) {
	bool sep = false;
	char *buf = buffer;

	memset(buffer, 0, BUFIZE + 1);
	for (int i = 0; i < 64; i++) {
		switch(1LL << i) {
		case DEV_STAT_BOOT:
			if (sep) bnprintf(buf, BUFIZE, ", "); sep = true;
			bnprintf(buf, BUFIZE, "%s", (stat & DEV_STAT_BOOT) ? "booting" : "started");
			break;
		case DEV_STAT_DEVELOP:
			if (sep) bnprintf(buf, BUFIZE, ", "); sep = true;
			bnprintf(buf, BUFIZE, "%s", (stat & DEV_STAT_DEVELOP) ? "develop" : "released");
			break;
		case DEV_STAT_ACTIVATE:
			if (sep) bnprintf(buf, BUFIZE, ", "); sep = true;
			bnprintf(buf, BUFIZE, "%s", (stat & DEV_STAT_ACTIVATE) ? "activated" : "unactive");
			break;
		case DEV_STAT_REGIST:
			if (sep) bnprintf(buf, BUFIZE, ", "); sep = true;
			bnprintf(buf, BUFIZE, "%s", (stat & DEV_STAT_REGIST) ? "registerd" : "unregister");
			break;
		case DEV_STAT_MANAGE:
			if (sep) bnprintf(buf, BUFIZE, ", "); sep = true;
			bnprintf(buf, BUFIZE, "%s", (stat & DEV_STAT_MANAGE) ? "managered" : "unmanager");
			break;

		//
		case DEV_STAT_POWEROFF:
			if (stat & DEV_STAT_POWEROFF) {
				if (sep) bnprintf(buf, BUFIZE, ", "); sep = true;
				bnprintf(buf, BUFIZE, "poweroff");
			}
			break;
		case DEV_STAT_REBOOT:
			if (stat & DEV_STAT_REBOOT) {
				if (sep) bnprintf(buf, BUFIZE, ", "); sep = true;
				bnprintf(buf, BUFIZE, "reboot");
			}
			break;
		case DEV_STAT_RESET:
			if (stat & DEV_STAT_RESET) {
				if (sep) bnprintf(buf, BUFIZE, ", "); sep = true;
				bnprintf(buf, BUFIZE, "reset");
			}
			break;
		case DEV_STAT_UPGRADE:
			if (stat & DEV_STAT_UPGRADE) {
				if (sep) bnprintf(buf, BUFIZE, ", "); sep = true;
				bnprintf(buf, BUFIZE, "upgrade");
			}
			break;


		}
	}

	return buf;
}



int main (int agrc, char *argv[]) {


	printf("%p - id:0x%08X, name:%s, state:%08X (%s)\n", dev, dev->id, dev->name, dev_get_stat(dev, 0), func0(dev_get_stat(dev, 0)));

	return 0;
}
