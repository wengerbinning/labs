#include <stdio.h>



#define MMAX 4
#define NMAX 4
int main (int agrc, char *argv[]) {
	int m = 1, n = 9;
	int M= 0, sm[MMAX] = { 1, 2, 3, 4 };
	double pm[MMAX] = { 0, 0, 0, 0 };
	int N = 0, sn[NMAX] = { 5, 6, 7, 8 };
	double pn[NMAX] = { 0, 0, 0, 0 };

	for (int i = 0; i < MMAX; i++) { M += sm[i]; }
	for (int i = 0; i < NMAX; i++) { N += sn[i]; }

	printf("m: %d -> %2d = %d ", m, M, sm[0]);
	for (int i = 1; i < MMAX; i++) { char c=(i+1 != MMAX?' ':'\n'); printf("+ %d%c", sm[i], c); }
	printf("n: %d -> %2d = %d ", n, N, sn[0]);
	for (int i = 1; i < NMAX; i++) { char c=(i+1 != NMAX?' ':'\n'); printf("+ %d%c", sn[i], c); }

	for (int i = 0; i < MMAX; i++) { pm[i] = ((double)sm[i]/M) * ((double)m/(m+n)); }
	for (int i = 0; i < NMAX; i++) { pn[i] = ((double)sn[i]/N) * ((double)n/(m+n)); }

	printf("m0: %d - %2.0f = %d/%d\n", m, (double)m /(m + n) * 100, m, m + n);
	for (int i = 0; i< MMAX; i++) {
		printf("m%d: %d - %2.0f = %d/%d * %d/%d\n",
			i + 1, sm[i], pm[i] * 100, sm[i], M, m, m + n);
	}
	printf("n0: %d - %2.0f = %d/%d\n", n, (double)n /(m + n) * 100, n, m + n);
	for (int i = 0; i< NMAX; i++) {
		printf("n%d: %d - %2.0f = %d/%d * %d/%d\n",
			i + 1 , sn[i], pn[i] * 100, sn[i], N, n, m + n);
	}
	return 0;
}