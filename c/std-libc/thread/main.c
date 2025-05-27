#define _GNU_SOURCE

#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/resource.h>

#include <pthread.h>

#define print(fmt, ...) printf("[%u/%u] %lu " fmt, \
	getpid(), gettid(), pthread_self(), ##__VA_ARGS__)



void * thread_idle1 (void *arg) {
	struct rusage usage;
	int running = (long int)arg;
	print("task thread is running arg(%p:%d) ...\n", arg, running);

	while (running) {
		// print("task running %2d\n", running);
		// sleep(10);
		// running--;
	}

	getrusage(RUSAGE_SELF, &usage);
	print("task thread exit, time %d.%d secs "
		  "(usr %d.%d secs, sys %d.%d secs)\n",
		usage.ru_utime.tv_sec  + usage.ru_stime.tv_sec,
	 	usage.ru_utime.tv_usec + usage.ru_stime.tv_usec,
	 	usage.ru_utime.tv_sec, usage.ru_utime.tv_usec,
	 	usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);

	return (void *) -1;
}

struct thread_context {
	bool running;
} thread_ctx_t;

void * thread_idle2 (void *arg) {
	struct rusage usage;
	struct thread_context *ctx = (struct thread_context *)arg;
	print("task thread is running arg(%p:%p) ...\n", arg, ctx);

	while (ctx->running) {
		print("running task \n");
		sleep(1);
	}

	getrusage(RUSAGE_SELF, &usage);
	print("task thread exit, time %d.%d secs "
		  "(usr %d.%d secs, sys %d.%d secs)\n",
		usage.ru_utime.tv_sec  + usage.ru_stime.tv_sec,
		usage.ru_utime.tv_usec + usage.ru_stime.tv_usec,
		usage.ru_utime.tv_sec, usage.ru_utime.tv_usec,
		usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
	return NULL;
}

#define THREAD_POOLS 5

int main (int argc, const char *argv[]) {
	struct rusage usage;
	pthread_t threads[THREAD_POOLS];
	int timeout[THREAD_POOLS];

	print("main thread is running ...\n");

	for (int i = 0; i < THREAD_POOLS; i++) {
		void *arg = &timeout[i];
		if (0 != pthread_create(&threads[i], NULL, thread_idle1, (void *)3)) {
			print("crate thread is error!\n");
		}
		print("create thread (%lu)\n", threads[i]);
	}

	while (true) {
	}



	for (int i = 0; i < THREAD_POOLS; i++) {
		void *status;
		pthread_join(threads[i], &status);
		print("main thread wait %lu exit %d \n", threads[i], (long int)status);
	}

	sleep(10);

	getrusage(RUSAGE_SELF, &usage);
	print("main thread exit, time %d.%d secs "
		   "(usr %d.%d secs, sys %d.%d secs)\n",
		usage.ru_utime.tv_sec  + usage.ru_stime.tv_sec,
		usage.ru_utime.tv_usec + usage.ru_stime.tv_usec,
		usage.ru_utime.tv_sec, usage.ru_utime.tv_usec,
		usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
	return 0;
}
