#include <stdio.h>
#include <pthread.h>

void *hydra_handler(void *args)
{
    printf("Hydra handler created with arg %d in thread %d\n", (int*)args, pthread_self());
    while (1) {

        Sleep(20);
    }

    printf("Thread %d destroyed\n", pthread_self());
    pthread_exit(NULL);
}
