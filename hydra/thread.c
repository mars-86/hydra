#include "thread.h"
#include <stdio.h>

int hydra_create_thread(hydra_thread_t *thread)
{
    pthread_t tid;
    pthread_attr_t attr;
    void *(*routine)(void *) = NULL;
    void *args = NULL;

    if (thread) {
        if(thread->attr) {
            if (pthread_attr_init(&attr) != 0)
                return -1;
            if (thread->attr->state)
                pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
        }
        routine = thread->func;
        args = thread->args;
    }

    if (pthread_create(&tid, &attr, routine, args) != 0)
        return -1;

    // pthread_detach(tid);

    if (thread != NULL) {
        thread->id = tid;
        thread->is_alive = 1;
    }

    if (thread && thread->attr) pthread_attr_destroy(&attr);

    return 0;
}

int hydra_cancel_thread(pthread_t tid)
{
    if (pthread_cancel(tid) != 0) {
        fprintf(stderr, "Thread %d cannot be canceled\n", tid);
        return -1;
    }
    printf("Thread %d canceled\n", tid);
    return 0;
}
