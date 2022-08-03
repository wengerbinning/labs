#include <stdio.h>
#include <string.h>

#define debug_point(fmt, ...) \
	printf("%s:%d %s:%s\n", \
		__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

typedef int (check_name_t) (int idx, const char *name);

int func1 (const char *path, size_t size, size_t count, check_name_t *func) {
	int i, ret;
	const char *p, *q, s = '/';

	i = 0; p = path;
	while ((q = p) && (p = strchr(p, s))) {
		char name[size + 1];
		size_t m = p - q;
		size_t n = p - path;

		if (n == 0) {
			++p;

			// printf(";%d %d/%d:%s - %s\n", i, m, n, q, p);
			return -255;

			continue;
		}

		if (m <= size) {
			p++;
			snprintf(name, m + 1, "%s", q);
			if (0 == strcmp(name, "..")) {
				if (i <= 0)
					return -2;
				i--;
				continue;
			} else if (0 == strcmp(name, ".")) {
				continue;
			} else {
				i++;

				if (0 < count && count < i)
					return -3;
				if (func && (ret = func(i, name)) < 0)
					return ret;
				else if (!func)
					printf("%d %d/%d:%s - %s\n", i, m, n, name, p);
			}
		} else {
			return -1;
		}
	}
	if ((q) && strlen(q)) {
		char name[size + 1];
		size_t m = strlen(q);
		size_t n = q - path;

		if (m <= size) {
			snprintf(name, m + 1, "%s", q);
			if (0 == strcmp(name, "..")) {
				if (i <= 0)
					return -2;
				i--;
			} else if (0 == strcmp(name, ".")) {
				return -254;
			} else {
				i++;
			}
			if (0 < count && count < i)
				return -3;
			if (func && (ret = func(i, name)) < 0)
				return ret;
			else if (!func)
				printf("%d %d/%d:%s\n", i, m, n, name);

		} else {
			return -1;
		}
	}

	return i;
}



int func2 (int idx, const char *name)
{
	printf("%d - %s\n", idx, name);



	if ( <= c)


	return 0;
}




int main (int argc, char *argv[]) {
	int ret;

	// if (argc < 2)
	// 	return -1;

	ret = func1(argv[1], 32, 0, func2);
	printf("# return %d\n", ret);
	debug_point("running in here");
	return 0;
}