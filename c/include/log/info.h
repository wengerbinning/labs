#ifndef __INFO_H__
#define __INFO_H__

#define INFO_LEVEL_MASK     0xFF
#define INFO_LEVEL_A        1
#define INFO_LEVEL_B        2
#define INFO_LEVEL_C        3

#define info(level, fmt, ...) \
    printf("[%02d INFO] "fmt"\n", level, ##__VA_ARGS__)

#endif /* __INFO_H__ */
