内存管理




####

#### stdlib.h

* `void * malloc (size_t __size)`
* `void * calloc (size_t __nmemb, size_t __size)`
* `void * realloc (void *__ptr, size_t __size)`
* `void free (void *__ptr)`

#### string.h

* `void * memcpy (void *__dest, const void *__src, size_t __n)`
* `void * memmove (void *__dest, const void *__src, size_t __n)`
* `void * memset (void *__s, int __c, size_t __n)`
* `void * rawmemchr (const void *__s, int __c)`
* `void * memchr (void *__s, int __c, size_t __n)`
* `void * memrchr (const void *__s, int __c, size_t __n)`
* `int memcmp (const void *__s1, const void *__s2, size_t __n)`

