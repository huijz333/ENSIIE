# include "customlibs/exo2.h"

/**
 *	@require : une matrice representant les arcs et leur poids, et les sommets du graphe
 *	@ensure  : renvoie l'index du sommet non visité avec le nouveau chemin de poids minimum.
 *			si le chemin n'existe pas, MAX_NODES est renvoyé
 *				=> 't' et 's' ne sont pas sur la meme partie connexe
 *	@assign  : ------
 */
static INDEX dijkstra_next_nodew(t_matrix * ws, t_array * nodes) {
	INDEX u = MAX_NODES;
	INDEX i;
	/* pour chaque sommet ... */
	for (i = 0 ; i < ws->n ; i++) {
		t_nodew * node = (t_nodew *) array_get(nodes, i);
		if (node->visited) {
			continue ;
		}
		/*
		 * si l'on trouve un 1er sommet non visité,
		 *   OU
		 * si on trouve un sommet non visité de poids plus faible
		 */
		if (u == MAX_NODES || node->pathw < ((t_nodew *) array_get(nodes, u))->pathw) {
			u = i;
		}
	}

	/* si 'u' == MAX_NODES, alors tous les sommets sont déjà visité.
	 *	OU
	 * si 'u' n'est pas dans la meme partie connexe que 's'
	 *	ALORS
	 * erreur, non connecté
	 */
	if (u == MAX_NODES || ((t_nodew *) array_get(nodes, u))->super.pathlen == 0) {
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
	empty.visited = 0;
	empty.pathw = UINT_MAX;
	empty.super.pathlen = 0;
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
	t_array * nodes = dijkstra_init(ws->n, s);
	if (nodes == NULL) {
		return (NULL);
	}

	/* boucle algorithm dijsktra */
	while (1) {
		/** on cherche un noeud 'u' non visite minimisant d(u) */
		INDEX u = dijkstra_next_nodew(ws, nodes);
		if (u == MAX_NODES) {
			array_delete(nodes);
			return (NULL);
		}

		/* si on a atteint t, on affiche le resultat */
		if (u == t) {
			return (nodes);
		}

		/* on definit 'u' comme visite */
		t_nodew * node = (t_nodew *) array_get(nodes, u);
		node->visited = 1;
		
		/* pour chaque sommet */
		INDEX v;
		for (v = 0 ; v < ws->n ; v++) {
			/** s'il est voisin de 'u' ET si ce nouveau
			 *  chemin passant par 'u' est plus court */
			int wuv = matrix_get(ws, u, v);
			t_nodew * neighbor = (t_nodew *) array_get(nodes, v);
			if (wuv >= 0 && node->pathw + wuv < neighbor->pathw) {
				/* on cree le nouveau chemin */
				neighbor->pathw = node->pathw + wuv;
				neighbor->super.prev = u;
				neighbor->super.pathlen = node->super.pathlen + 1;
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
