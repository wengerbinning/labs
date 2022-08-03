#ifndef __WARNING_H__
#define __WARNING_H__


#define warnning(level, fmt, ...) \
    printf("[%02d WARNNING] "fmt"\n",level, ##__VA_ARGS__)

#endif /* __WARNING_H__ */
