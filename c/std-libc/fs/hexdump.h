#ifndef __HEXDUMP_H__
#define __HEXDUMP_H__

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef note
#define  note(fmt, ...) printf(fmt"\n", ## __VA_ARGS__)
#endif

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

void hexdump (const void *ptr, unsigned int size);

#endif /* __HEXDUMP_H__ */