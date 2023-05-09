#include <stdio.h>
#include <stdlib.h>
#include "pool.h"

extern void *hydra_handler(void *args);
extern hydra_job_queue_t *hydra_job_queue_init(void);

static hydra_thread_node_t *hydra_create_tnode(hydra_thread_t *thread);
static hydra_job_node_t *hydra_create_job_node(hydra_job_t *job);
static hydra_thread_t *hydra_pool_alloc_thread(void *args);

static int hydra_pool_add_thread(hydra_threads_t *threads, hydra_thread_t *thread);
static int hydra_pool_remove_last_thread(hydra_threads_t *threads);
static int hydra_pool_remove_thread(hydra_threads_t *threads, pthread_t tid);

int hydra_pool_init(hydra_pool_t *pool, unsigned short nthreads)
{
    int i;

    pool->threads = (hydra_threads_t *)malloc(sizeof(hydra_threads_t));
    pool->shared = (hydra_pool_shared_t *)malloc(sizeof(hydra_pool_shared_t));
    pool->threads->begin = NULL;
    pool->nthreads = nthreads;
    pthread_mutex_init(&(pool->shared->lock), NULL); // PTHREAD_MUTEX_INITIALIZER;
    pool->shared->jobs = hydra_job_queue_init();
    pool->active_jobs = 0;

    pool->status = CREATED;
    
    for (i = 0; i < pool->nthreads; ++i) {
        hydra_thread_t *t = hydra_pool_alloc_thread(pool->shared);
        hydra_thread_create(t);
        hydra_pool_add_thread(pool->threads, t);
    }

    pool->status = STARTING;

    return 0;
}

int hydra_pool_add_job(hydra_pool_t *pool, hydra_job_t *job)
{
    hydra_job_node_t *new_job = hydra_create_job_node(job);
    hydra_job_queue_add(pool->shared->jobs, pool->shared->lock, new_job);

    return 0;
}

hydra_job_t *hydra_pool_dispatch_job(hydra_pool_t *pool)
{
    return hydra_job_queue_dispatch(pool->shared->jobs, pool->shared->lock);
}

int hydra_pool_destroy(hydra_pool_t *pool)
{
    while (pool->nthreads > 0)
    {
        hydra_pool_remove_last_thread(pool->threads);
        pool->nthreads--;
    }

    if (pool->threads != NULL) free(pool->threads);
    if (pool->shared != NULL) free(pool->shared);
    pthread_mutex_destroy(&(pool->shared->lock));

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

hydra_job_node_t *hydra_create_job_node(hydra_job_t *job)
{
    hydra_job_node_t *jnode;
    // TODO: add malloc validations
    jnode = (hydra_job_node_t *)malloc(sizeof(hydra_job_node_t));
    jnode->job = (hydra_job_t *)malloc(sizeof(hydra_job_t));
    jnode->job->routine = job->routine;
    jnode->job->args = job->args;
    jnode->job->status = PENDING;
    jnode->next = NULL;

    return jnode;
}

int hydra_pool_add_thread(hydra_threads_t *threads, hydra_thread_t *thread)
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

int hydra_pool_remove_last_thread(hydra_threads_t *threads)
{
    hydra_thread_node_t *tpb = threads->begin, *tp = threads->begin;
    if (tp == NULL) return 0;
    for (tp = (tp->next == NULL) ? tp : tp->next; tp != NULL; tp = tp->next, tpb = tpb->next)
        if (tp->next == NULL)
            break;

    printf("Removing thread %d from active threads\n", tp->thread->id);
    // TODO: Handle detached state
    hydra_thread_cancel(tp->thread->id);
    free(tp->thread), tp->thread = NULL;
    tpb->next = NULL, free(tp), tp = NULL;

    return 0;
}

int hydra_pool_remove_thread(hydra_threads_t *threads, pthread_t tid)
{
    hydra_thread_node_t *tp;
    for (tp = threads->begin; tp != NULL; tp = tp->next) {
        if (tp->thread->id == tid) {
            break;
        }
    }

    return 0;
}

void hydra_pool_list_threads(hydra_pool_t *pool)
{
    hydra_thread_node_t *tp;
    for (tp = pool->threads->begin; tp != NULL; tp = tp->next)
        printf("Thread id %d\n", tp->thread->id);
}

hydra_thread_t *hydra_pool_alloc_thread(void *args)
{
    hydra_thread_t *t = (hydra_thread_t *)malloc(sizeof(hydra_thread_t));
    t->attr = NULL, t->routine = &hydra_handler;
    t->args = args;

    return t;
}
