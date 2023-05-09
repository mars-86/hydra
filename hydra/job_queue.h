#ifndef __HYDRA_JOB_QUEUE_H_INCLUDED__
#define __HYDRA_JOB_QUEUE_H_INCLUDED__

#include <pthread.h>
#include "job.h"

struct hydra_job_node {
    hydra_job_t *job;
    struct hydra_job_node *next;
};

typedef struct hydra_job_node hydra_job_node_t; 

struct hydra_job_queue {
    hydra_job_node_t *begin;
    hydra_job_node_t *end;
    int jobs;
};

typedef struct hydra_job_queue hydra_job_queue_t;

hydra_job_queue_t *hydra_job_queue_init(void);
int hydra_job_queue_add(hydra_job_queue_t *queue, pthread_mutex_t lock, hydra_job_node_t *job);
hydra_job_t *hydra_job_queue_dispatch(hydra_job_queue_t *queue, pthread_mutex_t lock);
int hydra_job_queue_destroy(hydra_job_queue_t *queue);

#endif // __HYDRA_JOB_QUEUE_H_INCLUDED__
