#include <stdio.h>
#include <signal.h>
#include <unistd.h>


void SIG_handler (int sig) {
	switch(sig) {
	case SIGHUP:
	/*
	 */
		printf("handler %d - SIGHUP\n", sig);
		break;
	case SIGINT:
		printf("handler %d - SIGINT\n", sig);
		break;
	case SIGQUIT:
		printf("handler %d - SIGQUIT\n", sig);
		break;
	case SIGILL:
		printf("handler %d - SIGILL\n", sig);
		break;
	case SIGTERM:
		printf("handler %d - SIGTERM\n", sig);
		break;
	default:
		printf("handler %d - SIG\n", sig);
	}
}


int main (int argc, char *argv[]) {

	signal(SIGQUIT, SIG_handler);
	signal(SIGTERM, SIG_handler);

	printf("task: %lld\n", getpid());
	while(1) {};

	return 0;
}