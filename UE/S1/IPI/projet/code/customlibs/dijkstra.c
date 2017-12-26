# include "dijkstra.h"

/**
 *	@require : 	'nodes':	un tableau de sommet
 *			's':		un indice sommet source
 *			't':		un indice de sommet de destination
 *	@ensure  : resout le plus court chemin dans le graphe entre 's' et 't'.
 *			Renvoie 1 si un chemin a été trouvé, 0 sinon.
 *	@assign  : 'nodes': les attributs des sommets peuvent être modifié
 */
int dijkstra(t_array * nodes, INDEX s, INDEX t) {
	return (astar(nodes, heuristic_zero, s, t));
}
