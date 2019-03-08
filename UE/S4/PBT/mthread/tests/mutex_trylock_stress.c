/**
 *	Crée 32 threads qui essaye de prendre le mutex avec 'trylock'
 *
 *	Résultat attendu:
 *		- 1 seul thread renvoie "Got the mutex"
 *		- les autres "Did not get the mutex"
 */

#include <unistd.h>
#include <mthread.h>
#include <stdio.h>

# define NB_THREADS	32

volatile int exclusion_var = 0;
mthread_mutex_t mutex;

void* trylock_routine(void* arg) {
	if (mthread_mutex_trylock(&mutex) == 0) {
		printf("Got the mutex !\n");
		exclusion_var++;
		/** attends un peu histoire que les autres threads try lock */
		sleep(2);
		puts("Unlocking");
		mthread_mutex_unlock(&mutex);
		puts("Unlocked");
	} else {
		printf("Did not get the mutex\n");
	}
	return NULL;
}

int main() {
	int j;
	mthread_t pids[NB_THREADS];
	exclusion_var = 0;

	mthread_mutex_init(&mutex, NULL);

	printf("Exclusion var: %d\n", exclusion_var);
	for (j = 0; j < NB_THREADS ; j++)
		mthread_create(&(pids[j]), NULL, trylock_routine, NULL);

	for (j = 0; j < NB_THREADS; j++)
		mthread_join(pids[j], NULL);

	printf("Exclusion var: %d\n", exclusion_var);
}
