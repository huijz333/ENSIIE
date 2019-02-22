#include <stdio.h>
#include <mthread.h>

static void * run(void * unused) {
	puts("Thread crée");
	return NULL;
}

int main(void) {
	mthread_t thrd;
	mthread_create(&thrd, NULL, run, NULL);

	mthread_mutex_t mutex;
	puts("Initializing mutex...");
	mthread_mutex_init(&mutex, NULL);
	puts("Done");
	puts("Joining threads");
	mthread_join(thrd, NULL);
	puts("Success");
	return 0;
}

