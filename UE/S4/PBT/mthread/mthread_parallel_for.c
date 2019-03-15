#include "mthread_internal.h"
#include <errno.h>

/**
 *	Routine qui fait executer à un worker le job qui lui est assigné
 */
static void * mthread_parallel_for_work(mthread_pf_worker_t * worker) {
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
int mthread_parallel_for(mthread_pf_t * conf, void (* run)(mthread_pf_context_t *)) {

	/* création des threads */
	mthread_pf_worker_t * workers = (mthread_pf_worker_t *) malloc(sizeof(mthread_pf_worker_t) * conf->num_threads);
	if (workers == NULL) {
		return ENOMEM;
	}

	/* tableau qui discrètise tous les jobs à effectuer pour paralléliser la boucle */
	mthread_pf_job_t * jobs;

	/* selectionne le mode dans lequel le 'for' est lancé */
	switch(conf->schedule) {

		/* dans le cas static : 1 tâche par thread */
		case MTHREAD_PARALLEL_FOR_STATIC:

			/* allocation des tâches : 1 par thread */
			jobs = (mthread_pf_job_t *) malloc(sizeof(mthread_pf_job_t) * conf->num_threads);
			if (jobs == NULL) {
				free(workers);
				return ENOMEM;
			}

			/** variable pour séparer les jobs sur les workers */
			int jobs_per_thread = (conf->end - conf->bgn + 1) / conf->num_threads;

			/* fork : création des threads et lancement de la tâche */
			/* pour chaque thread */
			for (int thread_id = 0 ; thread_id < conf->num_threads ; thread_id++) {

				/* initialise la tâche associé au thread */
				mthread_pf_worker_t * worker = workers + thread_id;

				/* initialisation du job */
				worker->job = jobs + thread_id;
				worker->job->bgn = thread_id * jobs_per_thread;
				worker->job->end = (conf->end <= worker->job->bgn + jobs_per_thread) ? conf->end : worker->job->bgn + jobs_per_thread;
				worker->job->run = run;

				/* initialisation du contexte */
				worker->ctx.thread_id = thread_id;

				/* et on execute la tâche */
			    mthread_create(&(workers[thread_id].thread), NULL, (void *(*)(void *))mthread_parallel_for_work, (void *)worker);
			}

			/** libère les jobs */
			free(jobs);

			break ;

		/* le mode d'ordonnancement des tâches n'est pas implémenté */
		default:
	        not_implemented ();
	}

	/* libère la mémoire */
	for (int thread_id = 0 ; thread_id < conf->num_threads ; thread_id++) {
	    mthread_join(workers[thread_id].thread, NULL);
	}
	free(workers);

	/* succès */
	return 0;
}
