#ifndef __HYDRA_THREAD_POOL_INCLUDED_H__
#define __HYDRA_THREAD_POOL_INCLUDED_H__

#include "thread.h"
#include "job_queue.h"

struct hydra_thread_node {
    hydra_thread_t *thread;
    struct hydra_thread_node *next;
};

typedef struct hydra_thread_node hydra_thread_node_t;

struct hydra_threads {
    hydra_thread_node_t *begin;
};

typedef struct hydra_threads hydra_threads_t;

struct hydra_pool_shared {
    pthread_mutex_t lock;
    hydra_job_queue_t *jobs;
};

typedef struct hydra_pool_shared hydra_pool_shared_t;

struct hydra_pool {
    hydra_threads_t *threads;
    unsigned short nthreads;
    short active_jobs;
    hydra_pool_shared_t *shared;
};

typedef struct hydra_pool hydra_pool_t;

int hydra_pool_init(hydra_pool_t *pool, unsigned short nthreads);
int hydra_pool_add_job(hydra_pool_t *pool, hydra_job_t *job);
int hydra_pool_destroy(hydra_pool_t *pool);
void hydra_pool_list_threads(hydra_pool_t *pool);

#endif // __HYDRA_THREAD_POOL_INCLUDED_H__
