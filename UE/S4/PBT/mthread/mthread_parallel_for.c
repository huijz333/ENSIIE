#include "mthread_internal.h"
#include <string.h>
#include <errno.h>

/**
 * initialise la structure du parallel for avec
 * les valeurs par défaut
 */
void mthread_pf_default_conf(mthread_pf_t * conf) {

	/* variable d'environnement */
	char * num_threads = getenv("MTHREAD_PARALLEL_FOR_NUM_THREADS");
	if (num_threads) {
		conf->num_threads = atoi(num_threads);
	} else {
		conf->num_threads = 1;
	}

	/* ordonnancement static par défaut */
	conf->schedule = MTHREAD_PARALLEL_FOR_STATIC;

	/* taille des chunks (nombre d'iteration par job) */
	conf->chunk_size = 1;

	/* nombre de reduction */
	conf->n_reductions = 0;

	/**
	 *  Non initialisé volontairement
	 *
	 *	conf->bgn = 0;
	 * 	conf->end = 0;
	 */
}

/**
 *	Routine qui fait executer à un worker le job qui lui est assigné
 */
static void * mthread_pf_work(mthread_pf_worker_t * worker) {
	int i;
	for (i = worker->job.bgn ; i < worker->job.end ; i++) {
		/* maj du contexte */
		worker->ctx.iterator = i;

		/* execute la routine */
		worker->job.run(&(worker->ctx));
	}

	return NULL;
}

/**
 * Routine en mode dynamique
 * Tous les workers appels cette routine, et prennent de nouvelles tâches dés
 * qu'ils sont disponibles
 */
static void * mthread_pf_dynamic_work(mthread_pf_dyn_worker_t * dw) {

	/** tant qu'il reste du boulot */
	while (dw->dyn->iterator < dw->dyn->conf->end) {

		/** on génère un job pour le thread courant */
		mthread_mutex_lock(&(dw->dyn->mutex));
		if (dw->dyn->iterator < dw->dyn->conf->end) {

			/** génère le job */
			dw->worker.job.bgn = dw->dyn->iterator;
			dw->worker.job.end = dw->dyn->iterator + dw->dyn->conf->chunk_size;
			if (dw->worker.job.end > dw->dyn->conf->end) {
				dw->worker.job.end = dw->dyn->conf->end;
			}
			dw->dyn->iterator = dw->worker.job.end;

			/** la tâche suivante peut être généré */
			mthread_mutex_unlock(&(dw->dyn->mutex));

			/* lance la tâche */
			mthread_pf_work(&(dw->worker));

		} else {
			mthread_mutex_unlock(&(dw->dyn->mutex));
		}
	}

	return NULL;
}

/**
 *	L'implémentation du 'parallel for'
 *
 *	@param conf : la configuration de la boucle
 *	@param run  : une fonction qui sera appelé par chaque thread, avec son contexte d'execution
 */
