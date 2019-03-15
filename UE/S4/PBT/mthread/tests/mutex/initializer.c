/**
 * 1 - Crée 16 threads et 1 mutex à partir de la macro d'initilisation
 * 2 - Chaque thread incrémente une variable partagé en mémoire (pour vérifier que le mutex fonctionne)
 *
 * Résultat attendu :
 * > Success
 */

#include <stdio.h>
#include <mthread.h>
#include <assert.h>

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

    assert(i == 16);
    puts("Success");

    return 0;
}
