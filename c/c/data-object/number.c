
// vim: set ts=4

#include <stdio.h>
#include <math.h>
#define BIN_POW(n)			(1LU << n)
#define INT_SIZE(type)			(sizeof(type) * 8)
#define INT_MIN_VALUE(type) 		(-1 * BIN_POW(INT_SIZE(type) - 1))
#define	INT_MAX_VALUE(type)		(BIN_POW(INT_SIZE(type) - 1) - 1)
#define UINT_MIN_VALUE(type)		0
#define UINT_MAX_VALUE(type)		(BIN_POW(INT_SIZE(type) - 1))

#define NUMBER_DUMP(type) \
	printf("size: %d, min: %lld,  max: %lld,  # %s\n", INT_SIZE(type), INT_MIN_VALUE(type), INT_MAX_VALUE(type), #type)


// int main() {
// 	printf("         char  size %2lu, min value %16ld, max value %16ld\n", INT_SIZE(char), INT_MIN_VALUE(char), INT_MAX_VALUE(char));
// 	printf("unsigned char  size %2lu, min value %16ld, max value %16ld\n", INT_SIZE(unsigned char), UINT_MIN_VALUE(unsigned char), UINT_MAX_VALUE(unsigned char));
// 	printf("	 short size %2lu, min value %16ld, max value %16ld\n", INT_SIZE(short), INT_MIN_VALUE(short), INT_MAX_VALUE(short));
// 	printf("unsigned short size %2lu, min value %16ld, max value %16ld\n", INT_SIZE(unsigned short), UINT_MIN_VALUE(unsigned short), UINT_MAX_VALUE(unsigned short));
// 	printf("         int   size %2lu, min value %16ld, max value %16ld\n", INT_SIZE(int), INT_MIN_VALUE(int), INT_MAX_VALUE(int));
// 	printf("unsigned int   size %2lu, min value %16ld, max value %16ld\n", INT_SIZE(unsigned int), UINT_MIN_VALUE(unsigned int), UINT_MAX_VALUE(unsigned int));
// 	printf("         long  size %2lu, min value %16ld, max value %16ld\n", INT_SIZE(long), INT_MIN_VALUE(long), INT_MAX_VALUE(long));
// 	printf("unsigned long  size %2lu, min value %16ld, max value %16ld\n", INT_SIZE(unsigned long), UINT_MIN_VALUE(unsigned long), INT_MAX_VALUE(unsigned long));
// 	return 0;
// }






int main (int argc, char *argv) {
	NUMBER_DUMP(int);
	NUMBER_DUMP(long);
	NUMBER_DUMP(long long);


	if (0 && 0 <= 0) {
		printf("N\n");
	}

	return 0;
}