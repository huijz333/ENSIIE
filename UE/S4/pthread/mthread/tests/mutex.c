#include <stdio.h>
#include <mthread.h>

# define NB_THREADS 16

int i = 0;
mthread_mutex_t mutex;

static void * run(void * arg) {
	mthread_mutex_lock(&mutex);
	i++;
	mthread_mutex_unlock(&mutex);
	return NULL;
}
int main(int argc, char ** argv) {
	int j;
	mthread_t threads[NB_THREADS];

	mthread_mutex_init(&mutex, NULL);
	for (j = 0; j < NB_THREADS; j++) {
		mthread_create(threads + j, NULL, run, NULL);
	}
	for (j = 0; j < NB_THREADS; j++) {
		mthread_join(threads[j], NULL);
	}
	mthread_mutex_destroy(&mutex);

	printf("%d\n", i);
	return 0;
}
