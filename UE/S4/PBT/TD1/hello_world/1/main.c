# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

int main(int argc, char ** argv) {
	/* lecture paramètres */
	if (argc != 2) {
		fprintf(stderr, "usage: %s [N_THREADS]\n", argv[0]);
		return 1;
	}	
	int n = atoi(argv[1]);

	/* création des threads */
	pthread_t * thrds = (pthread_t *) malloc(n * sizeof(pthread_t));
	int i;
	for (i = 0 ; i < n ; i++) {
		if (pthread_create(thrds + i, NULL, (void * (*)(void *))puts, (void *)"Hello")) {
			fprintf(stderr, "Couldn't create thread %d\n", i);
			memset(thrds + i, 0, sizeof(pthread_t));
		}
	}

	/* join sur les threads */
	for (i = 0 ; i < n ; i++) {
		pthread_t * thrd = thrds + i;
		if (!thrd) {
			continue ;
		}
		void * r;
		pthread_join(*thrd, &r);
	}

	free(thrds);

	return 0;
}
