#ifndef BREADTH_SEARCH_H
# define BREADTH_SEARCH_H

# include "ipi.h"
# include "bitmap.h"
# include "node.h"

/**
 *	@require : 	'nodes' : un tableau de structure 't_node' (ou en héritant)
 *		 	'arcs' : une bitmap representant les arcs
 *			's' : indice de sommet source
 *	@ensure  : parcours le graphe en profondeur, et trouve le chemin le plus court
 *		   entre chaque sommet et le sommet source
 *	@assign  : modifies le tableau 'nodes' en lui appliquant un algorithme
 *		   de parcours en profondeur sur ses sommets
 */
void depth_search(t_array * nodes, t_bitmap * arcs, INDEX s);

#endif
