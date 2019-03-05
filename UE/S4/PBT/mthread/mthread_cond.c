# include "mthread_internal.h"
# include <errno.h>

/* Functions for handling conditional variables.  */

/* Initialize condition variable COND using attributes ATTR, or use
 the default values if later is NULL.  */
int mthread_cond_init(mthread_cond_t * cond,
                      const mthread_condattr_t * __cond_attr) {
    if (__cond_attr) {
        not_implemented ();
    }

    cond->nb_thread	= 0;
    cond->lock		= 0;
    cond->list 		= NULL;

    return 0;
}

/* Destroy condition variable COND.  */
int mthread_cond_destroy(mthread_cond_t * cond) {
    if (cond->list) {
        free(cond->list);
    }
    return 0;
}

/* Wake up one thread waiting for condition variable COND.  */
int mthread_cond_signal(mthread_cond_t * cond) {

    /** si la condition est NULL, retourne 'invalid argument' */
    if (cond == NULL) {
        mthread_log("COND_SIGNAL","COND NULL\n");
        return EINVAL;
    }

    /** prends le spinlock */
    mthread_spinlock_lock(&(cond->lock));

    /** s'il n'y a aucun thread en attente sur la condition */
    if (cond->list == NULL) {
        mthread_spinlock_unlock(&(cond->lock));
        return 0;
    }

    /** sinon, on retire le 1er thread de la liste (FIFO) */
    mthread_t first = mthread_remove_first(cond->list);

    /** si on a debloqué le dernier thread, on libère la liste en mémoire */
    if (mthread_is_empty(cond->list)) {
        free(cond->list);
        cond->list = NULL;
    }

    /** on le debloque */
    first->status = RUNNING;

    /** ce thread devient prêt à l'execution */
    mthread_virtual_processor_t * vp = mthread_get_vp();
    mthread_insert_last(first, &(vp->ready_list));

    mthread_spinlock_unlock(&cond->lock);

    return 0;

}

/* Wake up all threads waiting for condition variables COND.  */
int mthread_cond_broadcast(mthread_cond_t * cond) {

    /** si la condition est NULL, retourne 'invalid argument' */
    if (cond == NULL) {
        mthread_log("COND_SIGNAL","COND NULL\n");
        return EINVAL;
    }

    /** prends le spinlock */
    mthread_spinlock_lock(&(cond->lock));

    /** s'il n'y a aucun thread en attente sur la condition */
    if (cond->list == NULL) {
        mthread_spinlock_unlock(&(cond->lock));
        return 0;
    }

    /** pour chaque thread en attente sur la condition */
    while (!mthread_is_empty(cond->list)) {
        /** on le retire de la liste */
        mthread_t thrd = mthread_remove_first(cond->list);

        /** on le debloque */
        thrd->status = RUNNING;

        /** ce thread devient prêt à l'execution */
        mthread_virtual_processor_t * vp = mthread_get_vp();
        mthread_insert_last(thrd, &(vp->ready_list));
    }


    /** finalement, on peut supprimer la liste des threads en attente */
    free(cond->list);
    cond->list = NULL;

    /** libère le spinlock */
    mthread_spinlock_unlock(&cond->lock);

    return 0;
}

/* Wait for condition variable COND to be signaled or broadcast.
 MUTEX is assumed to be locked before.  */
int mthread_cond_wait(mthread_cond_t * cond, mthread_mutex_t * mutex) {
    /** cond ou mutex NULL => invalid argument */
    if (cond == NULL || mutex == NULL) {
        mthread_log("COND_WAIT","COND or MUTEX NULL\n");
        return -1;
    }

    /** prends le verrou */
    mthread_spinlock_lock(&cond->lock);

    /** initialise la liste des threads en attente si besoin */
    if (cond->list == NULL) {
        cond->list = malloc(sizeof(mthread_list_t));
        if (cond->list == NULL) {
            perror("malloc");
            exit(errno);
        }
        cond->list->first = NULL;
        cond->list->last  = NULL;
    }

    /** insere le thread courant dans la liste des threads en attente sur la condition */
    mthread_t self = mthread_self();
    mthread_insert_last(self, cond->list);

    /** bloque le thread */
    self->status = BLOCKED;

    /** enregistres le spinlock du mutex sur le processeur virtuel */
    mthread_virtual_processor_t * vp = mthread_get_vp();
    vp->p = &cond->lock;

    /** deverouille le mutex */
    mthread_mutex_unlock(mutex);

    /** passe au thread suivant en appelant l'ordonnanceur */
    mthread_log("COND_WAIT","Thread blocked\n");
    mthread_yield();

    /** reverouille le mutex avant de reprendre */
    mthread_mutex_lock(mutex);

    return 0;
}
