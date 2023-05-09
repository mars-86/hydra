#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "pool.h"

pthread_cond_t job_in_queue = PTHREAD_COND_INITIALIZER;

void *hydra_handler(void *args)
{
    hydra_pool_shared_t *shared = (hydra_pool_shared_t *)args;
    hydra_job_queue_t *jobs = (hydra_job_queue_t *)shared->jobs;
    pthread_mutex_t *lock = &(shared->lock);
    hydra_job_t *job = NULL;
#ifdef __DEBUG
    printf("Hydra handler created in thread %d\n", pthread_self());
#endif
    while (1) {
        job = hydra_job_queue_dispatch(jobs, lock);

        if (job != NULL) {
#ifdef __DEBUG
            printf("In thread %d job with args %d\n", pthread_self(), job->args);
#endif    
            job->routine(job->args);
            free(job), job = NULL;
            // continue;
        }
        // pthread_cond_wait(&job_in_queue, lock);
        Sleep(1);
    }

#ifdef __DEBUG
    printf("Thread %d destroyed\n", pthread_self());
#endif
    pthread_exit(NULL);
}
