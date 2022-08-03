int string_escape (const char *str, char chr, char *buffer, size_t bufsize)
{
    int cnt = 0;
    const char *p, *c;
    char *q;
    int len, offset = 0;

    if (!buffer)
        return -1;

    p = str;
    q = buffer;
    memset(buffer, 0, bufsize);

    while (( c = memchr(p, chr, (str + strlen(str) - p)) )) {
        cnt++;
        len = c - p;

        if (((bufsize - 1) - offset) <= 0) {
            *(buffer + (bufsize - 1)) = '\0';
            break;
        }

        if ((len) >= ((bufsize - 1) - offset)) {
            memcpy(q, p, ((bufsize - 1) - offset));
            *(buffer + (bufsize - 1)) = '\0';
            q += (bufsize - 1) - offset;
            offset = bufsize - 1;
            break;
        }

        if (len > 0)
            memcpy(q, p, len);

        if (((bufsize - 1) - offset - len) < 2) {
            *(q + len + 0) = '\0';
            q += 1;
            offset += 1;
            break;
        }

        *(q + len + 0) = '\\';
        *(q + len + 1) = chr;
        q += len + 2;
        offset += len + 2;

        p  = c + 1;
    }

    if (((str + strlen(str) - p) > 0) && (((buffer + bufsize - 1) - q) > 0)) {
        if ((str + strlen(str) - p) >= ((buffer + bufsize - 1) - q)) {
            memcpy(q, p, ((buffer + bufsize - 1) - q));
            *(buffer + bufsize - 1) = '\0';
        } else {
            memcpy(q, p, (str + strlen(str) - p));
            *(q + (str + strlen(str) - p)) = '\0';
        }
    }

    return cnt;
}


/* Iterate over substrings in a string based on delimiters.
 *  char sub[] - Save the substring.
 *  unsigned int len - Save the substring's actual length.
 *  char *p, *q - The pointers for traversing substrings.
 *  char *str - The string to br traversed.
 *  char *sep - The separator for traversing.
 */
/* Warning - sub must not be emoty. */
#define string_for_by_separator(sub, len, p, q, str, sep)	\
for (																		   \
	p = str, q = (p ? strstr(p, sep) : NULL),                                  \
	memset(sub, 0, sizeof(sub)),											   \
	len = (q ? (q - p) : (p ? strlen(p): 0)),			     	     		   \
	len = ((len >= sizeof(sub)) ? (sizeof(sub) - 1) : len),				       \
	memcpy(sub, p, len)											               \
	; p;                                                                       \
	p = (q ? (q + 1) : q), q = (p ? strstr(p, sep) : NULL),			           \
	memset(sub, 0, sizeof(sub)),							                   \
	len = (q ? (q - p) : (p ? strlen(p): 0)),   							   \
	len = ((len >= sizeof(sub)) ? (sizeof(sub) -1) : len),				       \
	memcpy(sub, p, len)														   \
)


extern int string_escape (const char *str, char chr, char *buffer, size_t bufsize);


