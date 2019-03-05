/**
 * Crée un thread puis 1024 clefs
 *
 * Résultat attendu :
 * > Success
 */
#include <assert.h>
#include <stdio.h>
#include <mthread.h>

static void * run(void * unused) {

	mthread_key_t key;

	int i;
	for (i = 0 ; i < 1024 ; i++) {
		mthread_key_create(&key, NULL);
		assert(key == i);
	}

	return NULL;
}

int main(void) {
	mthread_t thrd;
	mthread_create(&thrd, NULL, run, NULL);


	mthread_join(thrd, NULL);
	puts("Success");
	return 0;
}

