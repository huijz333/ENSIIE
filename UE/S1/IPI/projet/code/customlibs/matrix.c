#include "matrix.h"

/**
 *	@require : la taille de la matrice 'n'
 *	@ensure  : alloue une nouvelle matrice sur le tas, et la renvoie
 *	@assign  : la matrice est initialisé à 0
 */
t_matrix * matrix_new(size_t n) {
	t_matrix * matrix = (t_matrix *) malloc(sizeof(t_matrix) + n * n * sizeof(int));
	if (matrix == NULL) {
		return (NULL);
	}
	matrix->n = n;
	memset(matrix + 1, 0, n * n * sizeof(int));
	return (matrix);
}

/**
 *	@require : une matrice 'matrix', deux index (i, j)
 *	@ensure  : renvoie l'addresse de l'element (i, j) dans la matrice
 *	@assign  : ---
 */
int * matrix_addr(t_matrix * matrix, size_t i, size_t j) {
	int * values = (int *) (matrix + 1);
	return (values + matrix->n * j + i);
}

/**
 *	@require : une matrice 'matrix', deux index (i, j)
 *	@ensure  : renvoie la valeur de l'element (i, j) dans la matrice
 *	@assign  : ---
 */
int matrix_get(t_matrix * matrix, size_t i, size_t j) {
	return *(matrix_addr(matrix, i, j));
}

/**
 *	@require : ---
 *	@ensure  : lit une matrice carre sur l'entree standart.
 *			Le 1er entier lu est la dimension 'n',
 *			suivi de n * n valeurs, où la k-ieme valeur correspond
 *			a la ligne 'k / n' et a la colonne 'k % n'
 *
 *			 0    1   ...   k   ...   n
 *			n+1  n+2  ...  k+n  ...  2n-1
 *			          ........
 *			          ........
 *		        	  ........
 *			n(n-1)    ........	 n.n
 *
 *	@assign  : le graphe est initialement vide
*/
t_matrix * matrix_parse(void) {
	size_t n;
	scanf("%lu", &n);
	t_matrix * matrix = matrix_new(n);
	if (matrix == NULL) {
		return (NULL);
	}

	size_t i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			scanf("%d", matrix_addr(matrix, i, j));
		}
	}
	return (matrix);
}

/**
 *	@require : une matrice préallablement alloué via 'matrix_new()'
 *	@ensure  : libere la memoire utilise par la matrice du tas
 *	@assign  : ------
 */
void matrix_delete(t_matrix * matrix) {
	free(matrix);
}


