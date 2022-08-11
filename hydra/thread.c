#include "thread.h"

int hydra_create_thread(hydra_thread_t *t)
{
    pthread_attr_t attr;
    if (t->attr) {
        if (pthread_attr_init(&attr) != 0)
            return -1;
        if (t->attr->state)
            pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    }
    if (pthread_create(&t->id, &attr, t->func, t->args) != 0)
        return -1;

    if (t->attr) pthread_attr_destroy(&attr);

    return 0;
}

int hydra_cancel_thread(pthread_t tid)
{
    pthread_cancel(tid);
}
