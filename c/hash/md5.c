#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define ERROR(level, fmt, ...) \
	do { \
		printf("[ERROR] %s(%d): "fmt"\n", __FUNCTION__, __LINE__, ## __VA_ARGS__); \
	} while(0)

#define DEBUG(level, fmt, ...) \
	do { \
		printf("[DEBUG] %s(%d): "fmt"\n", __FUNCTION__, __LINE__, ## __VA_ARGS__); \
	} while(0)

static inline void *md5_malloc (int size) {
	void *p;
	
	p = malloc(size);
	if (!p)
		return NULL;

	return p;
}


#define MALLOC(size) md5_malloc(size)


static inline int hexdump(const char *data, unsigned int size) {
	int i, addr = 0, line = 16;
	
	printf("%04X: ", addr);
	for (i = 0; i < size; i++) {

		if ( i != 0 && i % line == 0)
			printf("\n%04X: ", addr);
	
		printf("%02X ", *(unsigned char *)(data+i));
		addr += 1;
	}

	printf("\n");
}

#define HEXDUMP(data, size) hexdump(data, size)


unsigned char *md5_padding (const unsigned char *data, unsigned long *size) {
	long len, padding; 
	int x = *size % 64;
	unsigned char *pdata; 
	unsigned long *p;

	if (x < 56) 
		padding = 56 - x;
	else if (x == 56) 
		padding = 64;
	else
		padding = 56 + (64 - x);
	 
	len = *size + padding + 8;
	pdata = MALLOC(len);
	if (!pdata) {
		ERROR(0, "malloc is failed");
		return NULL;
	}

	memcpy(pdata, data, *size);
	if (x == 56)
		pdata[*size]= 0x80U;

	p = (unsigned long *)(pdata + *size + padding);
	*p = *size;
	
	*size = len;
	
	return pdata; 
}






int main(int argc, char *argv[]) {
	unsigned char *pdata;
	char str[] = "ABCDEFGHIGKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz012";
	unsigned long len =(unsigned long)sizeof(str);

	DEBUG(0, "str -> %s, size %d", str, sizeof(str));
	
	HEXDUMP(str, sizeof(str));
	
	pdata = md5_padding(str, &len);
	if (!pdata) {
		ERROR(0, "Padding error");
		return -1;
	}	
	DEBUG(0, "Padding after");
	HEXDUMP(pdata, len);
}
