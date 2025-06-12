#include <stdio.h>
#include <string.h>

/*
 * && - 按照顺序,如果逻辑中存在一个为0的值,则停止计算并返回逻辑值.
 * || - 按照顺序,如果逻辑中存在一个为1的值,则停止计算并返回逻辑值.
 */

int main (int argc, char *argv[]) {

	if ( 0 && strlen(NULL) ) {
		printf("&& FAIL\n");
	}
	printf("&& PASS\n");

	if ( 1 || strlen((NULL)) ) {
		printf("|| PASS\n");
		return 0;
	}

	printf("|| FAIL\n");
	return 0;
}