/**
 * 1 - Crée un thread, un semaphore, puis la détruit
 *
 * Sortie attendu:

	> Initializing semaphore...
	> Done
	> Joining threads
	> Thread created
	> Success

 */

#include <stdio.h>
#include <mthread.h>

static void * run(void * unused) {
    puts("Thread created");
    return NULL;
}

int main(void) {

    mthread_t thrd;
    mthread_create(&thrd, NULL, run, NULL);

    mthread_sem_t sem;
    puts("Initializing semaphore...");
    mthread_sem_init(&sem, 4);
    puts("Done");
    puts("Joining threads");
    mthread_join(thrd, NULL);
    mthread_sem_destroy(&sem);
    puts("Success");
    return 0;
}

