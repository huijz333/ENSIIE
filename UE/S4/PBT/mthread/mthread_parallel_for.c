#include "mthread_internal.h"
#include <errno.h>

/**
 * initialise la structure du parallel for avec
 * les valeurs par défaut
 */
void mthread_pf_default_conf(mthread_pf_t * conf) {

	//TODO lire les variables d'environnement

	/* variable d'environnement */
	conf->num_threads = 1;

	/* ordonnancement static par défaut */
	conf->schedule = MTHREAD_PARALLEL_FOR_STATIC;

	/* taille des chunks (nombre d'iteration par job) */
	conf->chunk_size = 1;

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
	for (i = worker->job->bgn ; i < worker->job->end ; i++) {
		/* maj du contexte */
		worker->ctx.iterator = i;

		/* execute la routine */
		worker->job->run(&(worker->ctx));
	}

	return NULL;
}

/**
 *
 */
int mthread_pf(mthread_pf_t * conf, void (* run)(mthread_pf_context_t *)) {

	/* création des threads */
	mthread_pf_worker_t * workers = (mthread_pf_worker_t *) safe_malloc(sizeof(mthread_pf_worker_t) * conf->num_threads);

	/* tableau qui discrètise tous les jobs à effectuer pour paralléliser la boucle */
	mthread_pf_job_t * jobs = NULL;

	/* selectionne le mode dans lequel le 'for' est lancé */
	switch(conf->schedule) {

		/* dans le cas static : 1 tâche par thread */
		case MTHREAD_PARALLEL_FOR_STATIC:
		{
			/* allocation des tâches : 1 par thread */
			mthread_pf_job_t * jobs = (mthread_pf_job_t *) safe_malloc(sizeof(mthread_pf_job_t) * conf->num_threads);
			int iterator_per_thread = (conf->end - conf->bgn) / conf->num_threads;

			/* fork : création des threads et lancement de la tâche */
			/* pour chaque thread */
			for (int thread_id = 0 ; thread_id < conf->num_threads ; thread_id++) {

				/* initialise le thread */
				mthread_pf_worker_t * worker = workers + thread_id;

				/* initialisation la tâche associé au thread */
				worker->job = jobs + thread_id;
				worker->job->bgn = thread_id * iterator_per_thread;
				worker->job->end = (conf->end <= worker->job->bgn + iterator_per_thread) ? conf->end : (worker->job->bgn + iterator_per_thread);
				worker->job->run = run;

				/* initialisation du contexte */
				worker->ctx.thread_id = thread_id;

				/* et on execute la tâche */
			    mthread_create(&(worker->thread), NULL, (void *(*)(void *))mthread_pf_work, (void *)worker);
			}
			break ;
		}

		/* mode dynamique */
		case MTHREAD_PARALLEL_FOR_DYNAMIC:
		{
			/* allocation des tâches */
			unsigned int num_jobs = (conf->end - conf->bgn) / conf->chunk_size;
			jobs = (mthread_pf_job_t *) safe_malloc(sizeof(mthread_pf_job_t) * num_jobs);

			// TODO répartir les jobs dynamiquement sur les threads */

			break ;
		}

		/* le mode d'ordonnancement des tâches GUIDED n'est pas implémenté */
		default:
	        not_implemented ();
	}

	/* libère la mémoire */
	for (int thread_id = 0 ; thread_id < conf->num_threads ; thread_id++) {
		mthread_pf_worker_t * worker = workers + thread_id;
	    mthread_join(worker->thread, NULL);
	}
	free(jobs);
	free(workers);
	/* succès */
	return 0;
}
