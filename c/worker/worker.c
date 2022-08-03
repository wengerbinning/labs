#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define S1 " "
#define S2 "!@#$%^&*()"
#define S3 "1234567890"
#define S4 "`-=[];',./"
#define S5 "~_+{}|:\"<>?"
#define S6 "abcdefghijklmnopqrstuvwxyz"
#define S7 "ABCDEFGHIJKLMNOPQRSTUVWSYZ"
#define S S1 S2 S3 S4 S5 S6 S7 "%A"

#define FMT "0-9a-zA-Z"

// 0x2D -
// 0x2E .
// 0x7E ~
// 0x5F _

#define BUFIZE 1024




#define IS_DIGIT(n) ((0x30 <= (n)) && ((n) <= 0x39))
#define IS_UPPER(n) ((0x41 <= (n)) && ((n) <= 0x5A))
#define IS_LOWER(n) ((0x61 <= (n)) && ((n) <= 0x7A))
#define IS_ALPHA(n) (IS_LOWER(n) || IS_UPPER(n))
#define IS_HEXHA(n) (((0x41 <= (n)) && ((n) <= 0x46)))
#define IS_HEXLA(n) (((0x61 <= (n)) && ((n) <= 0x66)))
#define IS_HEX(n) (IS_DIGIT(n) || IS_HEXHA(n) || IS_HEXLA(n))

int urlencode (const char *str, char *buf, size_t size) {
	int i, len, count = 0;
	char *url = buf;

	if (!str || !buf)
		return -1;

	memset(buf, 0x0, size);
	for (i = 0; i < strlen(str); i++) {
		bool encode = false;
		char *fmt, c = str[i];
		if (!IS_DIGIT(c) && !IS_ALPHA(c)) {
			switch (c) {
				case 0x2D: /* - */
				case 0x2E: /* . */
				case 0x7E: /* ~ */
				case 0x5F: /* _ */
					break;
				default:
					encode = true;
					count++;
			}
		}
		fmt = encode ? "%%%02X" : "%c";
		len = encode ? 3 : 1;
		if (size <= (strlen(buf) + len))
			return -1;
		sprintf(url, fmt, c);
		url += len;
	}
	return count;
}
int urldecode (const char *str, char *buf, size_t size) {
	int i, v, count = 0;
	char *url = buf;

	if (!str || !buf)
		return -1;
	memset(buf, 0x0, size);
	for (i = 0; i < strlen(str); i++) {
		bool decode = false;
		char *fmt, c = str[i];
		switch (c) {
		case 0x25: /* % */
			if ((strlen(str) - i) < 3)
				break;
			if (!IS_HEX(str[i + 1]) && !IS_HEX(str[i + 2]))
				break;
			sscanf(str + i, "%%%02X", &v);
			if (IS_DIGIT(v) || IS_ALPHA(v))
				break;
			switch(v) {
				case 0x2D: /* - */
				case 0x2E: /* . */
				case 0x7E: /* ~ */
				case 0x5F: /* _ */
					break;
				default:
					i += 2;
					decode = true;
					count ++;
			}
			break;
		}
		if (size <= (strlen(buf) + 1)) {
			return -1;
		}
		sprintf(url++, "%c", decode ? v : c);
	}



	return count;
}



int main (int agrc, char *argv[]) {
	char buffer1[BUFIZE + 1] = { '1', '2', 0x0 };
	char buffer2[BUFIZE + 1] = { '1', '2', 0x0 };
	char *buf1 = buffer1;
	char *buf2 = buffer2;
	int ret;
	printf("# %d %s\n", strlen(S), S);
	ret = urlencode(S, buf1, BUFIZE);
	printf("@ %d %s\n", ret, buf1);
	ret = urldecode(buf1, buf2, BUFIZE);
	printf("* %d %s\n", ret, buf2);

    return 0;
}


// %20%21%40%23%24%25%5E%26%2A%28%291234567890%60-%3D%5B%5D%3B%27%2C.%2F~_%2B%7B%7D%7C%3A%22%3C%3E%3FabcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWSYZ