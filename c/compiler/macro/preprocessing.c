#include <stdio.h>





int main(int argc, char *argv[]) {
	


#if ! (defined(DEBUG) || defined(VERSION))
	printf("[%s(%d) debug] run in here", __func__, __LINE__);
#endif

	
	return 0;
}	

