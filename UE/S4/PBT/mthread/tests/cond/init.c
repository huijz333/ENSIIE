/**
 * 1 - Crée un thread qui lance une routine qui termine immédiatement
 * 2 - Initialise une condition
 *
 * Résultat attendu :
 * > [...]
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

    mthread_cond_t cond;
    puts("Initializing cond...");
    mthread_cond_init(&cond, NULL);
    puts("Done");
    puts("Joining threads");
    mthread_join(thrd, NULL);
    puts("Success");
    return 0;
}

