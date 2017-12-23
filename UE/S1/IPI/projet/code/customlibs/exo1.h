#ifndef EXO_1
# define EXO_1

# include "ipi.h"	/* constantes */
# include "array.h"	/* file pour générer le chemin */

/**
 *	MAX_NODES	: nombre maximal de sommets.
 *	INDEX		: type a utilisé pour stocker un indice d'un sommet
 *	INDEX_IDENTIFIER: printf(), scanf() identifier pour le type INDEX
 */
# define MAX_NODES 		(50)
# define INDEX			BYTE
# define INDEX_IDENTIFIER 	"%hhu"

/**
 *	Represente un sommet du graphe
 */
typedef struct	s_node {
	INDEX	pathlen;/* nombre de sommets entre s et ce sommet */
	INDEX	prev;	/* element precedant le chemin (index du sommet) */
}		t_node;

/**
 *	Le graphe
 */
typedef struct 	s_graph {
	INDEX 	n; 	/* nombre de sommets */
	t_node 	* nodes;/* les sommets avec leur attributs */
	BYTE 	* arcs;	/* tableau representant les arcs (un arc est codde sur un bit) */
}		t_graph;

#endif
