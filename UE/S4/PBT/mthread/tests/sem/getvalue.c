
/**
 * 1 - Crée 2 threads et un sémaphore avec une valeur de 1
 * 2 - Chacun des 2 threads prennent successivement le sémaphore
 * 3 - Chaque thread vérifie les valeurs successives du sémaphore
 *
 * Sortie attendu:

	Initializing semaphore...
	Done
	Joining threads
	Success


 */

#include <stdio.h>
#include <mthread.h>
#include <assert.h>

mthread_sem_t sem;

static void * run(void * unused) {
    unsigned int value;

    mthread_sem_getvalue(&sem, &value);
    assert(value == 1);

    mthread_sem_wait(&sem);

    mthread_sem_getvalue(&sem, &value);
    assert(value == 0);

    mthread_sem_post(&sem);

    mthread_sem_getvalue(&sem, &value);
    assert(value == 1);

    return NULL;
}

int main(void) {
    mthread_t thrd1;
    mthread_t thrd2;
    mthread_create(&thrd1, NULL, run, NULL);
    mthread_create(&thrd2, NULL, run, NULL);

    puts("Initializing semaphore...");
    mthread_sem_init(&sem, 1);
    puts("Done");
    puts("Joining threads");
    mthread_join(thrd1, NULL);
    mthread_join(thrd2, NULL);
    puts("Success");
    return 0;
}

