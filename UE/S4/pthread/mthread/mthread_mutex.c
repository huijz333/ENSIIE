#include <errno.h>
#include "mthread_internal.h"

  /* Functions for mutex handling.  */

  /* Initialize MUTEX using attributes in *MUTEX_ATTR, or use the
     default values if later is NULL.  */
int
mthread_mutex_init (mthread_mutex_t * __mutex,
		    const mthread_mutexattr_t * __mutex_attr)
{
    if (__mutex_attr != NULL) 
        not_implemented ();

    __mutex->list 		= NULL;										/** MODIF */
    __mutex->lock 		= 0;										/** MODIF */
    __mutex->nb_thread  = 0;										/** MODIF */

    return 0;
}

  /* Destroy MUTEX.  */
int
mthread_mutex_destroy (mthread_mutex_t * __mutex)
{
    if (__mutex == NULL) {
        not_implemented ();
    }

    mthread_spinlock_lock(&__mutex->lock);

    if (__mutex->nb_thread != 0) {
        return EBUSY;
    }
    if (__mutex->list) {
		free(__mutex->list);
		__mutex->list = NULL;
    }

    mthread_spinlock_unlock(&__mutex->lock);

    mthread_log("MUTEX_DESTROY","MUTEX destroyed\n");
    return 0;
}

  /* Try to lock MUTEX.  */
int
mthread_mutex_trylock (mthread_mutex_t * __mutex)
{
	/** si mutex NULL, retourne 'invalid argument' */
    if (__mutex == NULL) {
        mthread_log("MUTEX_TRYLOCK","MUTEX NULL\n");
        return EINVAL;
    }

    /** prends le verrou */
    mthread_spinlock_lock(&__mutex->lock);

    /** si le mutex a déjà été pris */
    if (__mutex->nb_thread) {
        mthread_spinlock_unlock(&__mutex->lock);
        mthread_log("MUTEX_TRYLOCK","MUTEX already locked\n");
    	return EBUSY;
    }

    /** le thread prends le verrou */
	__mutex->nb_thread = 1;
	mthread_spinlock_unlock(&__mutex->lock);
    mthread_log("MUTEX_TRYLOCK","MUTEX acquired\n");

    return 0;
}

  /* Wait until lock for MUTEX becomes available and lock it.  */
int
mthread_mutex_lock (mthread_mutex_t * __mutex)
{
	/** si mutex NULL, retourne 'invalid argument' */
    if (__mutex == NULL) {
        mthread_log("MUTEX_LOCK","MUTEX NULL\n");
        return EINVAL;
    }

    /** prends le verrou */
    mthread_spinlock_lock(&__mutex->lock);

    /** si le mutex a déjà été pris */
    if (__mutex->nb_thread) {

        /** initialise la liste des threads si besoin */
        if (__mutex->list == NULL) {
            __mutex->list = malloc(sizeof(mthread_list_t));
            if (__mutex->list == NULL) {
    			perror("malloc");
    			exit(errno);
            }
            __mutex->list->first = NULL;
            __mutex->list->last  = NULL;
        }

        /** insere le thread courant dans la liste des threads sur le mutex */
    	mthread_t self = mthread_self();
        mthread_insert_last(self,__mutex->list);

        /** bloque le thread */
        self->status = BLOCKED;

        /** enregistres le spinlock du mutex sur le processeur virtuel */
        mthread_virtual_processor_t * vp = mthread_get_vp();
        vp->p = &__mutex->lock;

        /** passe au thread suivant en appelant l'ordonnanceur */
        mthread_yield();

        mthread_log("MUTEX_LOCK","MUTEX already locked\n");

        return EDEADLK;
    }

    /** le thread prends le verrou */
	__mutex->nb_thread = 1;
	mthread_spinlock_unlock(&__mutex->lock);
    mthread_log("MUTEX_LOCK","MUTEX acquired\n");
    return 0;
}

  /* Unlock MUTEX.  */
int
mthread_mutex_unlock (mthread_mutex_t * __mutex)
{
	/** si mutex NULL, retourne 'invalid argument' */
    if (__mutex == NULL) {
        mthread_log("MUTEX_UNLOCK","MUTEX NULL\n");
        return EINVAL;
    }

    /** prends le spinlock */
    mthread_spinlock_lock(&__mutex->lock);

    /** si le verrou n'était pas pris */
    if (! __mutex->nb_thread) {
        mthread_spinlock_unlock(&__mutex->lock);
        mthread_log("MUTEX_UNLOCK","MUTEX wasnt locked\n");
        return EPERM;
    }

    /** sinon, s'il y a un autre thread en attente sur le mutex */
	if (__mutex->list) {
		/** on le retire de la liste */
		mthread_t first = mthread_remove_first(__mutex->list);

		/** si on a debloqué le dernier thread, on libère la liste en mémoire */
		if (mthread_is_empty(__mutex->list)) {
			free(__mutex->list);
			__mutex->list = NULL;
		}

		/** on le debloque */
		first->status = RUNNING;

		/** ce thread devient prêt à l'execution */
		mthread_virtual_processor_t * vp = mthread_get_vp();
		mthread_insert_last(first,&(vp->ready_list));
	} else {
        __mutex->nb_thread = 0;
	}

    mthread_spinlock_unlock(&__mutex->lock);
    
    mthread_log("MUTEX_UNLOCK","MUTEX released\n");
    return 0;
}

