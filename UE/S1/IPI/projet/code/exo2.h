#ifndef EXO_2
# define EXO_2

# include "ipi.h"

typedef struct	s_node {
	BIT	visited;	/* vrai ou faux si la node a deja ete visité */
	INDEX	pathlen;
	size_t	pathw;		/* le poids total du chemin */
	INDEX	prev;		/* le sommet precedent dans le chemin */
}		t_node;


#endif
