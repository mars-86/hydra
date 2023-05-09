#ifndef __HYDRA_SHARED_INCLUDED_H__
#define __HYDRA_SHARED_INCLUDED_H__

#include <pthread.h>
#include "job_queue.h"

struct hydra_pool_shared {
    pthread_mutex_t lock;
    hydra_job_queue_t *jobs;
};

typedef struct hydra_pool_shared hydra_pool_shared_t;

#endif // __HYDRA_SHARED_INCLUDED_H__
