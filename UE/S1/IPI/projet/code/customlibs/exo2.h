#ifndef EXO2_H
# define EXO2_H

# include "ipi.h"	/* constantes */
# include "pqueue.h"
# include "list.h"

/**
 *	Represente un arc
 */
typedef struct	s_arc {
	INDEX	to;
	WEIGHT	cost;
}		t_arc;

/**
 *	Represente un sommet du graphe (exo 2)
 */
typedef struct	s_node {
	INDEX	prev;	/** element precedant le chemin (index du sommet) */
	INDEX	cost;	/** poids du chemin entre s et ce sommet */
	t_list	* arcs;	/** arcs */
}		t_node;

#endif
