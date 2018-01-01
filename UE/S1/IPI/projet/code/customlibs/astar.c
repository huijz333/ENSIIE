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
 *			's':		un indice sommet source
 *			't':		un indice de sommet de destination
 *	@ensure  : resout le plus court chemin dans le graphe entre 's' et 't'.
 *			Renvoie 1 si un chemin a été trouvé, 0 sinon.
 *	@assign  : 'nodes': les attributs des sommets peuvent être modifié
 */
int astar(t_array * nodes, t_heuristic heuristic, INDEX sID, INDEX tID) {
	INDEX n = nodes->size;
	
	/** tableau enregistrant les sommets qui ont déjà eu un predecesseur visité */
	t_pqueue * unvisited = pqueue_new((t_cmpf)weightcmp);
	/** bitmap enregistrant les sommets visités */
	t_bitmap * visited = bitmap_new(n);
	/** tableau enregistrant les sommets de la file */
	t_pqueue_node ** pqueue_nodes = (t_pqueue_node **) malloc(sizeof(t_pqueue_node *) * n);
	/** tableau enregistrant le poids avec l'heuristique appliqué */
	WEIGHT * pathwh = (WEIGHT *) malloc(sizeof(WEIGHT) * n);
	if (unvisited == NULL || visited == NULL || pqueue_nodes == NULL || pathwh == NULL) {
		pqueue_delete(unvisited);
		bitmap_delete(visited);
		free(pqueue_nodes);
		free(pathwh);
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
		pathwh[i] = INF_WEIGHT;
		/** pas de position dans la file */
		pqueue_nodes[i] = NULL;
	}

	/** on initialise le sommet source */
	t_nodew * s = (t_nodew *) array_get(nodes, sID);
	s->pathw = 0; /* poids réel du chemin */
	pathwh[sID] = heuristic(nodes, sID, sID, sID, tID); /* poids heuristique */
	s->super.pathlen = 0;
	pqueue_nodes[sID] = pqueue_insert(unvisited, pathwh + sID, &sID);

	/** 2. BOUCLE DE L'ALGORITHME A* */
	/** Tant qu'il y a des sommets a visité, on les visite */
	while (!pqueue_is_empty(unvisited)) {
		/** 2.1. : on cherche un noeud 'u' non visite minimisant d(u).
		  ceci est optimisé à l'aide d'une file de priorité */
		t_pqueue_node node = pqueue_pop(unvisited);
		INDEX	uID = *((INDEX *) node.value);
		t_nodew	* u = (t_nodew *) array_get(nodes, uID);
		
		/** si on a atteint 't', ou si on est dans une autre partie connexe ... */
		if (uID == tID || u->pathw == INF_WEIGHT) {
			break ;
		}

		/** 2.2 : on minimise les chemins voisins de 'u' */

		/* si 'u' n'a pas de voisins, on continue de vider la file */
		if (u->super.successors == NULL) {
			continue ;
		}

		/* sinon, pour chaque successeur de 'u' */
		ARRAY_ITERATE_START(u->super.successors, INDEX *, vIDref, i) {
			/** successeur 'v' de 'u' */
			INDEX	vID = *vIDref;
			t_nodew	* v = (t_nodew *) array_get(nodes, vID);

			/** si le sommet a déjà été visité, on passe au suivant */
			if (bitmap_get(visited, vID)) {
				continue ;
			}

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
				pathwh[vID] = u->pathw + w + h;
				v->super.prev = uID;
				v->super.pathlen = u->super.pathlen + 1;
				/** on enregistre les sommets dans la file de priorité */
				if (pqueue_nodes[vID] == NULL) {
					pqueue_nodes[vID] = pqueue_insert(unvisited,
									pathwh + vID,
									vIDref);
				} else {
					pqueue_decrease(unvisited,
							pqueue_nodes[vID],
							pathwh + vID);
				}
			}
		}
		ARRAY_ITERATE_STOP(u->super.successors, INDEX *, vIDref, i);

		/** on definit 'u' comme visité */
		bitmap_set(visited, uID);
	}

	bitmap_delete(visited);
	pqueue_delete(unvisited);
	free(pqueue_nodes);
	free(pathwh);

	t_nodew * t = (t_nodew *) array_get(nodes, tID);
	return (t->pathw == INF_WEIGHT ? 0 : 1);
}
