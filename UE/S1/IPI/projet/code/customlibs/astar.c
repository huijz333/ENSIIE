# include "astar.h"

/** fonction interne qui compare 2 doubles (utile à la file de priorité) */
static int weightcmp(WEIGHT * a, WEIGHT * b) {
	if (*a < *b) {
		return (-1);
	}
	if (*a > *b) {
		return (1);
	}
	return (0);
}

/**
 *	@require : 	'nodes':	un tableau de sommet
 *			'heuristic':	la fonction d'heuristique
 *			'minw':		poids minimal souhaité
 *			's':		un indice sommet source
 *			't':		un indice de sommet de destination
 *	@ensure  : resout le plus court chemin dans le graphe entre 's' et 't'.
 *			Renvoie 1 si un chemin a été trouvé, 0 sinon.
 *	@assign  : 'nodes': les attributs des sommets peuvent être modifié
 */
int astar(t_array * nodes, t_heuristic heuristic, WEIGHT minw, INDEX sID, INDEX tID) {
	INDEX n = nodes->size;
	
	/** tableau enregistrant les sommets qui ont déjà eu un predecesseur visité */
	t_pqueue * unvisited = pqueue_new((t_cmpf)weightcmp);
	/** tableau enregistrant les sommets de la file */
	t_pqueue_node ** pqueue_nodes = (t_pqueue_node **) malloc(sizeof(t_pqueue_node *) * n);
	/** tableau enregistrant le poids avec l'heuristique appliqué */
	WEIGHT * h_cost = (WEIGHT *) malloc(sizeof(WEIGHT) * n);
	if (unvisited == NULL || pqueue_nodes == NULL || h_cost == NULL) {
		pqueue_delete(unvisited);
		free(pqueue_nodes);
		free(h_cost);
		return (0);
	}

	
	/** 1. INITIALISATION DE L'ALGORITHME */
	/** pour chaque sommets */
	INDEX i;
	for (i = 0 ; i < n ; i++) {
		/** on definit sa distance de 's' à '+oo' */
		t_nodew * node = (t_nodew *) array_get(nodes, i);
		node->super.pathlen = MAX_NODES;
		node->pathw= INF_WEIGHT;
		h_cost[i] = INF_WEIGHT;
		/** pas de position dans la file */
		pqueue_nodes[i] = NULL;
	}

	/** on initialise le sommet source */
	t_nodew * s = (t_nodew *) array_get(nodes, sID);
	s->pathw = 0; /* poids réel du chemin */
	s->super.pathlen = 0;
	h_cost[sID] = heuristic(nodes, sID, sID, sID, tID); /* poids heuristique */
	pqueue_nodes[sID] = pqueue_insert(unvisited, h_cost + sID, &sID);
	
	t_nodew * t = (t_nodew *) array_get(nodes, tID);
	
	/** 2. BOUCLE DE L'ALGORITHME A* */
	/** Tant qu'il y a des sommets a visité, on les visite */
	while (!pqueue_is_empty(unvisited)) {
		/** 2.1. : on cherche un noeud 'u' non visite minimisant d(u).
		  ceci est optimisé à l'aide d'une file de priorité */
		t_pqueue_node node = pqueue_pop(unvisited);
		INDEX	uID = *((INDEX *) node.value);
		pqueue_nodes[uID] = NULL;
		t_nodew	* u = (t_nodew *) array_get(nodes, uID);

		/** si on a atteint 't', ou si on est dans une autre partie connexe ... */
		if (t->pathw <= minw || u->pathw == INF_WEIGHT) {
			break ;
		}

		/** 2.2 : on minimise les chemins voisins de 'u' */
		/* pour chaque successeur de 'u' */
		ARRAY_ITERATE_START(u->super.successors, INDEX *, vIDref, i) {
			/** successeur 'v' de 'u' */
			INDEX	vID = *vIDref;
			t_nodew	* v = (t_nodew *) array_get(nodes, vID);

			/** on teste voir si le chemin de 's' à 't' passant
			    par 'u' et 'v' est plus court que le chemin
			    precedant enregistré passant par 'v' */

			/** poids de l'arc allant de 'u' à 'v' */
			WEIGHT w = *((WEIGHT *)array_get(u->ws, i));
			
			/** si ce nouveau chemin est de cout plus faible */
			if (u->pathw + w < v->pathw) {
				/* on ecrase le chemin precedant par le nouveau chemin */
				v->pathw = u->pathw + w;

				/** poids de la fonction d'heuristique */
				WEIGHT h = heuristic(nodes, uID, vID, sID, tID);
				h_cost[vID] = h;
				v->super.prev = uID;
				v->super.pathlen = u->super.pathlen + 1;
				/** on enregistre les sommets dans la file de priorité */
				if (pqueue_nodes[vID] == NULL) {
					pqueue_nodes[vID] = pqueue_insert(unvisited,
									h_cost + vID,
									vIDref);
				} else {
					pqueue_decrease(unvisited,
							pqueue_nodes[vID],
							h_cost + vID);
				}
			}
		}
		ARRAY_ITERATE_STOP(u->super.successors, INDEX *, vIDref, i);
	}

	pqueue_delete(unvisited);
	free(pqueue_nodes);
	free(h_cost);

	return (t->pathw <= minw);
}
