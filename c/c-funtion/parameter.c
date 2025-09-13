#include <stdio.h>
#include <string.h>
#include <math.h>

enum {
	AUTO_SYS = 0,
	BIN_SYS = 2,
	OCT_SYS = 8,
	DEC_SYS = 10,
	HEX_SYS = 16,
	__MAX_SYS = 0xFFFF
};


struct element_t {
	unsigned int cnt;
	const char *data; 
};

struct element_t sys_char[] = {
	{1, "0"},
	{1, "1"},
	{1, "2"},
	{1, "3"},
	{1, "4"},
	{1, "5"},
	{1, "6"},
	{1, "7"},
	{1, "8"},
	{1, "9"},
	{2, "aA"},
	{2, "bB"},
	{2, "cC"},
	{2, "dD"},
	{2, "eE"},
	{2, "fF"}
};

#define BIN_SYS_MASK	0x0003
#define OCT_SYS_MASK	0x00FF
#define DEC_SYS_MASK	0x03FF
#define HEX_SYS_MASK	0xFFFF

int str2int (const char *str, unsigned int mode);
int get_char (const char *chr, unsigned int mask);
int power(int base, int n);

int main( int agrc, char *argv[]) {
	int value;
	// printf("%s\n", argv[1]);
	value = str2int(argv[1], AUTO_SYS);

	printf("0x%08X\n", value);
	return 0;
}


int str2int (const char *str, unsigned int mode) {
	int len = strlen(str);
	int sys;
	int offset;
	int i, idx;
	char *p;
	unsigned int mask;
	int value;
	unsigned int result = 0;
	
	sys = mode;
	switch (mode) {
		case BIN_SYS:
			offset = 0;
			sys = 2;
			mask = BIN_SYS_MASK;
			break;
		case OCT_SYS:
			offset = 0;
			sys = 8;
			mask = OCT_SYS_MASK;
			break;
		case DEC_SYS:
			offset = 0;
			sys = 10;
			mask = DEC_SYS_MASK;
			break;
		case HEX_SYS:
			offset = 0;
			sys = 16;
			mask = HEX_SYS_MASK;
			break;
		case AUTO_SYS:
		default:
			if (0 == strncmp(str, "0b", 2) | 0 == strncmp(str, "0B", 2)) {
				offset = 2;
				sys = 2;
				len -= 2;
				mask = BIN_SYS_MASK;
			} else if (0 == strncmp(str, "0o", 2) | 0 == strncmp(str, "0O", 2)) {
				offset = 2;
				sys = 8;
				len -= 2;
				mask = OCT_SYS_MASK;
			} else if (0 == strncmp(str, "0d", 2) | 0 == strncmp(str, "0D", 2)) {
				offset = 2;
				sys = 10;
				len -= 2;
				mask = DEC_SYS_MASK;
			} else if (0 == strncmp(str, "0x", 2) | 0 == strncmp(str, "0X", 2)) {
				offset = 2;
				sys = 16;
				len -= 2;
				mask = HEX_SYS_MASK;
			} else {
				offset = 0;
				sys = 10;
				mask = DEC_SYS_MASK;
			}
	}

	// printf("length %d, offset %d, mode %d, mask 0x%02X\n", len, offset, sys, mask);

	for (i = 0, p = ((char *)str + offset + i); p && i < len; i++, p++) {
		idx = len - i;

		if ((value = get_char(p, mask)) <= 0) {
			continue;
		}

		result += (value * power(sys, (idx-1)));
	}

	return result;
}


int get_char (const char *chr, unsigned int mask) {
	int i, j;
	
	for (i = 0; (mask & (1 << i)); i++) {
		for (j = 0; j < sys_char[i].cnt; j++) {
			if ( !(sys_char[i].data + j) )
				return -1;
			
			if (*chr == *(sys_char[i].data + j)) {
				return i;
			}
		}
	}
	
	return -1;
}

int power(int base, int n) {
	if (n == 0)
		return 1;
	else
		return (base * power(base, n-1));
}




