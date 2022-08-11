#ifndef _HYDRA_THREAD_POOL_INCLUDED_H_
#define _HYDRA_THREAD_POOL_INCLUDED_H_

#include "thread.h"
struct hydra_tpool {
    pthread_mutex_t lock;
    hydra_thread_t *threads;
    unsigned short nthreads;
    short active_jobs;
};

typedef struct hydra_tpool hydra_tpool_t;

int hydra_init_tpool(unsigned short nthreads);
int hydra_add_job();
int hydra_destroy_tpool();

#endif // _HYDRA_THREAD_POOL_INCLUDED_H_
