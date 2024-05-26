#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#define RESULT_FILE "result.txt"
#define BUFIZE 35

#define HEXDUMP_COLUM	16
#define HEXDUMP_CHAR1	' '
#define HEXDUMP_ASCII	true
#define HEXDUMP_CHAR2	' '

static inline bool str2bool(const char *str) {
	if (!str)
		return false;

	if (0 == strcmp(str, "true") || 0 == strcmp(str, "True"))
		return true;

	if (0 == strcmp(str, "yes") || 0 == strcmp(str, "YES"))
		return true;

	if (1 == atoi(str))
		return true;

	return false;
}

static inline char printchar(char chr) {
	return (chr >31 && chr < 127) ? chr : '.';
}

void hexdump (const void *ptr, unsigned int size) {
	int i, j;
	unsigned char *buf1, *p;
	unsigned char *buf2, *q;
	const char *env;
	char chr1, chr2;
	int col;
	bool ascii;

	if (!ptr)
		return;

	env = getenv("HEXDUMP_COLUM");
	col = env ? atoi(env) : HEXDUMP_COLUM;
	env = getenv("HEXDUMP_CHAR1");
	chr1 = env ? *(char *)env : HEXDUMP_CHAR1;
	env = getenv("HEXDUMP_ASCII");
	ascii = env ? str2bool(env) : HEXDUMP_ASCII;
	env = getenv("HEXDUMP_CHAR2");
	chr2 = env ? *(char *)env : HEXDUMP_CHAR2;

	buf1 = malloc(col * (2 + 1));
	buf2 = malloc(col * (1 + 1));
	memset(buf1, 0, col * 3);
	memset(buf2, 0, col * 2);
	p = buf1;
	q = buf2;

	for (i = 0; i < size; i++) {
		sprintf(p, "%02X", *(unsigned char *)(ptr + i));
		sprintf(q, "%c", printchar(*(unsigned char *)(ptr + i)));
		p = p + 2; q++;

		if (((i + 1) % col)) {
			sprintf(p, "%c", printchar(chr1));
			sprintf(q, "%c", printchar(chr2));
			p++; q++;
		}

		if (!((i + 1) % col)) {
			printf("%s | %s\n", buf1, buf2);
			memset(buf1, 0, col * 3);
			memset(buf2, 0, col * 2);
			p = buf1; q = buf2;
		}
	}

	if (size % col) {
		for (i = 0; i < (col - (size % col)); i++) {
			sprintf(p, "  "); p = p + 2;
			sprintf(q, "%c", printchar(' ')); q++;
			if ((i != (col - (size % col) - 1))) {
				sprintf(p, "%c", chr1);
				sprintf(q, "%c", chr2);
				p++; q++;
			}
		}

		printf("%s | %s\n", buf1, buf2);
	}

	free(buf1);
	free(buf2);
}

#define OVPN_UNKNOWN 0
#define OVPN_CLIENT	1
#define OVPN_ROUTE 2
#define OVPN_STATS 3

int main (int argc, char *argv[]) {
	FILE *file;
	char buffer[BUFIZE + 1];
	int parse = OVPN_UNKNOWN;
	char *ptr, *col1, *col2, *col3, *col4, *col5;




	//file = fopen(RESULT_FILE, "r");
	hexdump(buffer, BUFIZE);

return 0;

	while (!feof(file)) {
		memset(buffer, 0, sizeof(buffer));
		fgets(buffer, BUFIZE, file);

		if (!strlen(buffer))
			continue;

		//printf("line: %s", buffer);
		if (0 == strncmp(buffer, "OpenVPN CLIENT LIST", 19)) {
			parse = OVPN_CLIENT;  continue;
		} else if (0 == strncmp(buffer, "ROUTING TABLE", 13)) {
			parse = OVPN_ROUTE;   continue;
		} else if (0 == strncmp(buffer, "GLOBAL STATS", 12)) {
			parse = OVPN_STATS;   continue;
		}

		if (parse == OVPN_UNKNOWN)
			continue;

		ptr = buffer;
		switch (parse) {
			case OVPN_CLIENT:
				col1=ptr;
				if ((ptr = strstr(ptr, ",")) && ptr < (buffer + BUFIZE)) {
					*ptr = '\0'; ptr = ptr + 1;
				}

				if (0 == strcmp(col1, "Updated") || 0 == strcmp(col1, "Common Name"))
					continue;

				col2 = ptr;
				if ((ptr = strstr(ptr, ",")) && ptr < (buffer + BUFIZE)) {
					*ptr = '\0'; ptr = ptr + 1;
				}
				col3 = ptr;
				if ((ptr = strstr(ptr, ",")) && ptr < (buffer + BUFIZE)) {
					*ptr = '\0'; ptr = ptr + 1;
				}
				col4 = ptr;
				if ((ptr = strstr(ptr, ",")) && ptr < (buffer + BUFIZE)) {
					*ptr = '\0'; ptr = ptr + 1;
				}
				col5 = ptr;
				if ((ptr = strstr(ptr, "\n")) && ptr < (buffer + BUFIZE)) {
					*ptr = '\0';
				}
				printf("column: %s\n", col5);
				break;

			case OVPN_ROUTE:
				col1=ptr;
				if ((ptr = strstr(ptr, ",")) && ptr < (buffer + BUFIZE)) {
					*ptr = '\0'; ptr = ptr + 1;
				}

				if (0 == strcmp(col1, "Virtual Address"))
					continue;

				col2 = ptr;
				if ((ptr = strstr(ptr, ",")) && ptr < (buffer + BUFIZE)) {
					*ptr = '\0'; ptr = ptr + 1;
				}
				col3 = ptr;
				if ((ptr = strstr(ptr, ",")) && ptr < (buffer + BUFIZE)) {
					*ptr = '\0'; ptr = ptr + 1;
				}
				col4 = ptr;
				if ((ptr = strstr(ptr, "\n")) && ptr < (buffer + BUFIZE)) {
					*ptr = '\0';
				}
				printf("column: %s\n", col4);
				break;
			case OVPN_STATS:
				break;
			default:
				printf("Nothing ....\n");
		}

	}

	fclose(file);

	return 0;
}