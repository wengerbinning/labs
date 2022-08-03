#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define _XOPEN_SOURCE
#define __USE_XOPEN
#include <time.h>
#include <libubox/blobmsg.h>
#include <libubox/blobmsg_json.h>
#include <libubox/kvlist.h>

#include <regex.h>
#include <sys/types.h>

#define RESULT_FILE "result.txt"
#define BUFIZE 256



#define IP_ADDR
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

void hexdump (const void *ptr, unsigned int size) {
	int i, j;
	unsigned char *buf;
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

	buf = malloc(col);
	memset(buf, 0, col);

	for (i = 0; i < size; i++) {
		printf("%02X", *(unsigned char *)(ptr + i));
		buf[i % col] = *(unsigned char *)(ptr + i);

		if (((i + 1) % col) || ascii)
			printf("%c", chr1);
		else
			printf("\n");

		if (!((i + 1) % col) && ascii) {
			printf(" | ");
			for (j = 0; j < col; j++) {
				printf("%c", (buf[j] > 31 && buf[j] < 128) ? buf[j] : '.');
				if (j != col - 1)
					printf("%c", chr2);
			}
			memset(buf, 0, col);
			printf ("\n");

		}
	}

	if (size % col) {
		for (i = 0; i < (col - (size % col)); i++) {
			printf("  ");
			if ((i != (col - (size % col) - 1)) || ascii)
				printf("%c", chr1);
		}
		if (ascii) {
			printf(" | ");
			for (j = 0; j < col; j++) {
				printf("%c", (buf[j] > 31 && buf[j] < 127) ? buf[j] : '.');
				if (j != col - 1)
					printf("%c", chr2);
			}
		}
		printf("\n");
	}

	free(buf);
}

#define OVPN_UNKNOWN 0
#define OVPN_CLIENT	1
#define OVPN_ROUTE 2
#define OVPN_STATS 3

int main (int argc, char *argv[]) {
	struct kvlist kv_list;
	struct blob_buf *b;
	const char *name;
	FILE *file;
	char buffer[BUFIZE + 1];
	int process = OVPN_UNKNOWN;

	kvlist_init(&kv_list,get_len_blob_buf);

	if ((file = fopen(RESULT_FILE, "r"))) {
		char *ptr, *col1, *col2, *col3, *col4, *col5, *col6;
		struct tm tm;
		time_t srt_time, cur_time;
		int len;
		unsigned long second;
		regex_t regex;
#define REGEX_MATCH_COUNT 1
		size_t nmatch = REGEX_MATCH_COUNT;
		regmatch_t pmatch[REGEX_MATCH_COUNT];

		while ( !feof(file)) {
			memset(buffer, 0, sizeof(buffer));
			fgets(buffer, BUFIZE, file);

			if (!strlen(buffer))
				continue;

			/** Initial parsing process */
			if (0 == strncmp(buffer, "OpenVPN CLIENT LIST", 19)) {
				process = OVPN_CLIENT;  continue;
			} else if (0 == strncmp(buffer, "ROUTING TABLE", 13)) {
				process = OVPN_ROUTE;   continue;
			} else if (0 == strncmp(buffer, "GLOBAL STATS", 12)) {
				process = OVPN_STATS;   continue;
			}

			/** */
			if (process == OVPN_UNKNOWN)
				continue;

			/** Parsing client info */
			ptr = buffer;
			switch (process) {
				case OVPN_CLIENT:
					// Prefix or Common Name
					col1=ptr;
					if ((ptr = strstr(ptr, ",")) && ptr < (buffer + BUFIZE)) {
						*ptr = '\0'; ptr = ptr + 1;
					}

					if (0 == strcmp(col1, "Updated") ||
						0 == strcmp(col1, "Common Name"))
						continue;

					// Real address
					col2 = ptr;
					if ((ptr = strstr(ptr, ",")) && ptr < (buffer + BUFIZE)) {
						*ptr = '\0'; ptr = ptr + 1;
					}
					// Protocol Port
					if ((col3 = strstr(col2, ":"))) {
						*col3 = '\0'; col3 = col3 + 1;
					}
					// RX
					col4 = ptr;
					if ((ptr = strstr(ptr, ",")) && ptr < (buffer + BUFIZE)) {
						*ptr = '\0'; ptr = ptr + 1;
					}
					// TX
					col5 = ptr;
					if ((ptr = strstr(ptr, ",")) && ptr < (buffer + BUFIZE)) {
						*ptr = '\0'; ptr = ptr + 1;
					}
					// Connected Since
					col6 = ptr;
					if ((ptr = strstr(ptr, "\n")) && ptr < (buffer + BUFIZE)) {
						*ptr = '\0';
					}

					// Add client object
					b = malloc(sizeof(struct blob_buf));
					memset(b, 0, sizeof(struct blob_buf));
					blob_buf_init(b, 0);

					blobmsg_add_string(b, "userName", col1);
					blobmsg_add_string(b, "ipAddress", col2);

					strptime(col6, "%Y-%m-%d %H:%M:%S", &tm);
					srt_time = mktime(&tm);
					cur_time = time(NULL);
					len = strlen(col6);
					memset(col6, 0, len);
					snprintf(col6, len, "%ld", difftime(srt_time, cur_time));
					blobmsg_add_string(b, "connectDuration", col6);

					kvlist_set(&kv_list, col1 ,b);
					free(b);
					break;

				case OVPN_ROUTE:
					// Prefix or Virtual Address
					col1=ptr;
					if ((ptr = strstr(ptr, ",")) && ptr < (buffer + BUFIZE)) {
						*ptr = '\0'; ptr = ptr + 1;
					}

					// Skip non virtual address information.
					if (0 == strcmp(col1, "Virtual Address"))
						continue;

					regcomp(&regex, "^[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}$", REG_EXTENDED);
					if (0 != regexec(&regex, col1, nmatch, pmatch, 0)) {
						continue;
					}
					regfree(&regex);

					// User Name
					col2 = ptr;
					if ((ptr = strstr(ptr, ",")) && ptr < (buffer + BUFIZE)) {
						*ptr = '\0'; ptr = ptr + 1;
					}

					b = (struct blob_buf *)kvlist_get(&kv_list, col2);
					if (!b)
						continue;

					// Real Address
					col3 = ptr;
					if ((ptr = strstr(ptr, ",")) && ptr < (buffer + BUFIZE)) {
						*ptr = '\0'; ptr = ptr + 1;
					}
					// Last Ref
					col4 = ptr;
					if ((ptr = strstr(ptr, "\n")) && ptr < (buffer + BUFIZE)) {
						*ptr = '\0';
					}

					blobmsg_add_string(b, "virtualIpAddress", col1);
					break;
				case OVPN_STATS:
					break;
			}
		}

		fclose(file);
	}

	kvlist_for_each(&kv_list, name, b) {
		printf("blob:%s\n",blobmsg_format_json_indent(b->head, true, -1));

	}

	kvlist_free(&kv_list);
	return 0;
}