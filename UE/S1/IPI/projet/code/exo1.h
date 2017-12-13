#ifndef EXO_1
# define EXO_1

# include "ipi.h"

typedef struct	s_node {
	INDEX	pathlen;		/* nombre de sommets entre s et ce sommet */
	INDEX	path[MAX_NODES - 1];	/* le chemin (index des sommets) */
}		t_node;

typedef struct 	s_graph {
	INDEX 	n; 		/* nombre de sommets */
	t_node 	* nodes; 	/* les sommets avec leur attributs */
	BYTE 	* arcs;		/* tableau representant les arcs (un arc est codde sur un bit) */
}				t_graph;


#endif
