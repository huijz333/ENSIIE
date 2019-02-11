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

	/* semaphore qui assure que la db est non vide pour un consommateur */
	sem_t sem;

	/* mutex pour l'acces à la base */
	pthread_mutex_t mutex;
}		t_db;

/* variable global contenant les valeurs produites */
t_db db;
int nb_threads;

/* produit une valeur */
static void produire(int donnee) {
	/* crée une valeur */
	t_db_data * new_data = (t_db_data *) malloc(sizeof(t_db_data));
	if (new_data == NULL) {
		fprintf(stderr, "malloc() error\n");
		return ;
	}
	new_data->donnee = donnee;

	/* ajoute la valeur à la base */
	pthread_mutex_lock(&db.mutex);
	new_data->next = db.first;
	db.first = new_data;
	pthread_mutex_unlock(&db.mutex);

	/* notifie la création d'une valeur */
	sem_post(&db.sem);
}

/* consomme une valeur */
static int consommer(void) {
	/* attends qu'une valeur soit créée */
	sem_wait(&db.sem);
	
	/* supprime la valeur de la base*/
	pthread_mutex_lock(&db.mutex);
	t_db_data * curr = db.first;
	db.first = curr->next;
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
		/* petit sleep pour forcer un autre producteur à produire */
		usleep(1);
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
		fprintf(stderr, "usage: %s [N_THREADS]\n", argv[0]);
		return 1;
	}
	nb_threads = atoi(argv[1]);
	srand(time(NULL));

	/* initialisation de la db */
	db.first = NULL;
	pthread_mutex_init(&db.mutex, NULL);
	sem_init(&db.sem, 0, 0);

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
	pthread_mutex_destroy(&db.mutex);
	sem_destroy(&db.sem);

	return 0;
}
