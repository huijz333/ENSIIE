#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

# define PTHREAD_NUM_THREADS	4

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int N;
int step;
int sum = 0;

static void * routine(void * i) {
	long id = (long) i;
	int bgn = id * step;
	int end = (id + 1) * step;
	if (end > N) {
		end = N;
	}
	int local_sum = 0;
	while (bgn < end) {
		local_sum += bgn;
		++bgn;
	}
	pthread_mutex_lock(&mutex);
	sum += local_sum;
	pthread_mutex_unlock(&mutex);
	return NULL;
}

int main(int argc, char **argv) {
	N = atoi(argv[1]);
	step = N / PTHREAD_NUM_THREADS;
	
	pthread_t thrd[PTHREAD_NUM_THREADS];
	long i;
	for (i = 0 ; i < PTHREAD_NUM_THREADS ; i++) {
		pthread_create(thrd + i, NULL, routine, (void *)i);
	}

	for (i = 0 ; i < PTHREAD_NUM_THREADS ; i++) {
		pthread_join(thrd[i], NULL);
	}

	printf("sum = %d\n", sum);

	return 0;
}

