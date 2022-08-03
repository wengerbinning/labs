#include <stdio.h>

/* Feature Description
Tests whether different logical block definitions of the same variable within 
the same funtion are available.

*/

int main(int argc, char *argv[]) {

	switch(argc) {
		case 1: {
			int a = 1;
			printf("Found %d argument and a %d\n", argc, a);
		}
			break;
		case 2: {
			int a = 2;
			printf("Found %d argument and a %d\n", argc, a);
		}
			break;
		case 3: {
			int a = 3;
			printf("Found %d argument and a %d\n", argc, a);
		}
			break;
		default: {
			int a = 0;
			printf("Found %d argument and a %d\n", argc, a);
		}
	}

    return 0;
}