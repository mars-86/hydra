#include <stdio.h>
#include "hydra.h"
#include <windows.h>

void *print(void *args)
{
    putchar((int)args);
    putchar('\n');
}

int main(void)
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);

    printf("cores: %d\n", sysinfo.dwNumberOfProcessors);
    hydra_pool_t pool;

    hydra_pool_init(&pool, sysinfo.dwNumberOfProcessors + 1);
    hydra_job_t job;

    int i, j =0;
    while(j < 3) {
        for (i = 96; i < 128; ++i) {
            job.args = i + 1;
            job.routine = &print;
            hydra_pool_add_job(&pool, &job);
        }
        i = 0; ++j;
        printf("cycle: %d\n", j);
    }

    Sleep(2000);
    hydra_pool_destroy(&pool);

    return 0;
}
