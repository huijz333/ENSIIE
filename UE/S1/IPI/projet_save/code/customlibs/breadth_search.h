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
 *			Renvoie 1 si un chemin a été trouvé, 0 sinon.
 *	@assign  : modifies le tableau 'nodes' en lui appliquant un algorithme
 *		   de parcours en largeur sur ses sommets
 */
int breadth_search(t_array * nodes, INDEX s, INDEX t);

#endif
