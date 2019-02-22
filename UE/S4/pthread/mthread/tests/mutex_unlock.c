#include <stdio.h>
#include <mthread.h>

# define NB_THREADS 16

int i = 0;
mthread_mutex_t mutex;

static void * run(void * arg) {
	printf("[THREAD %ld] tente de passer le verrou\n", (long)arg); fflush(stdout);
	mthread_mutex_lock(&mutex);
	printf("[THREAD %ld] a passé le verrou\n", (long)arg); fflush(stdout);
	i++;
	mthread_mutex_unlock(&mutex);
	printf("[THREAD %ld] a libéré le verrou\n", (long)arg); fflush(stdout);
	return NULL;
}

int main(int argc, char ** argv) {
	mthread_t threads[NB_THREADS];
	mthread_mutex_init(&mutex, NULL);

	int j;
	for (j = 0 ; j < NB_THREADS ; j++) {
		mthread_create(threads + j, NULL, run, (void *)((long)(j + 1)));
	}

	for (j = 0 ; j < NB_THREADS ; j++) {
		mthread_join(threads[j], NULL);
	}

	printf("%d\n", i);

	return 0;
}
