#include <stdio.h>
#include <stdlib.h>

#include <develop.h>


int main(int agrc, char *argv[]) {
	char *ptr = NULL;

	ptr = malloc(256);

	if (ptr)
		printf("%s\n", ptr);

	trace("main", "test");
	return 0;
}