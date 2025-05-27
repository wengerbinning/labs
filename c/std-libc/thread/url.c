#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>

#include "develop.h"
#include "url.h"




static size_t curl_func_write_dummy (void *ptr,
	size_t size, size_t nmemb, void *stream)
{
	return nmemb;
}

int url_check (url_t *url) {
	int ret = 0;
	long code;
	int buf[512];

	if (!url->url)
		return -1;

	if (0 == strncmp(url->url, "http://", 7)) {
		url->flags |= URLF_PROTO | URLF_PROTO_HTTP;
	} else if (0 == strncmp(url->url, "https://", 8)) {
		url->flags |= URLF_PROTO | URLF_PROTO_HTTPS;
	} else if (0 == strncmp(url->url, "ftp://", 6)) {
		url->flags |= URLF_PROTO | URLF_PROTO_FTP;
	} else if (0 == strncmp(url->url, "tftp://", 7)) {
		url->flags |= URLF_PROTO | URLF_PROTO_TFTP;
	} else if (0 == strncmp(url->url, "file://", 7)) {
		url->flags |= URLF_PROTO | URLF_PROTO_FILE;
	} else {
		devel_error("Unknown Protocol\n");
		return -1;
	}

	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url->url);

	// curl_easy_setopt(curl, CURLOPT_PROXY, "127.0.0.1:1080");
	// curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/72.0.3626.121 Safari/537.36");


	curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
	curl_easy_setopt(curl, CURLOPT_RANGE, "0-2048");
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_func_write_dummy);

	if ((ret = curl_easy_perform(curl)) != CURLE_OK) {
		printf("curl request return code: %d\n", ret);
	}


	curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);

	switch (code) {
		case 200:
		case 206:
			url->flags |= URLF_VALID;
			break;
		default:
			printf("response code: %d\n", code);
			ret = 1;
	}

	curl_easy_cleanup(curl);

	return ret;
}

url_t *url_new (char *url) {
	url_t *new_url;

	new_url = malloc(sizeof(*new_url));
	memset(new_url, 0, sizeof(*new_url));

	new_url->url = strdup(url);

	return new_url;
}

void url_free (struct url_t * url) {
	struct url_t *p=url;

	if (!url)
		return;

	if (url->url)
		free(url->url);

	free(url);
}