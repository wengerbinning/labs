#ifndef __DOWNLOAD_H__
#define __DOWNLOAD_H__

#include <stdbool.h>

#include "buffer.h"
#include "url.h"


#define DOWNF_FILE      (0x1 << 1)
#define DOWNF_BUFFER    (0x1 << 2)



typedef struct download_t {
	pthread_mutex_t mutex;
	bool enable;
	url_t *url;
	unsigned int flags;
	union {
		FILE *file;
		buffer_t *buffer;
	};
} download_t;

#endif /* __DOWNLOAD_H__ */