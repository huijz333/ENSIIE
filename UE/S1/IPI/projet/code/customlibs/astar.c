# include "astar.h"

/**
 *	@require : 	'nodes' :    	- le tableau des sommets
 *			'unvisited': 	- le tableau des sommets non visités
 *			'costs':    	- le tableau des poids totaux des chemins
 *				    	de 's' vers les sommets
 *			's':		- le sommet source
 *	@ensure  : initialise les arguments pour préparer l'algorithme de astar
 *	@assign  : 	'unvisited' est rempli de tous les sommets
 *			'costs' voit tous ses poids mis à '+oo'
 */
static void astar_init(t_array * nodes, t_array * toVisit, t_bitmap * visited,
			WEIGHT * costs, INDEX s) {
	/** pour chaque sommet */
	INDEX n = nodes->size;
	INDEX i;
	for (i = 0 ; i < n ; i++) {
		t_nodew * node = (t_nodew *) array_get(nodes, i);
		node->super.pathlen = 0;

		/** on definit sa distance de 's' à '+oo' */
		costs[i] = INF_WEIGHT;
	}
	
	/* on initialise le sommet 'source' */
	costs[s] = 0;
	array_add(toVisit, &s);
	bitmap_set(visited, s);
}

/**
 *	@require :	'unvisited': 	- le tableau des sommets non visités
 *			'costs':    	- le tableau des poids totaux des chemins
 *	@ensure  : renvoie l'indice dans le tableau 'unvisited' du
 /		   du prochain sommet à visiter
 *	@assign  : ----------------------------------------------------
 */
static INDEX astar_next_node(t_array * toVisit, WEIGHT * costs) {
	/* on prends le 1er sommet non visité */
	INDEX i = 0;
	INDEX uID = *((INDEX *) array_get(toVisit, i));

	/* pour chaque autre sommets non visités */
	ARRAY_ITERATE_START(toVisit, INDEX *, vIDref, j) {
		INDEX vID = *vIDref;
		/** si ce sommet est de poids plus faible */
		if (costs[vID] < costs[uID]) {
			uID = vID;
			i = j;
		}
	}
	ARRAY_ITERATE_STOP(toVisit, INDEX *, vIDref, j);
	return (i);
}

/**
 *	
 *	@require :	'nodes': 	- le tableau des sommets du graphe
 *			'costs':    	- le tableau des poids totaux des chemins
 *			'h':		- fonction heuristique
 *			'uID':		- le numéro du sommet que l'on visite
 *			's': 		- le numéro du sommet source
 *			't': 		- le numéro du sommet de destination
 *			
 *	@ensure  : applique un algorithme de propagation sur le sommet 'uID':
 *			pour chacun de ses successeurs, on essaye de construire
 *			un chemin plus court venant de 'u'
 *	@assign  : 'costs' peut être modifié au index des successeurs de 'u'
 */
static void astar_flood_fill(t_array * nodes, t_array * toVisit, t_bitmap * visited,
			WEIGHT * costs, t_heuristic h, INDEX uID, INDEX s, INDEX t) {
	t_nodew * u = (t_nodew *) array_get(nodes, uID);
	if (u->super.successors == NULL) {
		return ;
	}

	/* pour chaque successeur de 'u' */
	ARRAY_ITERATE_START(u->super.successors, INDEX *, vIDref, i) {
		/** successeur 'v' de 'u' */
		INDEX vID = *vIDref;
		/** si le sommet n'est pas dans 'toVisit' */
		if (!bitmap_get(visited, vID)) {
			/** on l'ajoute */
			bitmap_set(visited, vID);
			array_add(toVisit, &vID);
		}

		/** poids de l'arc allant de 'u' à 'v' */
		WEIGHT wcost = *((WEIGHT *)array_get(u->ws, i));
		/** poids de la fonction d'heuristique */
		WEIGHT hcost = h(nodes, uID, vID, s, t);
		/** nouveau cout final */
		WEIGHT ncost = costs[uID] + wcost + hcost;
		/** cout precedent de 'v' */
		WEIGHT vcost = costs[vID];
		/** si ce nouveau chemin est de cout plus faible */
		if (ncost < vcost) {
			/* on ecrase le chemin precedant par le nouveau chemin */
			costs[vID] = ncost;

			t_node * v = (t_node *) array_get(nodes, vID);
			v->prev = uID;
			v->pathlen = u->super.pathlen + 1;
		}
	}
	ARRAY_ITERATE_STOP(u->super.successors, INDEX *, vIDref, i);
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
int astar(t_array * nodes, t_heuristic h, INDEX s, INDEX t) {
	INDEX n = nodes->size;
	
	/** tableau enregistrant les sommets qui ont déjà eu un predecesseur visité */
	t_array * toVisit = array_new(n, sizeof(INDEX));
	/** bitmap enregistrant les sommets visités */
	t_bitmap * visited = bitmap_new(n);
	/** tableau enregistrant le cout total (poids + heuristiques) des chemins trouvés */
	WEIGHT * costs = (WEIGHT *) malloc(n * sizeof(WEIGHT));
	if (toVisit == NULL || visited == NULL || costs == NULL) {
		array_delete(toVisit);
		bitmap_delete(visited);
		free(costs);
		return (0);
	}

	/* initialisation des sommets */
	astar_init(nodes, toVisit, visited, costs, s);

	/* boucle algorithm a* */
	while (toVisit->size > 0) {
		/** on cherche un noeud 'u' non visite minimisant d(u) */
		INDEX i = astar_next_node(toVisit, costs);
		INDEX uID = *((INDEX *) array_get(toVisit, i));
		array_remove(toVisit, i);

		/* sinon, si on a atteint 't' */
		if (uID == t) {
			/**
			 *  deux posibilités: si le poids du chemin vaux '+oo',
			 *  c'est qu'il se situe dans une autre partie connexe du
			 *  graphe => pas de chemin
			 *  sinon, on a trouvé un chemin
			 */
			array_delete(toVisit);
			bitmap_delete(visited);
			WEIGHT cost = costs[uID];
			free(costs);
			return (cost == INF_WEIGHT ? 0 : 1);
		}

		/* on minimise les chemins des voisins de 'un' */
		astar_flood_fill(nodes, toVisit, visited, costs, h, uID, s, t);
	}
	bitmap_delete(visited);
	array_delete(toVisit);
	free(costs);
	return (0);
}
