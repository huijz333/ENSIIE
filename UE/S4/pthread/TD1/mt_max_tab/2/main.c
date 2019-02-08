# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>

/* valeur maximal dans le tableau */
# define MAX_VAL (1000)

/* nombre d'entier dans le tableau */
# define N_INTEGERS (20)

/* seed pour l'aléatoire */
# define SEED (time(NULL))

/* nombre total de threads */
int nb_threads;

/* tableau d'entiers aléatoires */
int integers[N_INTEGERS];

/* maximum global */
volatile int m_global;

/* mutex sur la variable "m_global" */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * calcul le maximum local pour le thread numéro 'i'
 * @param i l'id du thread
 * @return le maximum
 */
static long localMax(long i) {
	int integers_per_thread = N_INTEGERS / nb_threads; /* taille du tableau local */
	int bgn = i * integers_per_thread; /* debut local */
	int end = (i == nb_threads - 1) ? N_INTEGERS : (i + 1) * integers_per_thread; /* fin local */
	/* printf("%d to %d\n", bgn, end); */
	int m_local = integers[bgn];
	int j;
	for (j = bgn + 1 ; j < end ; j++) {
		if (m_local < integers[j] ) {
			m_local = integers[j];
		}
	}

	pthread_mutex_lock(&mutex);
	if (m_global < m_local) {
		m_global = m_local;
	}
	pthread_mutex_unlock(&mutex);

	return (long)m_local;
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
	/* initialise la valeur max */
	m_global = integers[0];

	/* création des threads */
	pthread_t * thrds = (pthread_t *) malloc(nb_threads * sizeof(pthread_t));
	for (i = 0 ; i < nb_threads ; i++) {
		if (pthread_create(thrds + i, NULL, (void * (*)(void *))localMax, (void *)i)) {
			fprintf(stderr, "Couldn't create thread %ld\n", i);
			memset(thrds + i, 0, sizeof(pthread_t));
		}
	}

	/* join sur les threads */
	for (i = 0 ; i < nb_threads ; i++) {
		pthread_t * thrd = thrds + i;
		if (!thrd) {
			continue ;
		}
		void * unused;
		pthread_join(*thrd, &unused);
	}

	free(thrds);
	pthread_mutex_destroy(&mutex);
	printf("Maximum global: %d\n", m_global);

	return 0;
}
