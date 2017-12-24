#ifndef DEPTH_BREADTH_SEARCH_H
# define DEPTH_BREADTH_SEARCH_H

# include "ipi.h"
# include "bitmap.h"
# include "node.h"

/**
 *	@require : une bitmap representant les arcs, et un indice de sommet source
 *	@ensure  : parcours le graphe en largeur, et trouve le chemin le plus court
 *		   entre chaque sommet et le sommet source. Renvoie
 *		   un tableau de sommet correspondant au resultat
 *	@assign  : ---------------------------------
 */
t_array * depth_breadth_search(t_bitmap * arcs, INDEX s);

#endif
