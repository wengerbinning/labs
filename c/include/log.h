#ifndef __LOG_H__
#define __LOG_H__

#include <stdio.h>

#define ERROR_LEVEL_MASK    0xFF
#define ERROR_LEVEL_A       1
#define ERROR_LEVEL_B       2
#define ERROR_LEVEL_C       3

#define WARNNING_LEVEL_MASK 0xFF
#define WARNNING_LEVEL_A    1
#define WARNNING_LEVEL_B    2
#define WARNNING_LEVEL_C    3

#define INFO_LEVEL_MASK     0xFF
#define INFO_LEVEL_A        1
#define INFO_LEVEL_B        2
#define INFO_LEVEL_C        3

#if defined(DEBUG)
#define DEBUG_LEVEL_MASK    0xFF
#define DEBUG_LEVEL_A       1
#define DEBUG_LEVEL_B       2
#define DEBUG_LEVEL_C       
#endif /* DEBUG */

#define error(fmt, ...) \
    printf("[%02d ERROR] "fmt"\n",level, ##__VA_ARGS__)

#define warning(fmt, ...) \
    printf("[%02d WARNNING] "fmt"\n",level, ##__VA_ARGS__)

#define note(fmt, ...) \
    printf("[%02d INFO] "fmt"\n", level, ##__VA_ARGS__)

#if defined(DEBUG)
#define debug(fmt, ...) \
    printf("[%02d DEBUG] "fmt"\n",level, ##__VA_ARGS__)
#else
#define debug(fmt, ...) do {} while(0)
#endif /* DEBUG */

#define LOG_LEVEL_DEBUG
#define LOG_LEVEL_INFO
#define LOG_LEVEL_WARNNING
#define LOG_LEVEL_ERROR

#endif /* __LOG_H__ */