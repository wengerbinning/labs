#ifndef __BUFFER_H__
#define __BUFFER_H__

#ifndef BUFIZE
#define BUFIZE 1024
#endif

/** Printf content into buffer.

 bprintf - print content into arrary buffer.
 bnprintf - print content into pointer buffer.

 */
#define bprintf(buf, fmt, ...) do { \
	int l = sizeof(buf) - strlen(buf);       \
	if (0 < l) {                             \
		char *p = buf + strlen(buf);         \
		snprintf(p, l, fmt, ## __VA_ARGS__); \
	} \
} while(0)

#define bnprintf(buf, len, fmt, ...) do { \
	int l = len - strlen(buf);               \
	if (0 < l) {                             \
		char *p = buf + strlen(buf);         \
		snprintf(p, l, fmt, ## __VA_ARGS__); \
	} \
} while(0)

#endif /* __BUFFER_H__ */
