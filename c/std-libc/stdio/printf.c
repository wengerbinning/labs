#include <stdio.h>

#include <time.h>
#include <unistd.h>



int printf_state() {
	for (int i = 0; i< 10; i++) {
		printf("\r\a status %d ..", i);
		fflush(stdout);
		sleep(1);
	}
	printf("\n");
	return 0;
}



int main(int argc, char *argv[]) {

	printf_state();

	return 0;
}





