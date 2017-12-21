# include "array.h"

/**
 *	@require : la capacité de départ du tableau dynamique
 *	@ensure  : alloue en mémoire un tableau dynamique
 *	@assign  : ------------------
 */
t_array * array_new(unsigned int defaultCapacity) {
	t_array * array = (t_array *) malloc(sizeof(t_array));
	if (array == NULL) {
		/* pas assez de mémoire */
		return (NULL);
	}
	array->values = (int *) malloc(sizeof(int) * defaultCapacity);
	if (array->values == NULL) {
		/* pas assez de mémoire */
		free(array);
		return (NULL);
	}
	array->capacity = defaultCapacity;
	array->size = 0;
	return (array);
}

/**
 *	@require : 'array': tableau dynamique alloué via 'array_new()'
 *	@ensure  : désalloue de la mémoire le tableau 'array()'
 *	@assign  : ------------------
 */
void array_delete(t_array * array) {
	free(array);
}

/**
 *	@require : un tableau 'array' et un index
 *	@ensure  : renvoie la valeur à l'index 'index' du tableau
 *	@assign  : -----------
 */
int array_get(t_array * array, unsigned int index) {
	if (index >= array->size) {
		return (-1);
	}
	return (array->values[index]);
}

/**
 *	fonction interne
 *
 *	@require : un tableau 'array' et une valeur 'value'
 *	@ensure  : modifie la capacité du tableau
 *	@assign  : 'array->values' peut être modifié par 'realloc()'
 *
 */
static void array_grow(t_array * array, unsigned int capacity) {
	if (capacity == 0) {
		free(array->values);
		array->values = NULL;
	} else {
		array->values = realloc(array->values, capacity * sizeof(int));
	}
	array->capacity = capacity;
}


/**
 *	@require : un tableau 'array', un index, et une valeur 'value'
 *	@ensure  : ajoutes la valeur 'value' a l'index donnée dans le tableau
 *			renvoie -1 en cas d'erreur, sinon 0
 *	@assign  : grossit la capacité du tableau si necessaire
 */
int array_set(t_array * array, unsigned int index, int value) {
	if (index >= array->capacity) {
		array_grow(array, index + 1);
		if (index >= array->capacity) {
			/* pas assez de mémoire */
			return (-1);
		}
	}
	array->values[index] = value;
	if (index >= array->size) {
		array->size = index + 1;
	}
	return (0);
}

/**
 *	@require : un tableau 'array' et une valeur 'value'
 *	@ensure  : ajoutes la valeur 'value' en bout de tableau 'array'
 *	@assign  : modifie les valeurs du tableau
 */
int array_add(t_array * array, int value) {
	return (array_set(array, array->size, value));
}
