#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t sem;

void*affichage(void*name) {
	int i, j;
	for (i = 0; i < 20; i++) {
		sem_wait(&sem); /*prologue*/
		for (j = 0; j < 5; j++) {
			printf("%s ", (char*) name);
			fflush(stdout);
		}
		sched_yield(); /*pour etre sur d’avoir des problemes*/
		for (j = 0; j < 5; j++) {
			printf("%s ", (char*) name);
			fflush(stdout);
		}
		printf("\n");
		fflush(stdout);
		sem_post(&sem); /*epilogue*/
	}
	return NULL;
}

int main(void) {
	pthread_t filsA, filsB;
	sem_init(&sem, 0, 1);
	if (pthread_create(&filsA, NULL, affichage, "AA")) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	if (pthread_create(&filsB, NULL, affichage, "BB")) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}
	if (pthread_join(filsA, NULL))
		perror("pthread_join");
	if (pthread_join(filsB, NULL))
		perror("pthread_join");
	printf("Fin du pere\n");
	return (EXIT_SUCCESS);
}
