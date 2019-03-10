/**
 * Crée plusieurs thread qui crée et supprime des clefs de manières aléatoires
 *
 * Résultat attendu :
 * > Success
 */

# include <stdio.h>
# include <stdlib.h>
# include <mthread.h>
# include <unistd.h>
# include <assert.h>

/* nombre de threads qui crée et supprime des clefs */
# define N_THREADS 		1

/* nombre maximal de clefs qui sont créé ou supprimé */
# define MAX_BATCH_SIZE	4

/* nombre d'itérations création/suppression */
# define N_ITER			8

static void * run(void * unused) {
    mthread_key_t * key = (mthread_key_t *) malloc(sizeof(mthread_key_t) * (MAX_BATCH_SIZE + 1));
    int i;
    for (i = 0 ; i < N_ITER ; i++) {
        /* crée de nombreuses clefs */
        int nb_keys = (rand() % MAX_BATCH_SIZE);
        int j;
        for (j = 0 ; j < nb_keys ; j++) {
            mthread_key_create(key + j, NULL);
            printf("created %u\n", key[j]);
        }

        /* en supprime les 2/3 */
        nb_keys = 2 * nb_keys / 3;
        for (j = 0 ; j < nb_keys ; j++) {
            mthread_key_delete(key[j]);
            printf("deleted %u\n", key[j]);
        }

    }

    return NULL;
}

int main(void) {
    mthread_t thrd[N_THREADS];

    int i;
    puts("Creating threads");
    for (i = 0 ; i < N_THREADS ; i++) {
        mthread_create(thrd + i, NULL, run, NULL);
    }

    puts("Joining threads");
    for (i = 0 ; i < N_THREADS ; i++) {
        mthread_join(thrd[i], NULL);
    }

    usleep(10000);
    puts("Success");
    return 0;
}

