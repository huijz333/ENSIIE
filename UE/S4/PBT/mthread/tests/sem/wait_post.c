/**
 *
 * Test que les fonctions sem_wait et sem_post fonctionnent bien
 *
 *
 * 1 - Crée 16 thread qui incrémente un entier partagé en mémoire (dans le segment data)
 * 2 - L'incrément est effectué dans un semaphore (valeur de 1 <=> mutex)
 * 3 - On vérifie que tous les incréments ont bien eu lieu sans erreur
 *
 * Résultat attendu :
 * > [...]
 * > Résultat attendu : 16000
 * > Résultat obtenu  : 16000
 * > Success
 */

# include <stdio.h>
# include <mthread.h>
# include <unistd.h>
# include <assert.h>

# define NB_THREADS 16
# define INC_PER_THREAD 1000

int i = 0;
mthread_sem_t sem;

static void * run(void * arg) {
    printf("[(sem.value=%d) THREAD %ld] tente de passer le verrou\n", sem.value, (long)arg);
    fflush(stdout);
    mthread_sem_wait(&sem);
    printf("[(sem.value=%d) THREAD %ld] a passé le verrou\n", sem.value, (long)arg);
    fflush(stdout);
    int j;
    for (j = 0 ; j < INC_PER_THREAD ; j++) {
        i++;
    }
    mthread_sem_post(&sem);
    printf("[(sem.value=%d) THREAD %ld] a libéré le verrou\n", sem.value, (long)arg);
    fflush(stdout);
    return NULL;
}

int main(int argc, char ** argv) {
    mthread_t threads[NB_THREADS];
    mthread_sem_init(&sem, 1);

    int j;
    for (j = 0 ; j < NB_THREADS ; j++) {
        mthread_create(threads + j, NULL, run, (void *)((long)(j + 1)));
    }

    for (j = 0 ; j < NB_THREADS ; j++) {
        mthread_join(threads[j], NULL);
    }

    printf("Résultat attendu : %d\n", NB_THREADS * INC_PER_THREAD);
    printf("Résultat obtenu  : %d\n", i);
    printf("%s\n", NB_THREADS * INC_PER_THREAD == i ? "Success" : "Error");
    assert(i == NB_THREADS * INC_PER_THREAD);

    return NB_THREADS * INC_PER_THREAD == i ? 0 : 1;
}
