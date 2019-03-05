#include <errno.h>
#include "mthread_internal.h"

/* Functions for mutex handling.  */

/* Initialize MUTEX using attributes in *MUTEX_ATTR, or use the
   default values if later is NULL.  */
int
mthread_mutex_init (mthread_mutex_t * mutex,
                    const mthread_mutexattr_t * mutex_attr) {
    if (mutex_attr != NULL)
        not_implemented ();

    mutex->list 		= NULL;										/** MODIF */
    mutex->lock 		= 0;										/** MODIF */
    mutex->nb_thread  = 0;										/** MODIF */

    return 0;
}

/* Destroy MUTEX.  */
int
mthread_mutex_destroy (mthread_mutex_t * mutex) {
    if (mutex == NULL) {
        not_implemented ();
    }

    mthread_spinlock_lock(&mutex->lock);

    if (mutex->nb_thread != 0) {
        return EBUSY;
    }
    if (mutex->list) {
        free(mutex->list);
        mutex->list = NULL;
    }

    mthread_spinlock_unlock(&mutex->lock);

    mthread_log("MUTEX_DESTROY","MUTEX destroyed\n");
    return 0;
}

/* Try to lock MUTEX.  */
int
mthread_mutex_trylock (mthread_mutex_t * mutex) {
    /** si mutex NULL, retourne 'invalid argument' */
    if (mutex == NULL) {
        mthread_log("MUTEX_TRYLOCK","MUTEX NULL\n");
        return EINVAL;
    }

    /** prends le verrou */
    mthread_spinlock_lock(&mutex->lock);

    /** si le mutex a déjà été pris */
    if (mutex->nb_thread) {
        mthread_spinlock_unlock(&mutex->lock);
        mthread_log("MUTEX_TRYLOCK","MUTEX already locked\n");
        return EBUSY;
    }

    /** le thread prends le verrou */
    mutex->nb_thread = 1;
    mthread_spinlock_unlock(&mutex->lock);
    mthread_log("MUTEX_TRYLOCK","MUTEX acquired\n");

    return 0;
}

/* Wait until lock for MUTEX becomes available and lock it.  */
int
mthread_mutex_lock (mthread_mutex_t * mutex) {
    /** si mutex NULL, retourne 'invalid argument' */
    if (mutex == NULL) {
        mthread_log("MUTEX_LOCK","MUTEX NULL\n");
        return EINVAL;
    }

    /** prends le verrou */
    mthread_spinlock_lock(&mutex->lock);

    /** si le mutex a déjà été pris */
    if (mutex->nb_thread) {

        /** initialise la liste des threads si besoin */
        if (mutex->list == NULL) {
            mutex->list = malloc(sizeof(mthread_list_t));
            if (mutex->list == NULL) {
                perror("malloc");
                exit(errno);
            }
            mutex->list->first = NULL;
            mutex->list->last  = NULL;
        }

        /** insere le thread courant dans la liste des threads sur le mutex */
        mthread_t self = mthread_self();
        mthread_insert_last(self,mutex->list);

        /** bloque le thread */
        self->status = BLOCKED;

        /** enregistres le spinlock du mutex sur le processeur virtuel */
        mthread_virtual_processor_t * vp = mthread_get_vp();
        vp->p = &mutex->lock;

        /** passe au thread suivant en appelant l'ordonnanceur */
        mthread_yield();

        mthread_log("MUTEX_LOCK","MUTEX already locked\n");

        return EDEADLK;
    }

    /** le thread prends le verrou */
    mutex->nb_thread = 1;
    mthread_spinlock_unlock(&mutex->lock);
    mthread_log("MUTEX_LOCK","MUTEX acquired\n");
    return 0;
}

/* Unlock MUTEX.  */
int
mthread_mutex_unlock (mthread_mutex_t * mutex) {
    /** si mutex NULL, retourne 'invalid argument' */
    if (mutex == NULL) {
        mthread_log("MUTEX_UNLOCK","MUTEX NULL\n");
        return EINVAL;
    }

    /** prends le spinlock */
    mthread_spinlock_lock(&mutex->lock);

    /** si le verrou n'était pas pris */
    if (! mutex->nb_thread) {
        mthread_spinlock_unlock(&mutex->lock);
        mthread_log("MUTEX_UNLOCK","MUTEX wasnt locked\n");
        return EPERM;
    }

    /** sinon, s'il y a un autre thread en attente sur le mutex */
    if (mutex->list) {
        /** on le retire de la liste */
        mthread_t first = mthread_remove_first(mutex->list);

        /** si on a debloqué le dernier thread, on libère la liste en mémoire */
        if (mthread_is_empty(mutex->list)) {
            free(mutex->list);
            mutex->list = NULL;
        }

        /** on le debloque */
        first->status = RUNNING;

        /** ce thread devient prêt à l'execution */
        mthread_virtual_processor_t * vp = mthread_get_vp();
        mthread_insert_last(first, &(vp->ready_list));
    } else {
        mutex->nb_thread = 0;
    }

    mthread_spinlock_unlock(&mutex->lock);

    mthread_log("MUTEX_UNLOCK","MUTEX released\n");
    return 0;
}

