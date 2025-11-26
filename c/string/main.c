#include <stdio.h>
#include <string.h>

#define develop(fmt, ...) \
	printf("%s:%d %s: " fmt "\n", __FILE__, __LINE__, __func__, ## __VA_ARGS__)

#define check_point develop("running in here")

int func1 (char c) {

	switch (c) {
	case 0x21: /* ! */
	case 0x40: /* @ */
	case 0x23: /* # */
	case 0x24: /* $ */
	case 0x25: /* % */
	case 0x5E: /* ^ */
	case 0x26: /* & */
	case 0x2A: /* * */
	case 0x28: /* ( */
	case 0x29: /* ) */
		printf("0x%02X '%c'\n", c, c);
		break;

	case 0x60: /* ` */
	case 0x2D: /* - */
	case 0x3D: /* = */
	case 0x5B: /* [ */
	case 0x5D: /* ] */
	case 0x5C: /* \ */
	case 0x3B: /* ; */
	case 0x27: /* ' */
	case 0x2C: /* , */
	case 0x2E: /* . */
	case 0x2F: /* / */
		printf("0x%02X '%c'\n", c, c);
		break;
	case 0x7E: /* ~ */
	case 0x5F: /* _ */
	case 0x2B: /* + */
	case 0x7B: /* { */
	case 0x7D: /* } */
	case 0x7C: /* | */
	case 0x3A: /* : */
	case 0x22: /* " */
	case 0x3C: /* < */
	case 0x3E: /* > */
	case 0x3F: /* ? */
		printf("0x%02X '%c'\n", c, c);
		break;

	case 0x20: /*   */
		printf("0x%02X '%c'\n", c, c);
		break;

	default:
	}
}

int func0 (const char *str) {
	size_t len;
	int n[6] = { 0 };

	if (!str)
		return -1;

	len = strlen(str);
	for (int i = 0; i < len; i++) {
		char c = str[i];
		if (0x30 <= c && c <= 0x39) {
			n[0]++;
		} else if (0x41 <= c && c <= 0x5A) {
			n[1]++;
		} else if (0x61 <= c && c <= 0x7A) {
			n[2]++;
		} else {
			n[3]++;
			func1(c);
		}
	}

	printf("# %d %d %d %d\n", n[0], n[1], n[2], n[3]);
	return 0;
}



int main (int argc, char *argv[]) {

	int ret;

	ret = func0(argv[1]);

	develop("# return %d", ret);
	return 0;
}