# include "customlibs/exo2.h"

/**
 *	@require : une matrice representant les arcs et leur poids, les sommets du graphe,
 *			et le tableau des sommets déjà visité
 *	@ensure  : renvoie l'index du sommet non visité avec le nouveau chemin de poids minimum.
 *			si le chemin n'existe pas, MAX_NODES est renvoyé
 *				=> 't' et 's' ne sont pas sur la meme partie connexe
 *	@assign  : ------
 */
static INDEX dijkstra_next_nodew(t_matrix * ws, t_array * nodes, t_bitmap * visited) {
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
static t_array * dijkstra_init(INDEX n, INDEX s) {
	/* initialisation des sommets */
	t_array * nodes = array_new(n, sizeof(t_nodew));
	if (nodes == NULL) {
		return (NULL);
	}
	
	/* on ajoute 'n' sommet 'vide' (<=> non visité) */
	t_nodew empty;
	empty.super.pathlen = 0;
	empty.pathw = UINT_MAX;
	array_addn(nodes, &empty, n);

	/* on initialise le sommet 'source' */
	t_nodew * source = (t_nodew *) array_get(nodes, s);
	source->super.pathlen = 1;
	source->super.prev =  s;
	source->pathw = 0;

	return (nodes);
}

/**
 *	@require : une matrice representant les arcs et leur poids, deux indices s et t
 *	@ensure  : resout les plus courts chemin dans le graphe, et renvoie
 *			les sommets résolus dans un tableau de 't_nodew'
 *	@assign  : --------------
 */
static t_array * dijkstra(t_matrix * ws, INDEX s, INDEX t) {
	/* initialisation des sommets */
	INDEX n = ws->n;
	t_array * nodes = dijkstra_init(n, s);
	if (nodes == NULL) {
		return (NULL);
	}

	t_bitmap * visited = bitmap_new(n);
	if (visited == NULL) {
		array_delete(nodes);
		return (NULL);
	}

	/* boucle algorithm dijsktra */
	while (1) {
		/** on cherche un noeud 'u' non visite minimisant d(u) */
		INDEX u = dijkstra_next_nodew(ws, nodes, visited);
		if (u == MAX_NODES) {
			array_delete(nodes);
			return (NULL);
		}

		/* si on a atteint t, on affiche le resultat */
		if (u == t) {
			return (nodes);
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
	/* on n'arrive théoriquement jamais ici */
	array_delete(nodes);
	return (NULL);
}

int main(void) {
	/* Matrix parsing */
	t_matrix * ws = matrix_parse();
	if (ws == NULL) {
		fprintf(stderr, "Allocation or parsing failed.\n");
		return (EXIT_FAILURE);
	}
	
	/* argument du parcours en profondeur */
	INDEX s, t;
	scanf(INDEX_IDENTIFIER, &s);
	scanf(INDEX_IDENTIFIER, &t);
	--s;
	--t;

	/* faire le parcours en profondeur */
	t_array * nodes = dijkstra(ws, s, t);
	node_solve_path(nodes, s, t, stdout);
	array_delete(nodes);

	/* libere la mémoire */
	matrix_delete(ws);

	return (EXIT_SUCCESS);
}
