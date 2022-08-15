#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "thread_pool.h"

void *hydra_handler(void *args)
{
    hydra_pool_shared_t *shared = (hydra_pool_shared_t *)args;
    hydra_job_queue_t *jobs = (hydra_job_queue_t *)shared->jobs;
    pthread_mutex_t *lock = &(shared->lock);
    hydra_job_t *job = NULL;
    printf("Hydra handler created in thread %d\n", pthread_self());
    while (1) {
        pthread_mutex_lock(lock);
        job = hydra_job_queue_dispatch(jobs);
        pthread_mutex_unlock(lock);

        if (job != NULL) {
            printf("In thread %d job with args %d\n", pthread_self(), (job != NULL) ? job->args : NULL);
            free(job), job = NULL;
        }
        Sleep(20);
    }

    printf("Thread %d destroyed\n", pthread_self());
    pthread_exit(NULL);
}
