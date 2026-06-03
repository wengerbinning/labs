
#include <stdio.h>
// #include <windows.h>

#include "version.h"










int main (int argc, const char argv[]) {
	printf(VERFMT0 "\n", VER2STR(VERSION_CODE));
	return 0;
}
