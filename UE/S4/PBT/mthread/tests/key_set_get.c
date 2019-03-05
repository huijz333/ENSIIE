/**
 * Crée un thread puis une clef, puis détruit la clef
 *
 * Résultat attendu :
 * > Success
 */

# include <assert.h>
# include <stdio.h>
# include <stdlib.h>
# include <mthread.h>
# include <unistd.h>


static void destr(void * value) {
	puts("Key value destroyed");
	free(value);
}

static void * run(void * unused) {

	mthread_key_t key;
	mthread_key_create(&key, destr);

	void * ptr = malloc(100);
	mthread_setspecific(key, ptr);
	assert(ptr == mthread_getspecific(key));

	return NULL;
}



int main(void) {

	mthread_t thrd;
	mthread_create(&thrd, NULL, run, NULL);

	mthread_join(thrd, NULL);

	/* sleep pour laisser le temps au destructeur d'être appelé */
	usleep(10000);
	puts("Success");

	return 0;
}

