#include <stdio.h>
#include <stdlib.h>
#include "job_queue.h"

hydra_job_queue_t *hydra_job_queue_init(void)
{
    hydra_job_queue_t *q = (hydra_job_queue_t *)malloc(sizeof(hydra_job_queue_t));
    q->begin = q->end = NULL;
    q->jobs = 0;

    return q;
}

int hydra_job_queue_add(hydra_job_queue_t *queue, hydra_job_node_t *job)
{
    if (queue->begin == NULL) {
        queue->begin = queue->end = job;
    } else {
        queue->end->next = job;
        queue->end = queue->end->next;
    }

    return 0;
}

hydra_job_t *hydra_job_queue_dispatch(hydra_job_queue_t *queue)
{
    hydra_job_node_t *n = queue->begin;
    hydra_job_t *j = NULL;

    if (n != NULL) {
        j = n->job;
        queue->begin = n->next;
        free(n);
    }

    return j;
}

int hydra_job_queue_destroy(hydra_job_queue_t *queue)
{
    if (queue != NULL) free(queue);
    return 0;
}
