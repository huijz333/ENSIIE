#include "mthread_internal.h"
#include <errno.h>

/* Create a key value identifying a location in the thread-specific
 data area.  Each thread maintains a distinct thread-specific data
 area.  DESTR_FUNCTION, if non-NULL, is called with the value
 associated to that key when the key is destroyed.
 DESTR_FUNCTION is not called if the value associated is NULL when
 the key is destroyed.  */
int mthread_key_create(mthread_key_t * userkey, void (*destr_f)(void *)) {

    /* récupères le thread courant */
    mthread_t mthread = mthread_self();

    /* si le tableau est plein */
    if (mthread->next_key == mthread->nb_keys) {

        /* alloue une case supplémentaire */
        struct mthread_s_key * keys = (struct mthread_s_key * )realloc(mthread->keys, sizeof(struct mthread_s_key) * (mthread->nb_keys + 1));
        if (keys == NULL) {
            /* pas assez de mémoire */
            return EAGAIN;
        }
        mthread->keys = keys;

        /* renvoie la clef à l'utilisateur */
        *userkey = mthread->nb_keys;

        /* augmente le nombre de clef dans le tableau */
        ++mthread->nb_keys;
        mthread->next_key = mthread->nb_keys;

        /* sinon, une clef est libre quelque part dans le tableau */
    } else {
        /* recupères la clef et la renvoie à l'utilisateur */
        *userkey = mthread->next_key;

        /* récupères l'index de la prochaine clef libre dans le tableau */
        mthread->next_key = (unsigned int)((unsigned long)(mthread->keys + mthread->next_key)->value);
    }

    /* copie le destructeur */
    (mthread->keys + *userkey)->destr_f = destr_f;

    return 0;
}

/* Destroy KEY.  */
int mthread_key_delete(mthread_key_t userkey) {
    /* récupères le thread courant */
    mthread_t mthread = mthread_self();

    /* si on supprime la dernière clef, on remet le thread dans l'état initial */
    if (mthread->nb_keys == 1) {
        mthread->nb_keys = 0;
        mthread->next_key = 0;
        free(mthread->keys);
        mthread->keys = NULL;
        return 0;
    }

    /* sinon */

    /* on enregistre ajoute cette clef à la liste des clefs libres */
    struct mthread_s_key * free_key = mthread->keys + userkey;
    free_key->value = (void *)((unsigned long)mthread->next_key);
    free_key->destr_f = NULL; /* N.B: on met cette fonction a NULL pour que le destructeur ne soit pas appelé par 'mthread_join()' */

    /* la clef supprimé devient libre, elle est mise en tête de liste */
    mthread->next_key = userkey;

    return 0;
}

/* Store POINTER in the thread-specific data slot identified by KEY. */
int mthread_setspecific(mthread_key_t key, const void * ptr) {
    /* trivial */
    (mthread_self()->keys + key)->value = (void *)ptr;
    return 0;
}

/* Return current value of the thread-specific data slot identified by KEY.  */
void * mthread_getspecific(mthread_key_t key) {
    /* trivial */
    return (mthread_self()->keys + key)->value;
}
