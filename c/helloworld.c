#include <stdio.h>


#define A1(m)   ((1<< (m)) - 1)
#define A2(m)  ~A1(m)
#define FUNC(n, m) ((n) + (1 << (m)) - 1) & ~((1 << (m)) - 1)

int main (int argc, char *argv[]) {
	int n = 5;
	int m = 2;
	printf("Hello, World! %d, %d > %d %d = %d\n", n, m, A1(m), A2(m), FUNC(n, m));
	return 0;
}
