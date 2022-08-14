#include <stdio.h>
#include "hydra.h"

int main(void)
{
    hydra_pool_t pool;
    hydra_init_tpool(&pool, 4);

    hydra_list_threads(&pool);

    hydra_destroy_tpool(&pool);

    return 0;
}
