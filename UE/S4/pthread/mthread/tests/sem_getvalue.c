#include <stdio.h>
#include <mthread.h>

mthread_sem_t sem;

static void * run(void * unused) {
	unsigned int value;

	mthread_sem_getvalue(&sem, &value); printf("sem.value = %u (expected 1)\n", value);

	mthread_sem_wait(&sem);

	mthread_sem_getvalue(&sem, &value); printf("sem.value = %u (expected 0)\n", value);

	mthread_sem_post(&sem);

	mthread_sem_getvalue(&sem, &value); printf("sem.value = %u (expected 1)\n", value);

	return NULL;
}

int main(void) {
	mthread_t thrd1;
	mthread_t thrd2;
	mthread_create(&thrd1, NULL, run, NULL);
	mthread_create(&thrd2, NULL, run, NULL);

	puts("Initializing semaphore...");
	mthread_sem_init(&sem, 1);
	puts("Done");
	puts("Joining threads");
	mthread_join(thrd1, NULL);
	mthread_join(thrd2, NULL);
	puts("Success");
	return 0;
}

