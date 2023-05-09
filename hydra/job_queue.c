#include <stdio.h>
#include <stdlib.h>
#include "job_queue.h"

#define TIME_TO_IDDLE 5 // seconds to change status to iddle

extern pthread_cond_t job_in_queue;
int delta = 0;

hydra_job_queue_t *hydra_job_queue_init(void)
{
    hydra_job_queue_t *q = (hydra_job_queue_t *)malloc(sizeof(hydra_job_queue_t));
    q->begin = NULL;
    q->end = NULL;
    q->jobs = 0;

    return q;
}

int hydra_job_queue_add(hydra_job_queue_t *queue, pthread_mutex_t lock, hydra_job_node_t *job)
{
    pthread_mutex_lock(&lock);
    if (queue->begin == NULL) {
        queue->begin = queue->end = job;
        
    } else {
        queue->end->next = job;
        queue->end = queue->end->next;
    }

    // there were no jobs in queue
    // if (!(*queue)->jobs) 
    // pthread_cond_signal(&job_in_queue);
    queue->jobs++;
    pthread_mutex_unlock(&lock);

    // printf("active jobs in queue: %d\n", queue->jobs);

    return 0;
}

hydra_job_t *hydra_job_queue_dispatch(hydra_job_queue_t *queue, pthread_mutex_t lock)
{
    pthread_mutex_lock(&lock);
    hydra_job_node_t *n = queue->begin;
    hydra_job_t *j = NULL;

    // printf("job status: %d\n", n->job->status);
    if (n != NULL) {
        j = n->job;
        queue->begin = n->next;
        queue->jobs--;
        free(n);
        // printf("job dispatched\n");
    }
    pthread_mutex_unlock(&lock);

    return j;
}

int hydra_job_queue_destroy(hydra_job_queue_t *queue)
{
    if (queue != NULL) free(queue);
    return 0;
}
