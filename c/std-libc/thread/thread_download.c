#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>
#include <curl/curl.h>

#include "develop.h"
#include "download.h"
#include "buffer.h"

static size_t curl_func_write_buffer (void *ptr, size_t size, size_t nmemb, void *data)
{
	buffer_t *buf = data;
	block_t *blk;

	devel_debug("write %d(= %d * %d) from %p, to %p", (size * nmemb), size, nmemb, ptr, data);

	devel_debug("current block %p, size: %d", buf->blk, buf->blk->size);

	if (buf->blk->size <= 0 && buf->blk->list.next) {
		// blk =
		devel_debug("block %p is full, use next ...", buf->blk);
		if (list_is_last(&buf->blk->list, &buf->blk_list)) {
			devel_debug("block %p is last, start first!", buf->blk);
			buf->blk = list_first_entry(&buf->blk_list, block_t, list);
			buf->blk->size = BUFIZE;
			return 0;
		} else {
			buf->blk = list_entry(buf->blk->list.next, block_t, list);
			buf->blk->size = BUFIZE;
		}
	} else if (buf->blk->size <= 0) {
		devel_debug("buffer all full current block size %d", buf->blk->size);
		return 0;
	}

	blk = buf->blk;

	if (blk->size >= (size * nmemb)) {
		memcpy(blk->data, ptr, (size * nmemb));
		blk->size -= (size * nmemb);
		buf->size += (size * nmemb);
		return (size * nmemb);
	} else if (blk->size > 0) {
		memcpy(blk->data, ptr, blk->size);
		blk->size -= blk->size;
		buf->size += blk->size;
		return buf->size;
	} else {
		devel_error("curl write Error");
		return 0;
	}


	return nmemb;
}

static size_t curl_func_write_file (void *ptr, size_t size, size_t nmemb, void *data)
{
	FILE *file = data;
	block_t *blk;

	devel_debug("write %d(= %d * %d) from %p, to %p", (size * nmemb), size, nmemb, ptr, data);

	devel_debug("current block %p, size: %d", buf->blk, buf->blk->size);

	if (buf->blk->size <= 0 && buf->blk->list.next) {
		// blk =
		devel_debug("block %p is full, use next ...", buf->blk);
		if (list_is_last(&buf->blk->list, &buf->blk_list)) {
			devel_debug("block %p is last, start first!", buf->blk);
			buf->blk = list_first_entry(&buf->blk_list, block_t, list);
			buf->blk->size = BUFIZE;
			return 0;
		} else {
			buf->blk = list_entry(buf->blk->list.next, block_t, list);
			buf->blk->size = BUFIZE;
		}
	} else if (buf->blk->size <= 0) {
		devel_debug("buffer all full current block size %d", buf->blk->size);
		return 0;
	}

	blk = buf->blk;

	if (blk->size >= (size * nmemb)) {
		memcpy(blk->data, ptr, (size * nmemb));
		blk->size -= (size * nmemb);
		buf->size += (size * nmemb);
		return (size * nmemb);
	} else if (blk->size > 0) {
		memcpy(blk->data, ptr, blk->size);
		blk->size -= blk->size;
		buf->size += blk->size;
		return buf->size;
	} else {
		devel_error("curl write Error");
		return 0;
	}


	return nmemb;
}


void *thread_task (void *arg) {
	download_t *download;
	url_t *url;
	long code;
	int ret;
	buffer_t *buf;

	download = (download_t *)arg;

	// devel_info("Now is thread ID: %lu", pthread_self());

	while (true) {
		pthread_mutex_lock(&download->mutex);
		devel_debug("running in here ...");
		// devel_debug("download   URL:%s", download->url ? download->url->url : "null");
		// devel_debug("download Flags:%08X", download->flags);
		url = download->url;
		buf = download->buffer;
		pthread_mutex_unlock(&download->mutex);

		sleep(3);

		if (!url)
			continue;

		buf->size = 0;

		if (list_empty(&buf->blk_list))
			devel_debug("buffer list if NULL");

		buf->blk = list_first_entry(&buf->blk_list, block_t, list);

		/* */
		CURL *curl = curl_easy_init();

		curl_easy_setopt(curl, CURLOPT_URL, url->url);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
		curl_easy_setopt(curl, CURLOPT_RANGE, "1-100");
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, download->buffer);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_func_write_buffer);

		ret = curl_easy_perform(curl);
		switch (ret) {
			case CURLE_OK:
				break;
			case CURLE_WRITE_ERROR:
				devel_error("curl request write error (code: %d)!", ret);
				break;
			default:
			devel_error("curl request return code: %d", ret);
		}

		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
		switch (code) {
			case 200:
			case 206:
				url->flags |= URLF_VALID;
				break;
			default:
				devel_error("response code: %d", code);
		}

		devel_info("buffer: %p, size: %d", download->buffer, download->buffer->size);
	}


	return NULL;
}
