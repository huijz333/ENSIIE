# include "astar.h"

/** 2 fonctions d'heuristique pré-implémenté */
WEIGHT heuristic_zero(t_array * nodes, INDEX uID, INDEX vID, INDEX sID, INDEX tID) {
	(void)nodes;
	(void)uID;
	(void)vID;
	(void)sID;
	(void)tID;
	return (0);
}

WEIGHT heuristic_euclidian(t_array * nodes, INDEX uID, INDEX vID, INDEX sID, INDEX tID) {
	(void)vID;
	(void)tID;
	t_nodel * v = (t_nodel *) array_get(nodes, vID);
	t_nodel * t = (t_nodel *) array_get(nodes, tID);
	return (t->x - v->x + t->y - v->y);
}

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
 *			'h':		la fonction d'heuristique
 *			's':		un indice sommet source
 *			't':		un indice de sommet de destination
 *	@ensure  : resout le plus court chemin dans le graphe entre 's' et 't'.
 *			Renvoie 1 si un chemin a été trouvé, 0 sinon.
 *	@assign  : 'nodes': les attributs des sommets peuvent être modifié
 */
int astar(t_array * nodes, t_heuristic h, INDEX sID, INDEX tID) {
	INDEX n = nodes->size;
	
	/** tableau enregistrant les sommets qui ont déjà eu un predecesseur visité */
	t_pqueue * unvisited = pqueue_new((t_cmpf)weightcmp);
	/** bitmap enregistrant les sommets visités */
	t_bitmap * visited = bitmap_new(n);
	/** tableau enregistrant les sommets de la file */
	t_pqueue_node ** pqueue_nodes = (t_pqueue_node **) malloc(sizeof(t_pqueue_node *) * n);
	if (unvisited == NULL || visited == NULL || pqueue_nodes == NULL) {
		pqueue_delete(unvisited);
		bitmap_delete(visited);
		free(pqueue_nodes);
		return (0);
	}

	/** pour chaque sommet */
	INDEX i;
	for (i = 0 ; i < n ; i++) {
		/** on definit sa distance de 's' à '+oo' */
		t_nodew * node = (t_nodew *) array_get(nodes, i);
		node->super.pathlen = 0;
		node->pathw = i == sID ? 0 : INF_WEIGHT;
		int * iRef = (int *) malloc(sizeof(int));
		*iRef = i;
		pqueue_nodes[i] = pqueue_insert(unvisited, &(node->pathw), iRef);
	}

	/* boucle algorithm a* */
	while (!pqueue_is_empty(unvisited)) {
		/** on cherche un noeud 'u' non visite minimisant d(u) */
		t_pqueue_node node = pqueue_pop(unvisited);
		INDEX	uID = *((INDEX *) node.value);
		t_nodew	* u = (t_nodew *) array_get(nodes, uID);
		bitmap_set(visited, uID);
		
		/** si on est dans une autre partie connexe ... */
		if (uID == tID || u->pathw == INF_WEIGHT) {
			break ;
		}

		/* on minimise les chemins des voisins de 'u' */
		if (u->super.successors == NULL) {
			continue ;
		}

		/* pour chaque successeur de 'u' */
		ARRAY_ITERATE_START(u->super.successors, INDEX *, vIDref, i) {
			/** successeur 'v' de 'u' */
			INDEX	vID = *vIDref;
			t_nodew	* v = (t_nodew *) array_get(nodes, vID);

			/** si le sommet n'est pas dans la file d'attente de visite */
			if (bitmap_get(visited, vID)) {
				continue ;
			}

			/** poids de l'arc allant de 'u' à 'v' */
			WEIGHT wcost = *((WEIGHT *)array_get(u->ws, i));
			/** poids de la fonction d'heuristique */
			WEIGHT hcost = h(nodes, uID, vID, sID, tID);
			/** nouveau cout final */
			WEIGHT ncost = u->pathw + wcost + hcost;
			/** cout precedent de 'v' */
			WEIGHT vcost = v->pathw;
			/** si ce nouveau chemin est de cout plus faible */
			if (ncost < vcost) {
				/* on ecrase le chemin precedant par le nouveau chemin */
				v->pathw = ncost;
				v->super.prev = uID;
				v->super.pathlen = u->super.pathlen + 1;
				pqueue_decrease(unvisited, pqueue_nodes[vID], &(v->pathw));
			}
		}
		ARRAY_ITERATE_STOP(u->super.successors, INDEX *, vIDref, i);
	}

	bitmap_delete(visited);
	pqueue_delete(unvisited);
	free(pqueue_nodes);
	t_nodew * t = (t_nodew *) array_get(nodes, tID);
	return (t->pathw == INF_WEIGHT ? 0 : 1);
}
