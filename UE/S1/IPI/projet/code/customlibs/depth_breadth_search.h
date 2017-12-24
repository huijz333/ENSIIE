#ifndef DEPTH_BREADTH_SEARCH_H
# define DEPTH_BREADTH_SEARCH_H

# include "ipi.h"
# include "bitmap.h"

/**
 *	Represente un sommet du graphe
 */
typedef struct	s_node {
	INDEX	pathlen;/* nombre de sommets entre s et ce sommet */
	INDEX	prev;	/* element precedant le chemin (index du sommet) */
}		t_node;


/**
 *	@require : une bitmap representant les arcs, et un indice de sommet source
 *	@ensure  : parcours le graphe en largeur, et trouve le chemin le plus court
 *		   entre chaque sommet et le sommet source. Renvoie
 *		   un tableau de sommet correspondant au resultat
 *	@assign  : ---------------------------------
 */
t_node * depth_breadth_search(t_bitmap * arcs, INDEX s);

#endif
