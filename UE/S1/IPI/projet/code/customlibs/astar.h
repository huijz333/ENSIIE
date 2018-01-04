#ifndef ASTAR_H
# define ASTAR_H

# include "lab.h"

/**
 *	@require : 	'lab':		le labyrinthe
 *			'sPos':		la position de départ
 *			'tPos':		la position d'arrivé
 *			'timer':	le temps maximal de résolution voulu
 *	@ensure  : resout le plus court chemin dans le graphe entre 's' et 't'.
 *		   Renvoie 1 et affiche le chemin, si un chemin suffisement
 *		   court a été trouvé, sinon renvoie 0 et 'Not connected' est affiché
 *	@assign  : --------------------------------------------
 */
int astar(t_lab * lab, t_pos sPos, t_pos tPos, WEIGHT timer);

#endif
