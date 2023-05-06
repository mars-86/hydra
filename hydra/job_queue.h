#ifndef __HYDRA_JOB_QUEUE_H_INCLUDED__
#define __HYDRA_JOB_QUEUE_H_INCLUDED__

enum JOB_STATUS {
    PENDING,
    DONE
};

typedef enum JOB_STATUS job_status_t;

struct hydra_job {
    void *(*func)(void *);
    void *args;
    job_status_t status;
};

typedef struct hydra_job hydra_job_t;

struct hydra_job_node {
    hydra_job_t *job;
    struct hydra_job_node *next;
};

typedef struct hydra_job_node hydra_job_node_t; 

struct hydra_job_queue {
    hydra_job_node_t *begin;
    hydra_job_node_t *end;
    int jobs;
};

typedef struct hydra_job_queue hydra_job_queue_t;

hydra_job_queue_t *hydra_job_queue_init(void);
int hydra_job_queue_add(hydra_job_queue_t *queue, hydra_job_node_t *job);
hydra_job_t *hydra_job_queue_dispatch(hydra_job_queue_t *queue);
int hydra_job_queue_destroy(hydra_job_queue_t *queue);

#endif // __HYDRA_JOB_QUEUE_H_INCLUDED__