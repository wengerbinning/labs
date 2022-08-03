#ifndef __DEVEL_H__
#define __DEVEL_H__

#include <stdio.h>

#define error(id, fmt, ...)      printf("(code:%d) "fmt"\n", id, ## __VA_ARGS__)
#define warning(id, fmt, ...)    printf(fmt"\n", ## __VA_ARGS__)
#define notice(id, fmt, ...)     printf(fmt"\n", ## __VA_ARGS__)
#define note(fmt, ...)           printf(fmt,     ## __VA_ARGS__)
#define debug(fmt, ...)          printf(fmt"\n", ## __VA_ARGS__)

#include "hexdump.h"

#endif /* __DEVEL_H__ */