// POSIX

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <getopt.h>

#include <sys/stat.h>

#include "version.h"

#define PATH_MAX 4096


#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_PHASE 0
#define VERSION_PATCH 0

#define VERSION_CODE \
	VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_PHASE, VERSION_PATCH)
#define DESCRIPTIONS " \
This is a scan files tools. 												\n\
 scan [options].. <path>													\n\
																			\n\
OPTIONS:																	\n\
	--help		Display this command description							\n\
	--version	Display this command version								\n\
																			\n\
"

typedef unsigned int file_attr_t;
typedef mode_t       file_mode_t;
typedef unsigned int number_t;
typedef struct scan_request {
	number_t deep;
	number_t max_deep;
	number_t limit;
	const char *home, *path;
} scan_req_t;

int func (scan_req_t *req);

static char *soptions = "12hvVd:r";
static struct option loptions[] = {
	{"help",    no_argument,       0, 'h'},
	{"debug",   no_argument,       0,  0 },
	{"verbose", no_argument,       0, 'v'},
	{"version", no_argument,       0, 'V'},
	{0, 0, 0, 0}
};





/*

	* 简单参数: argc, argv
	* POSIX标准短参数: optarg, optind, opterr, optopt
	* POSIX标准长参数: optarg, optind, opterr, optopt
	* 自定义解析参数: argc, argv



	* 解析选项参数
	* 解析位置参数

	POSIX - 标准
	-

 */

// int getopt (int argc, char * const argv[], const char *optstring);
// int getopt_long (int argc, char * const argv[], const char *optstring, const struct option *longopts, int *longindex);

int main (int argc, char *argv[]) {
	int o, idx;
	char *path = ".";
	scan_req_t _req = { 0 };
	scan_req_t *req = &_req;
	while (0 <= (o = getopt_long(argc, argv, soptions, loptions, &idx))) {
		switch (o) {
		case 0x0:
			// todo: parse only long
			break;
		case 'a':
			break;
		case 'A':
			break;
		case 'h':
			printf("%s\n", DESCRIPTIONS);
			return 0;
		case 'V':
			printf(VERFMT0 "\n", VER2STR(VERSION_CODE));
			return 0;
		case '?':
			// todo：invalid param
			break;
		default:
			// todo: default action
		}
	}
	if (argc <= optind) {
		printf("%s\n", DESCRIPTIONS);
		return -1;
	}
	while (optind < argc) {
		path = argv[optind++];
		// printf("[%02d] %s\n", optind, argv[optind++]);
	}

	req->max_deep = 2;
	req->limit = 1024;
	req->path = path;
	int ret = func(req);

	printf("count: %d\n", ret);
	return 0;
}

#define HIDDEN    0
#define ACCESS    1
#define LINK      2
#define DIRECTORY 3

int func (scan_req_t *req) {
	DIR *dir;
	size_t count = 0;
	char path[PATH_MAX + 1] = { 0 };
	struct dirent *ent;
	const char *home;

	home = req->path;
	if (req->max_deep && req->max_deep <= req->deep) {
		return 0;
	}
	if ((dir = opendir(req->path)) == NULL) {
		return -1;
	}
	while ((ent = readdir(dir)) != NULL) {
		char buffer[PATH_MAX + 1] = { 0 };
		file_attr_t attr = 0;
		file_mode_t mode = 0;
		struct stat st;
		size_t len;

		if (ent->d_name[0] ==  '.' && (len = strlen(ent->d_name))) {
			if (len == 1)
				continue;
			if (len == 2 && ent->d_name[1] ==  '.')
				continue;
			attr |= BIT(HIDDEN);
		}
		if (home[strlen(home) - 1] == '/')
			snprintf( path, PATH_MAX, "%s%s",  home, ent->d_name );
		else
			snprintf( path, PATH_MAX, "%s/%s", home, ent->d_name );

		if (0 <= lstat(path, &st)) {
			attr |= BIT(ACCESS);


			if (S_ISLNK(st.st_mode)) {
				attr |= BIT(LINK);
				if (0 < (len = readlink(path, buffer, PATH_MAX))) {
					buffer[len] = 0x0;
				}
			}
			if (S_ISDIR(st.st_mode)) {
				attr |= BIT(DIRECTORY);
			}

			if (!(attr & BIT(LINK)) && (attr & BIT(DIRECTORY))) {
				if (!req->limit || count < req->limit) {
					int ret;
					req->deep ++;
					req->path = path;
					ret = func(req);
					req->path = home;
					req->deep --;
					if (0 < ret) {
						count += ret;
						continue;
					}
				}
			}
			mode = st.st_mode;
		}



		printf("%04u %08x %u %u %u %u %3o", req->deep, attr,
			((attr & BIT(HIDDEN)) ? 1 : 0), ((attr & BIT(ACCESS)) ? 1 : 0),
			((attr & BIT(LINK)) ? 1 : 0),   ((attr & BIT(DIRECTORY)) ? 1 : 0),
			mode & 0777);

		printf(" %s", path);
		if (attr & BIT(LINK)) {
			printf(" -> %s", buffer);
		}
		printf("\n");
		count ++;
	}

	return count;
}
