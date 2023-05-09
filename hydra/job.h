#ifndef __HYDRA_JOB_INCLUDED_H__
#define __HYDRA_JOB_INCLUDED_H__

enum JOB_STATUS {
    PENDING,
    DONE
};

typedef enum JOB_STATUS job_status_t;

struct hydra_job {
    void *(*routine)(void *);
    void *args;
    job_status_t status;
};

typedef struct hydra_job hydra_job_t;

#endif // __HYDRA_JOB_INCLUDED_H__
