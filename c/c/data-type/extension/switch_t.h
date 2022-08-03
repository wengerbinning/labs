#ifndef __SWITCH_T_H__
#define __SWITCH_T_H__

#include <stdbool.h>

typedef union {
	bool _bool;
	unsigned char _raw;
	struct {
		unsigned char value: 1;
		unsigned char flags: 7;
	};
	struct {
		unsigned char _recv: 6;
		unsigned char  keep: 1;
		unsigned char valid: 1;
	};
} switch_t;
#define _CHK_KEEP(s, v) ((s)._raw & (((s).value == ((v) & 0x01)) ? 0xFF : 0xBF))
#define _SWITCH_FLAGS(s, v) (0x80 | _CHK_KEEP(s, v))
#define SWITCH_INIT_KEEP(s, v) ((s)._raw = 0xC0 | ((v) & 0x01))
#define SWITCH_SET_VALUE(s, v) ((s)._raw = (_SWITCH_FLAGS(s, v)) | ((v) & 0x01))
#define SWITCH_GET_VALUE(s) ((s).valid ? ((s)._raw & 0x01) : (s)._bool)
#define SWITCH_TEST_KEEP(s) ((s).valid && (s).keep)

#endif /* __SWITCH_T_H__ */
