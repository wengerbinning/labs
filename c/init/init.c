#include <stdio.h>
#include <unistd.h>
#include <sys/reboot.h>

#define BANNER  "/etc/banner"

/*

access(char *file, int mode)

mode:
	0 - F_OK
	1 - X_OK
	2 - W_OK
	4 - R_OK

	3 - XW_OK
	5 - XR_OK
	6 - WR_OK
 */

#define BUFFERIZE 1024

int main (int argc, char *argv[]) {
	char ch;
	FILE *fp = NULL;
	char buffer[1024];

	sleep(1);

	/* */
	if (0 == access(BANNER, R_OK)) {
		if (fp = fopen(BANNER, "r")) {
			while (!feof(fp)) {
				fgets(buffer, BUFFERIZE, fp);
				printf("%s", buffer);
			}
			fclose(fp);
		}
	}

	printf("\nPress any key to exit ...");
	while ((ch = getchar())) {
		switch (ch) {
			case 'r':
			case 'R':
				reboot(RB_AUTOBOOT);
				break;
			case 'q':
			case 'Q':
				reboot(RB_POWER_OFF);
				break;
		}
	}

	reboot(RB_POWER_OFF);
	return 0;
}
