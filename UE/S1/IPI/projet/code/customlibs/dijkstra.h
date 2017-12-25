#ifndef DIJKSTRA_H
# define DIJKSTRA_H

# include "ipi.h"
# include "array.h"
# include "node.h"
# include "matrix.h"
# include "bitmap.h"

/**
 *	@require : une matrice representant les arcs et leur poids, deux indices s et t
 *	@ensure  : resout le plus court chemin dans le graphe entre 's' et 't'.
 *			Renvoie 1 si un chemin a été trouvé, 0 sinon.
 *	@assign  : --------------
 */
int dijkstra(t_array * nodes, t_matrix * ws, INDEX s, INDEX t);

#endif
