# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>

/* structure passé à la routine de début d'un thread */
typedef struct	s_thrd_data {
	int id;
	char * s;
}		t_thrd_data;

/* routine qui commence un thread */
static t_thrd_data * routine(t_thrd_data * data) {
	printf("%s%d\n", data->s, data->id);
	return data;
}

int main(int argc, char ** argv) {
	/* lecture paramètres */
	if (argc != 2) {
		fprintf(stderr, "usage: %s [N_THREADS]\n", argv[0]);
		return 1;
	}	
	int n = atoi(argv[1]);

	/* création des threads */
	pthread_t * thrds = (pthread_t *) malloc(n * sizeof(pthread_t));
	t_thrd_data * thrds_data = (t_thrd_data *) malloc(n * sizeof(t_thrd_data));
	int i;
	for (i = 0 ; i < n ; i++) {
		t_thrd_data * thrd_data = thrds_data + i;
		thrd_data->id = i;
		thrd_data->s = "Hello world ! ";
		if (pthread_create(thrds + i, NULL, (void * (*)(void *))routine, thrd_data)) {
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

	free(thrds_data);
	free(thrds);

	return 0;
}
