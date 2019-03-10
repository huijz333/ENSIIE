/**
 * Crée un thread et un mutex, puis détruit le mutex
 *
 * Résultat attendu :
 * > Success
 */

#include <stdio.h>
#include <mthread.h>

static void * run(void * unused) {
    puts("Thread crée");
    return NULL;
}

int main(void) {
    mthread_t thrd;
    mthread_create(&thrd, NULL, run, NULL);

    mthread_mutex_t mutex;
    puts("Initializing mutex...");
    mthread_mutex_init(&mutex, NULL);
    puts("Done");
    puts("Destroying mutex...");
    mthread_mutex_destroy(&mutex);
    puts("Done");
    puts("Joining threads");
    puts("Success");
    mthread_join(thrd, NULL);
    return 0;
}

