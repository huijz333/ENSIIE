/**
 * Petite barrière à partir pour tester le broadcast
 *
 * 1 - Crée un thread qui lance une routine qui termine immédiatement
 * 2 - Initialise la condition
 * 3 - Bloque N-1 threads enfant sur la condition
 * 4 - un N-ième thread effectue un broadcast pour tous les débloquer (<=> barrière)
 * 5 - Attends que les threads se terminent
 * 6 - Détruit la condition
 *
 * Sortie attendu:
 	 > [1] WAITING
	 > [0] WAITING
	 > [...]
	 > [3] WAITING
	 > [5] BROADCASTING
	 > [1] PASSED
	 > [0] PASSED
		[...]
	 > [3] PASSED
	 > Success
 *
 */

#include <stdio.h>
#include <mthread.h>

/* nombre de threads */
# define N 8

/* un mutex sur le compteur de thread en attente dans la barrière */
mthread_mutex_t mutex = MTHREAD_MUTEX_INITIALIZER;
unsigned int waiting = 0;

/* la condition sur laquelle les threads attendent */
mthread_cond_t barrier;

static void * work(void * i) {
    /* si on est le maitre */
    long id = (long)i;
    mthread_mutex_lock(&mutex);
    ++waiting;
    if (waiting < N) {
        printf("[%ld] WAITING\n", id);
        mthread_cond_wait(&barrier, &mutex);
        printf("[%ld] PASSED\n", id);
    } else {
        printf("[%ld] BROADCASTING\n", id);
        mthread_cond_broadcast(&barrier);
    }
    mthread_mutex_unlock(&mutex);

    return NULL;
}

int main(void) {
    /* initialisation du mutex et des barrieres */
    mthread_cond_init(&barrier, NULL);

    /* création des threads */
    mthread_t threads[N];
    long i;
    for (i = 0 ; i < N ; i++) {
        mthread_create(threads + i, NULL, work, (void *)i);
    }

    for (i = 0 ; i < N ; i++) {
        mthread_join(threads[i], NULL);
    }

    puts("Success");

    return 0;
}

