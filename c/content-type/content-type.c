#include <stdio.h>
#include <regex.h>
#include <string.h>


#include "mime.h"

#define STRCMP(s1, s2) (0 == strncmp(s1, s2, strlen(s2)))

static const char *content_type_name[CONTENT_TYPE_MAX] = {
[CONTENT_TYPE_DEFAULT]  = "default",
[MULTIPART_FORM_DATA]   = "multipart/form-data",

[TEXT_PLAIN]            = "text/plain",
[TEXT_CSS]              = "text/css",
[TEXT_JAVASCRIPT]       = "text/javascript",

[APPLICATION_OCTET_STREAM] = "application/octet-stream",
[APPLICATION_ZIP]          = "application/zip",
[APPLICATION_X_GZIP]       = "application/x-gzip",
};

static inline int content_type_check(char *name) {
int i;
if (!name)
return CONTENT_TYPE_DEFAULT;

for (i = 0; i < CONTENT_TYPE_MAX; i++) {
if (!content_type_name[i])
    continue;
if (STRCMP(name, content_type_name[i]))
    return i;
}
return CONTENT_TYPE_DEFAULT;
}

#define CONTENT_TYPE_CHECK(name) content_type_check(name)

int content_type_parse (char *buf, char **_charset, char **_boundary) {
int content;
char *charset, *boundary;

content = CONTENT_TYPE_CHECK(buf);
if (buf && (buf = strstr(buf, ";"))) {
memset(buf, 0, 1);
buf += 1;
} else {
return content;
}
charset = NULL, boundary = NULL;
while(buf && strlen(buf)) {
char *key, *val;
regex_t regex;
size_t nmatch = 3;
regmatch_t pmatch[3];
regcomp(&regex, "[\\s]*(charset|boundary)=([^;\\s]*)", REG_EXTENDED);
if (0 == regexec(&regex, buf, nmatch, pmatch, 0)) {
    key = buf + pmatch[1].rm_so; memset(buf + pmatch[1].rm_eo, 0, 1);
    val = buf + pmatch[2].rm_so; memset(buf + pmatch[2].rm_eo, 0, 1);
    buf += pmatch[2].rm_eo + 1;
    if (STRCMP(key, "charset"))
        charset = val;
    else if (STRCMP(key, "boundary"))
        boundary = val;
} else {
    buf += strlen(buf);
}
regfree(&regex);
}

if (_charset)
*_charset = charset;
if (_boundary)
*_boundary = boundary;

return content;
}