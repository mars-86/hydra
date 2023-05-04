#ifndef _HYDRA_THREAD_INCLUDED_H_
#define _HYDRA_THREAD_INCLUDED_H_

#include <pthread.h>

struct hydra_thread_attr {
    int state;
};

typedef struct hydra_thread_attr hydra_thread_attr_t;

struct hydra_thread {
    pthread_t id;
    hydra_thread_attr_t *attr;
    void *(*func)(void *);
    void *args;
    int is_alive;
};

typedef struct hydra_thread hydra_thread_t;

int hydra_thread_create(hydra_thread_t *t);
int hydra_thread_cancel(pthread_t tid);

#endif // _HYDRA_THREAD_INCLUDED_H_
