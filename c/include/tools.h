#ifndef __TOOL_H__
#define __TOOL_H__


#define CHAR(val, n) ((val &(0xFF << (8*n))) >> (8 * n)) 


int hexdump (void *ptr, int size);

#endif /* __TOOL_H__ */
