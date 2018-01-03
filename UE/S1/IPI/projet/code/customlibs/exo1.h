#ifndef EXO1_H
# define EXO1_H

# include "ipi.h"
# include "list.h"

/**
 *	Represente un sommet du graphe (exo 1)
 */
typedef struct	s_node {
	INDEX	pathlen;/* nombre de sommets entre s et ce sommet */
	INDEX	prev;	/* element precedant le chemin (index du sommet) */
	t_list	* successors; /* les successeurs de ce sommet */
}		t_node;

#endif
