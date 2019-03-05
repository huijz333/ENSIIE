#include <stdio.h>
#include <mthread.h>

# define NB_THREADS 16

int i = 0;
mthread_mutex_t mutex = MTHREAD_MUTEX_INITIALIZER;

static void * run(void * arg) {
    mthread_mutex_lock(&mutex);
    i++;
    mthread_mutex_unlock(&mutex);
    return NULL;
}

int main(int argc, char ** argv) {
    mthread_t threads[NB_THREADS];

    int j;
    for (j = 0 ; j < NB_THREADS ; j++) {
        mthread_create(threads + j, NULL, run, (void *)((long)(j + 1)));
    }

    for (j = 0 ; j < NB_THREADS ; j++) {
        mthread_join(threads[j], NULL);
    }

    printf("%d\n", i);
    puts("Success");

    return 0;
}
