#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <libubox/ulog.h>

#include "worker.h"

#define WORK_PID "/var/run/worker.pid"
#define BUFIZE 64
#define PATHIZE 64


#define note(fmt, ...)         printf(fmt"\n", ## __VA_ARGS__)

#define log_note(fmt, ...)     printf(fmt"\n", ## __VA_ARGS__)

enum work_option {
	OPT_HELP,
	OPT_VERISON,
	OPT_FORCE,
	OPT_VERBOSE,
	OPT_TIMEOUT,
	OPT_NULL,
	__OPT_MAX
};

typedef struct work_t {
	pid_t pid, ppid;
	char pcomm[BUFIZE + 1];
	bool force;
	bool verbose;
	unsigned int opt_flags;
	unsigned int timeout;
} work_t;

bool work_lock (bool force) {
	int fd, pid;
	char buffer[BUFIZE + 1];

	if ((access(WORK_PID, F_OK) >= 0) && !force) {
		if ((fd = open(WORK_PID, S_IRUSR)) < 0)
			return false;

		memset(buffer, 0, sizeof(buffer));
		read(fd, buffer, BUFIZE);
		sscanf(buffer, "%d", &pid);

		if (pid == getpid())
			return true;

		return false;
	}

	if ((fd = creat(WORK_PID, S_IRUSR | S_IWUSR)) < 0)
		return false;

	memset(buffer, 0, sizeof(buffer));
	snprintf(buffer, BUFIZE, "%d", getpid());
	write(fd, buffer, sizeof(buffer));

	close(fd);

	return true;
}

void work_unlock (bool force) {

	int fd, pid;
	char buffer[BUFIZE + 1];

	if (access(WORK_PID, F_OK) < 0)
		return;

	if (((fd = open(WORK_PID, S_IRUSR)) < 0) && !force)
		return;

	memset(buffer, 0, sizeof(buffer));
	read(fd, buffer, BUFIZE);
	sscanf(buffer, "%d", &pid);

	if ((pid != getpid()) && !force)
		return;

	remove(WORK_PID);
}

static char soptions[] = "ht:xvV";
static struct option loptions[__OPT_MAX] = {
	[OPT_HELP]     = {"help",          no_argument, 0, 'h'},
	[OPT_VERISON]  = {"version",       no_argument, 0, 'V'},

	[OPT_FORCE]    = {"force",         no_argument, 0, 'x'},
	[OPT_VERBOSE]  = {"verbose",       no_argument, 0, 'v'},
	[OPT_TIMEOUT]  = {"timeout", required_argument, 0, 't'},

	[OPT_NULL]     = {0, 0, 0, 0}
};

static inline void parse_long_options (const struct option *opt, int idx, work_t *work) {
	char buffer[BUFIZE + 1];

	switch (idx) {
	case OPT_HELP:
		break;
	case OPT_VERISON:
		break;
	case OPT_FORCE:
		work->force = true;
		break;
	case OPT_VERBOSE:
		work->verbose = true;
		break;
	case OPT_TIMEOUT:
		work->timeout = atoi(optarg);
		break;
	default:
	}
}

static work_t work;

void work_init (work_t * work) {
	int fd;
	char path[PATHIZE + 1];
	char *p;

	/** */
	work->pid = getpid();
	work->ppid = getppid();

	memset(path, 0, sizeof(path));
	snprintf(paif ((fd = open(path, O_RDONLY)) > 0) {
		read(fd, work->pcomm, BUFIZE);
		if((p = strstr(work->pcomm, "\n")))
			*p = '\0';

		close(fd);
	}th, PATHIZE, "/proc/%d/comm", work->ppid);


	/** */
}

int main (int argc, char *argv[]) {
	int opt, idx;

	work_init(&work);

	while ((opt = getopt_long(argc, argv, soptions, loptions, &idx)) != -1) {
		switch (opt) {
		case 0:
			parse_long_options(loptions, idx, &work);
			break;
		case 'h':
			break;
		case 't':
			work.timeout = atoi(optarg);
			break;
		case 'x':
			work.force = true;
			break;
		case 'v':
			work.verbose = true;
			break;
		case 'V':
			break;
		case '?': /* Handle unknown valid option. */
		default:
		}
	}

	note("worker %d was call by %d(%s)", work.pid, work.ppid, work.pcomm);
	if (!work_lock(work.force)) {
		return -WORKE_BUSY;
	}

	note("Worker start running ...");

	sleep(work.timeout ? work.timeout : 1);

	note("Worker finished.");

	work_unlock(work.force);
	return 0;
}