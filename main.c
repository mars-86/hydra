#include <stdio.h>
#include "hydra.h"
#include <windows.h>

int main(void)
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);

    printf("cores: %d\n", sysinfo.dwNumberOfProcessors);
    hydra_pool_t pool;

    hydra_pool_init(&pool, sysinfo.dwNumberOfProcessors + 1);
    hydra_job_t job;

    int i;
    for (i = 0; i < 1024; ++i) {
        job.args = i + 1;
        job.func = NULL;
        hydra_pool_add_job(&pool, &job);
    }

    Sleep(10000);
    hydra_pool_destroy(&pool);

    return 0;
}
