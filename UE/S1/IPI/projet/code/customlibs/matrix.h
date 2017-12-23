#ifndef MATRIX_H
# define MATRIX_H

# include "ipi.h"

typedef struct	s_matrix {
	size_t	n; /* matrices carré n * n */
	/* les valeurs sont alloués dans un 'tableau 1D' juste apres la structure */
}		t_matrix;

/**
 *	@require : la taille de la matrice 'n'
 *	@ensure  : alloue une nouvelle matrice sur le tas, et la renvoie
 *	@assign  : la matrice est initialisé à 0
 */
t_matrix * matrix_new(size_t n);

/**
 *	@require : une matrice 'matrix', deux index (i, j)
 *	@ensure  : renvoie l'addresse de l'element (i, j) dans la matrice
 *	@assign  : ---
 */
int * matrix_addr(t_matrix * matrix, size_t i, size_t j);

/**
 *	@require : une matrice 'matrix', deux index (i, j)
 *	@ensure  : renvoie la valeur de l'element (i, j) dans la matrice
 *	@assign  : ---
 */
int matrix_get(t_matrix * matrix, size_t i, size_t j);

/**
 *	@require : une matrice 'matrix', deux index (i, j)
 *	@ensure  : renvoie la valeur de l'element (i, j) dans la matrice
 *	@assign  : ---
 */
int matrix_get(t_matrix * matrix, size_t i, size_t j);

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
t_matrix * matrix_parse(void);

/**
 *	@require : une matrice préallablement alloué via 'matrix_new()'
 *	@ensure  : libere la memoire utilise par la matrice du tas
 *	@assign  : ------
 */
void matrix_delete(t_matrix * matrix);

#endif
