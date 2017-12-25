#ifndef BREADTH_SEARCH_H
# define BREADTH_SEARCH_H

# include "ipi.h"
# include "bitmap.h"
# include "node.h"
# include "list.h"

/**
 *	@require : 	'nodes' : un tableau de structure 't_node' (ou en héritant)
 *		 	'arcs' : une bitmap representant les arcs
 *			's' : indice de sommet source
 *			't' : indice de sommet destination
 *	@ensure  : parcours le graphe en largeur, et trouve le chemin le plus court
 *		   entre chaque sommet et le sommet source
 *	@assign  : modifies le tableau 'nodes' en lui appliquant un algorithme
 *		   de parcours en largeur sur ses sommets
 */
void breadth_search(t_array * nodes, t_bitmap * arcs, INDEX s, INDEX t);

#endif
