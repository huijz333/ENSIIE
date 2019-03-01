/**
 * Un ping-pong à l'aide d'un condition
 *
 * 1 - Création de 2 threads ('ping' et 'pong')
 * 2 - Initialisation : 'ping' attends que 'pong' soit prêt pour afficher son 1er 'ping'
 * 3 - Le thread 'pong' affiche 'pong' après que 'ping' lui ai signalé avoir affiché 'ping'
 * 4 - Le thread 'ping' affiche 'ping' après que 'pong' lui ai signalé avoir affiché 'pong'
 */

# include <stdio.h>
# include <pthread.h>
# include <unistd.h>

/** la condition et le mutex associé */
pthread_cond_t cond;
pthread_mutex_t mutex;
volatile int can_start = 0;

static void * ping(void * unused) {

	/* 1. initialisation, attends 'PONG' soit en attente de signal */
	puts("[PING] starting");
	while(!can_start);
	puts("[PING] started");

	/** 2. affiche 'PING' puis le signale */
	puts("[PING] taking mutex");
    pthread_mutex_lock(&mutex);
	puts("[PING] took mutex");

	while (1) {
		puts("PING");
		pthread_cond_signal(&cond);
		pthread_cond_wait(&cond, &mutex);
	}

    pthread_mutex_unlock(&mutex);

	return NULL;
}

static void * pong(void * unused) {

    /** 1. initialisation : force 'PONG' a attendre en 1er */
    pthread_mutex_lock(&mutex);
    puts("[PONG] ping can start");
	can_start = 1;

	/** 2. affiche 'PONG' puis le signale */
	while (1) {
		puts("[PONG] taking condition");
		pthread_cond_wait(&cond, &mutex);
		puts("[PONG] took condition");
		puts("PONG");
		pthread_cond_signal(&cond);
	}

    pthread_mutex_unlock(&mutex);

	return NULL;
}

int main(void) {

	puts("Initializing cond...");
	pthread_cond_init(&cond, NULL);
	pthread_mutex_init(&mutex, NULL);

	puts("Initializing threads...");
	pthread_t thrd_ping;
	pthread_create(&thrd_ping, NULL, ping, NULL);

	pthread_t thrd_pong;
	pthread_create(&thrd_pong, NULL, pong, NULL);

	puts("Joining threads");
	pthread_join(thrd_ping, NULL);
	pthread_join(thrd_pong, NULL);

	puts("Success");
	return 0;
}

