#ifndef _HYDRA_THREAD_POOL_INCLUDED_H_
#define _HYDRA_THREAD_POOL_INCLUDED_H_

#include "thread.h"

struct hydra_thread_node {
    hydra_thread_t *thread;
    struct hydra_thread_node *next;
};

typedef struct hydra_thread_node hydra_thread_node_t;

struct hydra_threads {
    hydra_thread_node_t *begin;
};

typedef struct hydra_threads hydra_threads_t;

struct hydra_pool {
    pthread_mutex_t lock;
    hydra_threads_t *threads;
    unsigned short nthreads;
    short active_jobs;
};

typedef struct hydra_pool hydra_pool_t;

int hydra_init_tpool(hydra_pool_t *pool, unsigned short nthreads);
int hydra_add_job();
int hydra_destroy_tpool(hydra_pool_t *pool);
void hydra_list_threads(hydra_pool_t *pool);

#endif // _HYDRA_THREAD_POOL_INCLUDED_H_
