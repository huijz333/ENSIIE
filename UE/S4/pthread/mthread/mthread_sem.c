#include "mthread_internal.h"

/* Functions for handling semaphore.  */

int mthread_sem_init(mthread_sem_t * sem, unsigned int value) {
	sem->value	= value;
	sem->lock	= 0;
	sem->list	= NULL;
    return 0;
}

int mthread_sem_wait(mthread_sem_t * sem) {

	/** semaphore NULL => invalid argument */
    if (sem == NULL) {
        mthread_log("SEM_WAIT","SEM NULL\n");
        return -1;
    }

    /** prends le verrou */
    mthread_spinlock_lock(&sem->lock);

	/** si le semaphore ne peut plus accueillir de thread */
	if (sem->value == 0) {

        /** initialise la liste des threads si besoin */
        if (sem->list == NULL) {
        	sem->list = malloc(sizeof(mthread_list_t));
            if (sem->list == NULL) {
    			perror("malloc");
    			exit(1);
            }
            sem->list->first = NULL;
            sem->list->last  = NULL;
        }

        /** insere le thread courant dans la liste des threads sur le mutex */
    	mthread_t self = mthread_self();
        mthread_insert_last(self, sem->list);

        /** bloque le thread */
        self->status = BLOCKED;

        /** enregistres le spinlock du semaphore sur le processeur virtuel */
        mthread_virtual_processor_t * vp = mthread_get_vp();
        vp->p = &(sem->lock);

        /** passe au thread suivant en appelant l'ordonnanceur */
        mthread_yield();

        mthread_log("SEM_LOCK","SEM already locked\n");

        return -1;
	}

	/** sinon, si le semaphore peut encore accueillir de threads */
	--sem->value;
	mthread_spinlock_unlock(&sem->lock);
	mthread_log("SEM_WAIT","SEM acquired\n");

	return 0;
}

/* V(sem), signal(sem) */
int mthread_sem_post(mthread_sem_t * sem) {

	/** si mutex NULL, retourne 'invalid argument' */
    if (sem == NULL) {
        mthread_log("SEM_POST","SEM NULL\n");
        return -1;
    }

    /** prends le spinlock */
    mthread_spinlock_lock(&sem->lock);

    /** increment la valeur du semaphore */
	++sem->value;

    /** s'il y a un thread en attente sur le semaphore */
	if (sem->list) {
		/** on le retire de la liste */
		mthread_t thrd = mthread_remove_first(sem->list);

		/** si on a debloqué le dernier thread, on libère la liste en mémoire */
		if (mthread_is_empty(sem->list)) {
			free(sem->list);
			sem->list = NULL;
		}

		/** on le debloque */
		thrd->status = RUNNING;

		/** ce thread devient prêt à l'execution */
		mthread_virtual_processor_t * vp = mthread_get_vp();
		mthread_insert_last(thrd, &(vp->ready_list));
	}

    mthread_spinlock_unlock(&sem->lock);

    mthread_log("SEM_POST","SEM posted\n");
    return 0;


}

int mthread_sem_getvalue(mthread_sem_t * sem, int *sval) {
	*sval = sem->value;
	return 0;
}

int mthread_sem_trywait(mthread_sem_t * sem) {
	not_implemented ();
	return 0;
}

/* undo sem_init() */
int mthread_sem_destroy(mthread_sem_t * sem) {
	not_implemented ();
	return 0;
}
