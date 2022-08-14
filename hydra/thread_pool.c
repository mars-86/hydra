#include "thread_pool.h"
#include <stdio.h>
#include <stdlib.h>

extern void *hydra_handler(void *args);

hydra_thread_node_t *hydra_create_tnode(hydra_thread_t *thread);
hydra_thread_t *hydra_alloc_thread(void *args);

int hydra_add_thread(hydra_threads_t *threads, hydra_thread_t *thread);
int hydra_remove_last_thread(hydra_threads_t *threads);
int hydra_remove_thread(hydra_threads_t *threads, pthread_t tid);

int hydra_init_tpool(hydra_pool_t *pool, unsigned short nthreads)
{
    int i;

    pool->threads = (hydra_threads_t *)malloc(sizeof(hydra_threads_t));
    pool->threads->begin = NULL;
    pool->nthreads = nthreads;
    pool->lock = PTHREAD_MUTEX_INITIALIZER;
    pool->active_jobs = 0;

    for (i = 0; i < pool->nthreads; ++i) {
        hydra_thread_t *t = hydra_alloc_thread((void *)i);
        hydra_create_thread(t);
        hydra_add_thread(pool->threads, t);
    }

    return 0;
}

int hydra_add_job()
{

}

int hydra_destroy_tpool(hydra_pool_t *pool)
{
    while (pool->nthreads > 0)
    {
        hydra_remove_last_thread(pool->threads);
        pool->nthreads--;
    }
    
    if (pool->threads != NULL) free(pool->threads);
    
    return 0;
}

hydra_thread_node_t *hydra_create_tnode(hydra_thread_t *thread)
{
    hydra_thread_node_t *tnode;
    // TODO: add malloc validations
    tnode = (hydra_thread_node_t *)malloc(sizeof(hydra_thread_node_t));
    tnode->thread = thread;
    tnode->next = NULL;

    return tnode;
}

int hydra_add_thread(hydra_threads_t *threads, hydra_thread_t *thread)
{
    hydra_thread_node_t *tp, *tnode = hydra_create_tnode(thread);
    if (threads->begin == NULL) {
        threads->begin = tnode;
    } else {
        for (tp = threads->begin; tp != NULL; tp = tp->next) {
            if (tp->next == NULL) {
                tp->next = tnode;
                break;
            }
        }
    }
    printf("Thread %d added\n", tnode->thread->id);

    return 0;
}

int hydra_remove_last_thread(hydra_threads_t *threads)
{
    hydra_thread_node_t *tpb = threads->begin, *tp = threads->begin;
    if (tp == NULL) return 0;
    for (tp = (tp->next == NULL) ? tp : tp->next; tp != NULL; tp = tp->next, tpb = tpb->next)
        if (tp->next == NULL)
            break;

    printf("Removing thread %d from active threads\n", tp->thread->id);
    // TODO: Handle detached state
    hydra_cancel_thread(tp->thread->id);
    free(tp->thread), tp->thread = NULL;
    tpb->next = NULL, free(tp), tp = NULL;

    return 0;
}

int hydra_remove_thread(hydra_threads_t *threads, pthread_t tid)
{
    hydra_thread_node_t *tp;
    for (tp = threads->begin; tp != NULL; tp = tp->next) {
        if (tp->thread->id == tid) {
            break;
        }
    }

    return 0;
}

void hydra_list_threads(hydra_pool_t *pool)
{
    hydra_thread_node_t *tp;
    for (tp = pool->threads->begin; tp != NULL; tp = tp->next)
        printf("Thread id %d\n", tp->thread->id);
}

hydra_thread_t *hydra_alloc_thread(void *args)
{
    hydra_thread_t *t = (hydra_thread_t *)malloc(sizeof(hydra_thread_t));
    t->attr = NULL, t->func = &hydra_handler;
    t->args = args;

    return t;
}
