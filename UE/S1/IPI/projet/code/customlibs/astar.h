#ifndef ASTAR_H
# define ASTAR_H

# include "ipi.h"
# include "array.h"
# include "node.h"
# include "bitmap.h"
# include "pqueue.h"

/**
 *	fonction d'heuristique: renvoie un poids heuristique lorsque
 *	l'on souhaite passer des sommets 'u' à 'v' dans les sommets 'nodes',
 *	en sachant que l'on part de 's' et que l'on souhaite aller à 't'
 */
typedef WEIGHT (*t_heuristic)(t_array * nodes, INDEX u, INDEX v, INDEX s, INDEX t);

/**
 *	@require : 	'nodes':	un tableau de sommet
 *			'heuristic':	la fonction d'heuristique
 *			's':		un indice sommet source
 *			't':		un indice de sommet de destination
 *	@ensure  : resout le plus court chemin dans le graphe entre 's' et 't'.
 *			Renvoie 1 si un chemin a été trouvé, 0 sinon.
 *	@assign  : 'nodes': les attributs des sommets peuvent être modifié
 */
int astar(t_array * nodes, t_heuristic heuristic, INDEX sID, INDEX tID);

#endif
