#ifndef ARRAY_H
# define ARRAY_H

# include <stdio.h> /* printf */
# include <stdlib.h> /* malloc */
# include <string.h> /* memmove */

/**
 *	Structure de donnée: tableau dynamique
 *	utilisé ici comme des files
 */
typedef struct  s_array {
	int             * values;	/* les valeurs du tableau */
	unsigned int    capacity;	/* capacité mémoire du tableau 'values' */
	unsigned int    size;		/* nombre d'element dans le tableau 'values' */
}               t_array;

/**
 *	@require : la capacité de départ du tableau dynamique
 *	@ensure  : alloue en mémoire un tableau dynamique
 *	@assign  : ------------------
 */
t_array * array_new(unsigned int defaultCapacity);

/**
 *	@require : 'array': tableau dynamique alloué via 'array_new()'
 *	@ensure  : désalloue de la mémoire le tableau 'array()'
 *	@assign  : ------------------
 */
void array_delete(t_array * array);

/**
 *	@require : un tableau 'array' et un index
 *	@ensure  : renvoie la valeur à l'index 'index' du tableau
 *	@assign  : -----------
 */
int array_get(t_array * array, unsigned int index);

/**
 *	@require : un tableau 'array', un index, et une valeur 'value'
 *	@ensure  : ajoutes la valeur 'value' a l'index donnée dans le tableau
 *			renvoie -1 si erreur, 0 sinon
 *	@assign  : grossit la capacité du tableau si necessaire
 */
int array_set(t_array * array, unsigned int index, int value);

/**
 *	@require : un tableau 'array' et une valeur 'value'
 *	@ensure  : ajoutes la valeur 'value' en bout de tableau 'array'
 *	@assign  : modifie les valeurs du tableau
 */
int array_add(t_array * array, int value);

# endif
