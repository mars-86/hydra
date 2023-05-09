#ifndef __HYDRA_THREAD_INCLUDED_H__
#define __HYDRA_THREAD_INCLUDED_H__

#include <pthread.h>
struct hydra_thread_attr {
    int state;
};

typedef struct hydra_thread_attr hydra_thread_attr_t;

struct hydra_thread {
    pthread_t id;
    hydra_thread_attr_t *attr;
    void *(*routine)(void *);
    void *args;
    int is_alive;
};

typedef struct hydra_thread hydra_thread_t;

int hydra_thread_create(hydra_thread_t *t);
int hydra_thread_cancel(pthread_t tid);

#endif // __HYDRA_THREAD_INCLUDED_H__
