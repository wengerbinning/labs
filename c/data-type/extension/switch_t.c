
#include <stdio.h>

#include "switch_t.h"

int main (int agrc, char *argv[]) {
	bool enable = true;
	switch_t power = { ._bool = false };

	SWITCH_SET_VALUE(power, true);

	printf("Switch 0x%02X, %s, %s, %s\n", power,
		power.valid ? "true" : "false",
		SWITCH_TEST_KEEP(power) ? "true" : "false",
		SWITCH_GET_VALUE(power) ? "true" : "false");

	SWITCH_INIT_KEEP(power, false);

	printf("sizeof: %d, %d\n", sizeof(bool), sizeof(switch_t));



	SWITCH_SET_VALUE(power, true);


	printf("Switch 0x%02X, %s, %s, %s\n", power,
		power.valid ? "true" : "false",
		SWITCH_TEST_KEEP(power) ? "true" : "false",
		SWITCH_GET_VALUE(power) ? "true" : "false");
	return 0;
}