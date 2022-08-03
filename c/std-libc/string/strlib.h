#ifndef __STRLIB_H__
#define __STRLIB_H__

#include <string.h>

#define STR2UPPER(s) do { if (s) { int i; \
	for (i = 0; i < strlen(s); i++) {     \
		if ('a' <= s[i] && s[i] <= 'z')   \
			s[i] -= 0x20;                 \
	}                                     \
} } while(0)

#define STR2LOWER(s) do { if (s) { int i; \
	for (i = 0; i < strlen(s); i++) {     \
		if ('A' <= s[i] && s[i] <= 'Z')   \
			s[i] += 0x20;                 \
	}                                     \
} } while(0)

#endif /* __STRLIB_H__ */
