#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>

#define DEBUG_LEVEL_MASK    0xFF
#define DEBUG_LEVEL_A       1
#define DEBUG_LEVEL_B       2
#define DEBUG_LEVEL_C       3

#if defined(DEBUG)
#define debug(level, fmt, ...) \
    printf("[%02d DEBUG] "fmt"\n",level, ##__VA_ARGS__)
#else
#define debug(level, fmt, ...) do {} while(0)
#endif /* DEBUG */

#endif /* __DEBUG_H__ */

