/**
 * Crée un thread puis une clef
 *
 * Résultat attendu :
 * > Success
 */

#include <stdio.h>
#include <mthread.h>

static void * run(void * unused) {
	puts("Thread crée");
	return NULL;
}

int main(void) {
	mthread_t thrd;
	mthread_create(&thrd, NULL, run, NULL);

	mthread_key_t key;
	puts("Initializing key...");
	mthread_key_create(&key, NULL);
	puts("Done");
	puts("Joining threads");
	mthread_join(thrd, NULL);
	puts("Success");
	return 0;
}

