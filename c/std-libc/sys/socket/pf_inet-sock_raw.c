#include <sys/socket.h>

int main(int argc, char *argv[]) {
	int fd;

	fd =socket(PF_INET, SOCK_RAW, IPPROTO_TCP);

	return 0;
}
