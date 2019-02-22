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
																	/** MODIF */
    __mutex->list 		= NULL;										/** MODIF */
    __mutex->lock 		= 0;										/** MODIF */
    __mutex->nb_thread  = 0;										/** MODIF */

    fprintf(stderr, "[MUTEX_INIT] MUTEX initialized\n");

    return 0;
}

  /* Destroy MUTEX.  */
int
mthread_mutex_destroy (mthread_mutex_t * __mutex)
{
    if (__mutex == NULL) 
        not_implemented ();

    mthread_spinlock_lock(&__mutex->lock);
    if (__mutex->nb_thread != 0)
        return EBUSY;
    free(__mutex->list);
    __mutex->list = NULL;
    mthread_spinlock_unlock(&__mutex->lock);

    mthread_log("MUTEX_INIT","MUTEX destroyed\n");
    return 0;
}

  /* Try to lock MUTEX.  */
int
mthread_mutex_trylock (mthread_mutex_t * __mutex)
{
  not_implemented ();
  return 0;
}

  /* Wait until lock for MUTEX becomes available and lock it.  */
int
mthread_mutex_lock (mthread_mutex_t * __mutex)
{
    int retval = EINVAL;
    mthread_t self;
    mthread_virtual_processor_t *vp;

    if (__mutex == NULL) 
        return retval;

    mthread_spinlock_lock(&__mutex->lock);

    if (__mutex->nb_thread == 0) {
        __mutex->nb_thread = 1;
        mthread_spinlock_unlock(&__mutex->lock);
    } else {
        self = mthread_self();

        /** si la liste des threads bloqués est NULL, on l'initialise */
        if (__mutex->list == NULL) {							/** AJOUT */
            __mutex->list = malloc(sizeof(mthread_list_t));		/** AJOUT */
            if (__mutex->list == NULL) {						/** AJOUT */
    			perror("malloc");								/** AJOUT */
    			exit(errno);									/** AJOUT */
            }													/** AJOUT */
            __mutex->list->first = NULL;						/** AJOUT */
            __mutex->list->last  = NULL;						/** AJOUT */
        }														/** AJOUT */

        mthread_insert_last(self,__mutex->list);
        self->status = BLOCKED;
        vp = mthread_get_vp();
        vp->p = &__mutex->lock;
        mthread_yield();
    }


    mthread_log("MUTEX_LOCK","MUTEX acquired\n");
    return retval;
}

  /* Unlock MUTEX.  */
int
mthread_mutex_unlock (mthread_mutex_t * __mutex)
{
    int retval = EINVAL;
    mthread_t first;
    mthread_virtual_processor_t *vp;

    if (__mutex == NULL) 
        return retval;

    mthread_spinlock_lock(&__mutex->lock);
    if (__mutex->list && __mutex->list->first != NULL) {		/** MODIF */
        first = mthread_remove_first(__mutex->list);
        vp = mthread_get_vp();
        first->status = RUNNING;
        mthread_insert_last(first,&(vp->ready_list));
    } else {
        __mutex->nb_thread = 0;

        /** la liste des threads bloqués est vide, elle peut être dé-alloué */
        free(__mutex->list);									/** AJOUT */
        __mutex->list = NULL;									/** AJOUT */
    }


    mthread_spinlock_unlock(&__mutex->lock);
    
    mthread_log("MUTEX_UNLOCK","MUTEX released\n");
    return retval;
}
