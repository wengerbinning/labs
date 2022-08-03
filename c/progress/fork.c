#include<stdio.h>
#include<unistd.h>
#include <sys/wait.h>

#define note(fmt, ...)         printf(fmt"\n", ## __VA_ARGS__)

int main (int argc, const char *argv[]) {
	
	pid_t pid = 0;
	int ret;

	pid = fork();
	if ( pid == 0 ) {
		execl("./worker", "/worker", "-t", "10", NULL);
	}

	pid = fork();
	if ( pid == 0 ) {
		execl("./worker", "/worker", "-t", "10", NULL);
	}


	// wait(&ret);
	while ((pid = waitpid(-1, &ret, 0)) > 0) {
		note("sub progress(%d) return code: %d",pid, WEXITSTATUS(ret));
	}

	return 0;
}