int mthread_pf(mthread_pf_t * conf, void (* run)(mthread_pf_context_t *)) {

	/* selection du mode d'ordonnancement des tâches */
	enum mthread_parallel_for_schedule schedule;
	switch (conf->schedule) {
		/* si configuré au runtime */
		case MTHREAD_PARALLEL_FOR_RUNTIME:
		{
			char * mode = getenv("MTHREAD_PARALLEL_FOR_SCHEDULE");
			schedule = MTHREAD_PARALLEL_FOR_STATIC;
			if (mode) {
				if (strcmp(mode, "dynamic") == 0) {
					schedule = MTHREAD_PARALLEL_FOR_DYNAMIC;
				} else if (strcmp(mode, "guided") == 0) {
					schedule = MTHREAD_PARALLEL_FOR_GUIDED;
				}
			}
			break ;
		}

		/* sinon on recupere la config */
		default:
		{
			schedule = conf->schedule;
			break ;
		}
	}

	/* s'il y a des reductions a effectué, on alloue les mutexes */
	mthread_mutex_t * reduction_mutexes = NULL;
	if (conf->n_reductions) {
		reduction_mutexes = (mthread_mutex_t *) safe_malloc(sizeof(mthread_mutex_t) * conf->n_reductions);
		unsigned int i;
		for (i = 0 ; i < conf->n_reductions ; i++) {
			mthread_mutex_init(reduction_mutexes + i, NULL);
		}
	}

	/* execution selon le mode dans lequel le 'for' est lancé */
	switch(schedule) {

		/* dans le cas static : 1 tâche par thread */
		case MTHREAD_PARALLEL_FOR_STATIC:
		{
			/* création des threads */
			mthread_pf_worker_t * workers = (mthread_pf_worker_t *) safe_malloc(sizeof(mthread_pf_worker_t) * conf->num_threads);

			/* allocation des tâches : 1 par thread */
			int iterator_per_thread = (conf->end - conf->bgn) / conf->num_threads;

			/* fork : création des threads et lancement de la tâche */
			/* pour chaque thread */
			for (int thread_id = 0 ; thread_id < conf->num_threads ; thread_id++) {

				/* initialise le thread */
				mthread_pf_worker_t * worker = workers + thread_id;

				/* initialisation la tâche associé au thread */
				worker->job.bgn = thread_id * iterator_per_thread;
				worker->job.end = (conf->end <= worker->job.bgn + iterator_per_thread) ? conf->end : (worker->job.bgn + iterator_per_thread);
				worker->job.run = run;

				/* initialisation du contexte */
				worker->ctx.arg = conf->arg;
				worker->ctx.thread_id = thread_id;
				worker->ctx.mutexes = reduction_mutexes;

				/* et on execute la tâche */
			    mthread_create(&(worker->thread), NULL, (void *(*)(void *))mthread_pf_work, (void *)worker);
			}

			/* join : libère la mémoire */
			for (int thread_id = 0 ; thread_id < conf->num_threads ; thread_id++) {
				mthread_pf_worker_t * worker = workers + thread_id;
			    mthread_join(worker->thread, NULL);
			}
			free(workers);

			break ;
		}




		/* mode dynamique */
		case MTHREAD_PARALLEL_FOR_DYNAMIC:
		{
			/* création des threads */
			mthread_pf_dyn_worker_t * workers = (mthread_pf_dyn_worker_t *) safe_malloc(sizeof(mthread_pf_dyn_worker_t) * conf->num_threads);

			/*
			 * création de la structure partagé par tous les workers pour travailler
			 * dynamiquement en parallèle
			 */
			mthread_pf_dyn_t dyn;
			dyn.conf		= conf;
			dyn.iterator	= conf->bgn;
			mthread_mutex_init(&(dyn.mutex), NULL);

			/* fork : création des threads */
			for (int thread_id = 0 ; thread_id < conf->num_threads ; thread_id++) {

				/* initialise le thread */
				mthread_pf_dyn_worker_t * dw = workers + thread_id;
				dw->dyn = &dyn;
				dw->worker.ctx.arg = conf->arg;
				dw->worker.ctx.thread_id = thread_id;
				dw->worker.ctx.mutexes = reduction_mutexes;

				dw->worker.job.run = run;
			    mthread_create(&(dw->worker.thread), NULL, (void *(*)(void *))mthread_pf_dynamic_work, (void *)dw);
			}


			/* join : libère la mémoire */
			for (int thread_id = 0 ; thread_id < conf->num_threads ; thread_id++) {
				mthread_pf_dyn_worker_t * dw = workers + thread_id;
			    mthread_join(dw->worker.thread, NULL);
			}
			free(workers);

			break ;
		}

		/* le mode d'ordonnancement des tâches GUIDED n'est pas implémenté */
		case MTHREAD_PARALLEL_FOR_GUIDED:

		default:
	        not_implemented ();
	}

	/* libère la mémoire */
	if (reduction_mutexes) {
		free(reduction_mutexes);
	}

	/* succès */
	return 0;
}
