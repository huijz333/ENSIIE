# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>

/* valeur maximum dans le tableau */
# define MAX_VAL (20)

/* nombre d'entier dans le tableau */
# define N_INTEGERS (20)

/* seed pour l'aléatoire */
# define SEED (time(NULL))

int nb_threads;
int integers[N_INTEGERS];

/**
 * calcul le maximum local pour le thread numéro 'i'
 * @param i l'id du thread
 * @return le maximum
 */
static long localMax(long i) {
	/* taille du tableau local */
	int integers_per_thread = N_INTEGERS / nb_threads;
	/* debut local */
	int bgn = i * integers_per_thread;
	/* fin local */
	int end = (i == nb_threads - 1) ? N_INTEGERS : (i + 1) * integers_per_thread;
	/* printf("%d to %d\n", bgn, end); */
	int m = integers[bgn];
	int j;
	for (j = bgn + 1 ; j < end ; j++) {
		if (integers[j] > m) {
			m = integers[j];
		}
	}
	return (long)m;
}

int main(int argc, char ** argv) {
	/* lecture paramètres */
	if (argc != 2) {
		fprintf(stderr, "usage: %s [N_THREADS]\n", argv[0]);
		return 1;
	}	
	nb_threads = atoi(argv[1]);

	/* création du tableau pseudo-aléatoire */
	srand(SEED);
	long i;
	for (i = 0 ; i < N_INTEGERS ; i++) {
		integers[i] = rand() % MAX_VAL;
	}

	/* création des threads */
	pthread_t * thrds = (pthread_t *) malloc(nb_threads * sizeof(pthread_t));
	for (i = 0 ; i < nb_threads ; i++) {
		if (pthread_create(thrds + i, NULL, (void * (*)(void *))localMax, (void *)i)) {
			fprintf(stderr, "Couldn't create thread %ld\n", i);
			memset(thrds + i, 0, sizeof(pthread_t));
		}
	}

	/* join sur les threads + calcul du maximum global à partir des maximums locaux */
	int m = integers[0];
	for (i = 0 ; i < nb_threads ; i++) {
		pthread_t * thrd = thrds + i;
		if (!thrd) {
			continue ;
		}
		long m_local;
		pthread_join(*thrd, (void **)&m_local);
		/* printf("Maximum local: %ld\n", m_local); */
		if ((int)m_local > m) {
			m = m_local;
		}
	}
	printf("Maximum global: %d\n", m);

	free(thrds);

	return 0;
}
