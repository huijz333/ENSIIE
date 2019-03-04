/**
 * Un ping-pong à l'aide d'un condition
 *
 * 1 - Création de 2 threads ('ping' et 'pong')
 * 2 - Initialisation : 'ping' attends que 'pong' soit prêt pour afficher son 1er 'ping', et envoyer son 1er signal
 * 3 - Le thread 'pong' affiche 'pong' après que 'ping' lui ai signalé avoir affiché 'ping'
 * 4 - Le thread 'ping' affiche 'ping' après que 'pong' lui ai signalé avoir affiché 'pong'
 *
 * Résultat attendu:
 *  > [...]
 * 	> PING (n°5787)
 *	> [PING] sending signal
 *  > [PING] waiting condition
 *  > [PONG] passed condition
 *  > PONG (n°5787)
 *  > [PONG] sending signal
 *  > [PONG] waiting condition
 *  > [PING] passed condition
 *  > [...]
 */

# include <stdio.h>
# include <mthread.h>
# include <unistd.h>

/** la condition et le mutex associé */
mthread_cond_t cond;
mthread_mutex_t mutex;
volatile int can_start = 0;

static void debug(char * s) {
	fprintf(stderr, "%s\n", s);
}

static void * ping(void * unused) {

	/* 1. initialisation, attends 'PONG' soit en attente de signal */
	debug("[PING] starting");
	while(!can_start);
	debug("[PING] started");

	/** 2. affiche 'PING' puis le signale */
	debug("[PING] taking mutex");
    mthread_mutex_lock(&mutex);
    debug("[PING] took mutex");

    /* nombre de PING affiché */
    unsigned int count = 0;
	while (1) {
		printf("PING (n°%d)\n", count);
		debug("[PING] sending signal");
		mthread_cond_signal(&cond);
		debug("[PING] waiting condition");
		mthread_cond_wait(&cond, &mutex);
		debug("[PING] passed condition");
		++count;
	}

    mthread_mutex_unlock(&mutex);

	return NULL;
}

static void * pong(void * unused) {

    /** 1. initialisation : force 'PONG' a attendre en 1er */
    mthread_mutex_lock(&mutex);
    debug("[PONG] ping can start");
	can_start = 1;

	/** 2. affiche 'PONG' puis le signale */

    /* nombre de PONG affiché */
	unsigned int count = 0;
	while (1) {
		debug("[PONG] waiting condition");
		mthread_cond_wait(&cond, &mutex);
		debug("[PONG] passed condition");
		printf("PONG (n°%d)\n", count);
		debug("[PONG] sending signal");
		mthread_cond_signal(&cond);
		++count;
	}

    mthread_mutex_unlock(&mutex);

	return NULL;
}

int main(void) {

	debug("Initializing cond...");
	mthread_cond_init(&cond, NULL);
	mthread_mutex_init(&mutex, NULL);

	debug("Initializing threads...");
	mthread_t thrd_ping;
	mthread_create(&thrd_ping, NULL, ping, NULL);

	mthread_t thrd_pong;
	mthread_create(&thrd_pong, NULL, pong, NULL);

	debug("Joining threads");
	mthread_join(thrd_ping, NULL);
	mthread_join(thrd_pong, NULL);

	debug("Success");
	return 0;
}

