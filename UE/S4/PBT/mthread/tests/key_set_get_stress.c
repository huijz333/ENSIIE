/**
 * Crée un thread puis N clef avec des valeurs aléatoires, puis les recupères et les vérifies
 *
 * Résultat attendu :
 * > Success
 */

# include <assert.h>
# include <stdio.h>
# include <stdlib.h>
# include <mthread.h>
# include <unistd.h>
# include <time.h>

# define N 512

static void * run(void * unused) {

	srand(time(NULL));

	mthread_key_t * keys = (mthread_key_t *) malloc(sizeof(mthread_key_t) * N);
	int * values = (int *) malloc(sizeof(int) * N);
	int i;
	for (i = 0 ; i < N ; i++) {
		mthread_key_create(keys + i, NULL);
		long x = (long)rand();
		mthread_setspecific(keys[i], (void *)x);
		values[i] = x;
	}

	for (i = 0 ; i < N ; i++) {
		assert((long)values[i] == (long)mthread_getspecific(keys[i]));
	}

	free(values);
	free(keys);

	return NULL;
}



int main(void) {

	mthread_t thrd;
	mthread_create(&thrd, NULL, run, NULL);

	mthread_join(thrd, NULL);

	usleep(10000);
	puts("Success");

	return 0;
}

