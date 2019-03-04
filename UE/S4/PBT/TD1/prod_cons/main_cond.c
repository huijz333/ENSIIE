/**
 * Problème d'exemple de producteur / consommateur
 *
 * On possède une base de données d'entier sous forme de liste chainée
 *
 * N threads accèdent de manière concurrente en lecture et en ecriture dans la base.
 * Une valeur est inséré dans la base à partir de la fonction 'produire'
 * Lorsqu'une valeur est lu, elle est supprimé de la base (fonction 'consommer')
 * Si aucunes valeurs n'est présente dans la base, le consommateur attends qu'une valeur soit produite
 *
 */


# include <pthread.h>
# include <semaphore.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

/* base de données d'entier */
typedef struct	s_db_data {
	int donnee;
	struct s_db_data * next;
}		t_db_data;

typedef struct	s_db {
	/* 1ere donnee dans la db */
	t_db_data * first;

	/* condition et mutex pour attendre tant que la*/
	pthread_cond_t empty_cond;
	pthread_mutex_t empty_mutex;

	/* mutex pour l'acces à la base */
	pthread_mutex_t mutex;
}		t_db;

/* variable global contenant les valeurs produites */
t_db db;
int nb_threads;

/* produit une valeur */
static void produire(int donnee) {
	t_db_data * new_data = (t_db_data *) malloc(sizeof(t_db_data));
	if (new_data == NULL) {
		fprintf(stderr, "malloc() error\n");
		return ;
	}
	new_data->donnee = donnee;
	pthread_mutex_lock(&db.mutex);
	new_data->next = db.first;
	db.first = new_data;
	pthread_cond_signal(&db.empty_cond);
	pthread_mutex_unlock(&db.mutex);
}

/* consomme une valeur */
static int consommer(void) {
	while (!db.first) {
		pthread_mutex_lock(&db.empty_mutex);
		pthread_cond_wait(&db.empty_cond, &db.empty_mutex);
		pthread_mutex_unlock(&db.empty_mutex);
	}	
	
	pthread_mutex_lock(&db.mutex);
	t_db_data * curr = db.first;
	db.first = db.first->next;
	pthread_mutex_unlock(&db.mutex);

	int donnee = curr->donnee;
	free(curr);

	return donnee;
}

/* consomme toute les valeurs */
static void vider(void) {
	while (db.first) {
		t_db_data * curr = db.first;
		db.first = curr->next;
		free(curr);
	}
}

/* boucle infini pour produire */
static long producteur(long thrdID) {
	while (1) {
		int donnee = (int)thrdID;
		produire(donnee);
		printf("(%ld) + %d\n", thrdID, donnee);
		sleep(1);
	}
	return thrdID;
}

/* boucle infini pour consommer */
static long consommateur(long thrdID) {
	while (1) {
		int donnee = consommer();
		printf("(%ld) - %d\n", thrdID, donnee);
	}
	return thrdID;
}

/* main */
int main(int argc, char ** argv) {
	/* lecture arguments */
	if (argc < 2) {
		fprintf(stderr, "usage: %s [N_THREADS] {N_INTEGERS}\n", argv[0]);
		return 1;
	}
	nb_threads = atoi(argv[1]);
	srand(time(NULL));

	/* initialisation de la db */
	db.first = NULL;
	pthread_mutex_init(&db.mutex, NULL);
	pthread_mutex_init(&db.empty_mutex, NULL);
	pthread_cond_init(&db.empty_cond, NULL);

	/* création des threads */
	pthread_t * thrds = (pthread_t *) malloc(nb_threads * sizeof(pthread_t));
	long i;
	for (i = 0 ; i < nb_threads ; i++) {
		void * (*routine)(void *) = (void *(*)(void *)) (rand() % 2 == 0 ? producteur : consommateur);
		void * thrdID = (void *)i;
		if (pthread_create(thrds + i, NULL, routine, thrdID)) {
			fprintf(stderr, "Couldn't create thread %ld\n", i);
			memset(thrds + i, 0, sizeof(pthread_t));
		}
	}

	/* join sur les threads */
	for (i = 0 ; i < nb_threads ; i++) {
		pthread_t * thrd = thrds + i;
		if (!thrd) {
			continue ;
		}
		void * unused;
		pthread_join(*thrd, &unused);
	}

	free(thrds);

	/* vide la base de données (free la mémoire) */
	vider();
	pthread_mutex_destroy(&db.empty_mutex);
	pthread_mutex_destroy(&db.mutex);

	return 0;
}
