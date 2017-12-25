# include "dijkstra.h"

/**
 *	@require : les sommets du graphe, et le tableau des sommets déjà visité
 *	@ensure  : renvoie l'index du sommet non visité avec le nouveau chemin de poids minimum.
 *			si le chemin n'existe pas, MAX_NODES est renvoyé
 *				=> 't' et 's' ne sont pas sur la meme partie connexe
 *	@assign  : ------
 */
static INDEX dijkstra_next_nodew(t_array * nodes, t_bitmap * visited) {
	INDEX n = nodes->size;
	INDEX u = MAX_NODES;
	INDEX v;
	/* pour chaque sommet ... */
	for (v = 0 ; v < n ; v++) {
		if (bitmap_get(visited, v)) {
			continue ;
		}
		/*
		 * si l'on trouve un 1er sommet non visité,
		 *   OU
		 * si on trouve un sommet non visité de poids plus faible
		 */
		t_nodew * un = (t_nodew *) array_get(nodes, u);
		t_nodew * vn = (t_nodew *) array_get(nodes, v);
		if (u == MAX_NODES || vn->pathw < un->pathw) {
			u = v;
		}
	}

	/* si 'u' == MAX_NODES, alors tous les sommets sont déjà visité.
	 *	OU
	 * si 'u' n'est pas dans la meme partie connexe que 's'
	 *	ALORS
	 * erreur, non connecté
	 */
	t_node * un = (t_node *) array_get(nodes, u);
	if (u == MAX_NODES || un->pathlen == 0) {
		return (MAX_NODES);
	}
	return (u);
}

/**
 *	@require : le nombre de sommet 'n' et le sommet source 's'
 *	@ensure  : initialise et renvoie un tableau de 't_nodew' prêt à être
 *			utilisé dans l'algorithme de dijkstra
 *	@assign  : --------------
 */
static void dijkstra_init(t_array * nodes, INDEX s) {
	/** on definit toutes les distances de 's' sur '+oo' */
	INDEX n = nodes->size;
	INDEX i;
	for (i = 0 ; i < n ; i++) {
		t_nodew * node = (t_nodew *) array_get(nodes, i);
		node->super.pathlen = 0;
		node->pathw = UINT_MAX;
	}
	
	/* on initialise le sommet 'source' */
	t_nodew * source = (t_nodew *) array_get(nodes, s);
	source->super.pathlen = 1;
	source->super.prev =  s;
	source->pathw = 0;
}

/**
 *	@require : une matrice representant les arcs et leur poids, deux indices s et t
 *	@ensure  : resout le plus court chemin dans le graphe entre 's' et 't'.
 *			Renvoie 1 si un chemin a été trouvé, 0 sinon.
 *	@assign  : --------------
 */
int dijkstra(t_array * nodes, t_matrix * ws, INDEX s, INDEX t) {
	/* initialisation des sommets */
	INDEX n = nodes->size;
	dijkstra_init(nodes, s);

	/* tableau enregistrant les sommets visité */
	t_bitmap * visited = bitmap_new(n);
	if (visited == NULL) {
		return (0);
	}

	/* boucle algorithm dijsktra */
	while (1) {
		/** on cherche un noeud 'u' non visite minimisant d(u) */
		INDEX u = dijkstra_next_nodew(nodes, visited);
		
		/** s'il n'y a plus de tel noeud, alors pas de chemin entre 's' et 't' */
		if (u == MAX_NODES) {
			bitmap_delete(visited);
			return (0);
		}

		/* sinon, si on a atteint 't', alors on a trouvé le chemin, fin de la boucle */
		if (u == t) {
			bitmap_delete(visited);
			return (1);
		}

		/* on definit 'u' comme visite */
		bitmap_set(visited, u);
		
		/* pour chaque sommet */
		t_nodew * un = (t_nodew *) array_get(nodes, u);
		INDEX v;
		for (v = 0 ; v < ws->n ; v++) {
			/** s'il est voisin de 'u' ET si ce nouveau
			 *  chemin passant par 'u' est plus court */
			int wuv = matrix_get(ws, u, v);
			t_nodew * vn = (t_nodew *) array_get(nodes, v);
			if (wuv >= 0 && un->pathw + wuv < vn->pathw) {
				/* on cree le nouveau chemin */
				vn->pathw = un->pathw + wuv;
				vn->super.prev = u;
				vn->super.pathlen = un->super.pathlen + 1;
			}
		}
	}
	/** on n'arrive théoriquement jamais ici */
	bitmap_delete(visited);
	return (0);
}
