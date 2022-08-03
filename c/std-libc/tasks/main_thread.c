#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


typedef struct thread_context {
    unsigned int id;
    const char *name;

    size_t time;
} thread_context_t;



void *thread_func(void *priv) {
    thread_context_t *ctx = (thread_context_t *)priv; // Cast argument back to int
    printf("Thread%d: Starting work %lu %ds ...\n", ctx->id, (unsigned long)pthread_self(), ctx->time);

    if (ctx->time)
        sleep(ctx->time);

    printf("Thread%d: Work done!\n", ctx->id);
    return NULL;
}



#define THREAD_POOLIZE 5
int main(int agrc, char *argv[]) {
    int i;
    thread_context_t contexts[THREAD_POOLIZE];
    pthread_t threads[THREAD_POOLIZE];

    printf("Task: Start Threading Test Task %d ...\n", getpid());

    /* Create threads */
    for (i = 0; i < THREAD_POOLIZE; i++) {
        contexts[i].id = i;
        contexts[i].time = 10 * i + 10;
        if (pthread_create(&threads[i], NULL, thread_func, &contexts[i]) != 0) {
            perror("Error creating thread");
            exit(EXIT_FAILURE);
        }
        // printf("Main: Thread %d created.\n", i);
    }

    /* Wait for all threads to finish */
    for (i = 0; i < THREAD_POOLIZE; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("Error joining thread");
            exit(EXIT_FAILURE);
        }
        // printf("Thread%d joined.\n", i);
    }


    printf("Task: All threads completed.\n");
    return 0;
}