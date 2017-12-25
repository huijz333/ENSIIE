#ifndef QUEUE_H
# define QUEUE_H

/* independant du reste du projet */

# include "array.h"

# ifndef BYTE
#	define BYTE unsigned char
# endif

/**
 *	Structure de donnée: file dynamique
 */
typedef struct  s_queue {
	t_array		* values;	/* les valeurs du tableau */
	unsigned int    head;		/* l'élément en tête de file */
}               t_queue;

/**
 *	@require : la capacité maximal de départ de la file
 *	@ensure  : alloue en mémoire une file dynamique
 *	@assign  : ------------------
 */
t_queue * queue_new(unsigned int defaultCapacity, unsigned int elemSize);

/**
 *	@require : 'queue': file dynamique alloué via 'queue_new()'
 *	@ensure  : désalloue la mémoire de la file 'queue'
 *	@assign  : ------------------
 */
void queue_delete(t_queue * queue);

/**
 *	@require : une file 'queue'
 *	@ensure  : renvoie et supprime la valeur en tête de file
 *			ou NULL si erreur
 *	@assign  : -----------------
 */
void * queue_pop(t_queue * queue);

/**
 *	@require : un tableau 'array', un index, et une valeur 'value'
 *	@ensure  : ajoutes la valeur 'value' a l'index donnée dans le tableau
 *			renvoie -1 si erreur, sinon l'index ou l'élément a été inséré
 *	@assign  : grossit la capacité du tableau si necessaire
 */
void queue_push(t_queue * queue);

# endif
