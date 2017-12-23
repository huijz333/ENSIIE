#ifndef EXO_2
# define EXO_2

# include "ipi.h"	/* constantes */
# include "matrix.h"	/* matrice representant les arcs */
# include "array.h"	/* files pour générer le chemin */

/**
 *	MAX_NODES	: nombre maximal de sommets.
 *	INDEX		: type a utilisé pour stocker un indice d'un sommet
 *	INDEX_IDENTIFIER: printf(), scanf() identifier pour le type INDEX
 */
# define MAX_NODES 		(50)
# define INDEX			BYTE
# define INDEX_IDENTIFIER 	"%hhu"

/**
 *	represente un sommet dans le graphe
 */
typedef struct	s_node {
	BIT	visited;	/* vrai ou faux si la node a deja ete visité */
	INDEX	pathlen;
	size_t	pathw;		/* le poids total du chemin */
	INDEX	prev;		/* le sommet precedent dans le chemin */
}		t_node;

#endif
