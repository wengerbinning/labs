#include <stdio.h>

int main (int argc, char *argv[])  {
	
	printf("__LINE__: %d", __LINE__);
	printf("__FILE__: %s", __FILE__);
	
	printf("__DATE__: %s", __DATE__);
	printf("__TIME__: %s", __TIME__);
	
	printf("__STDC__: %d", __STDC__);

	// printf("__cplusplus: %d", __cplusplus);

	return 0;
}



