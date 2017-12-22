# include "customlibs/exo2.h"

/**
 *	@require : une matrice representant les arcs et leur poids, et les sommets
 *	@ensure  : renvoie l'index du sommet non visité avec le chemin de poids minimum
 *			si le chemin n'existe pas, MAX_NODES est renvoyé
 *	@assign  : ------
 */
static INDEX dijkstra_next_node(t_matrix * ws, t_node * nodes) {
	INDEX u = MAX_NODES;
	INDEX v;
	/* pour chaque sommet ... */
	for (v = 0 ; v < ws->n ; v++) {
		/*
		 * si l'on trouve un 1er sommet non visité,
		 *   OU
		 * si on trouve un sommet non visité de poids plus faible
		 */
		if (!nodes[v].visited && (u == MAX_NODES
					|| nodes[v].pathw < nodes[u].pathw)) {
			u = v;
		}
	}

	/* si 'u' == MAX_NODES, alors tous les sommets sont déjà visité.
	 *	OU
	 * si 'u' n'est pas dans la meme partie connexe que 's'
	 *	ALORS
	 * erreur, non connecté
	 */
	if (u == MAX_NODES || nodes[u].pathlen == 0) {
		return (MAX_NODES) ;
	}
	return (u);
}

/**
 *	@require : une matrice representant les arcs et leur poids, deux indices s et t
 *	@ensure  : affiche le plus court chemin trouve entre 's' et 't'
 *	@assign  : ------
 */
static void dijkstra_result(t_node * nodes, INDEX s, INDEX t) {
	/* la pile contenant le chemin */
	INDEX pathlen = nodes[t].pathlen;
	t_array * path = array_new(pathlen, sizeof(INDEX));
	if (path == NULL) {
		fprintf(stderr, "Pas assez de mémoire\n");
		return ;
	}
	
	/* on construit le chemin */
	INDEX i = t;
	while (i != s) {
		i = nodes[i].prev;
		INDEX node = i + 1;
		array_add(path, &node);
	}

	/* on affiche le chemin */
	int j; /* besoin d'un int pour tester la positivité */
	for (j = path->size - 1 ; j >= 0 ; j--) {
		printf(INDEX_IDENTIFIER, *((INDEX *)array_get(path, j)));
		printf("\n");
	}
	printf(INDEX_IDENTIFIER, t + 1);
	printf("\n");

	array_delete(path);
}

/**
 *	@require : une matrice representant les arcs et leur poids, deux indices s et t
 *	@ensure  : resout les plus courts chemin dans le graphe
 *	@assign  : ------
 */
static void dijkstra(t_matrix * ws, INDEX s, INDEX t) {
	/* initialisation des sommets */
	t_node * nodes = (t_node *) malloc(ws->n * sizeof(t_node));
	if (nodes == NULL) {
		return ;
	}
	INDEX v;
	for (v = 0 ; v < ws->n ; v++) {
		nodes[v].visited = 0;
		nodes[v].pathw = UINT_MAX;
		nodes[v].pathlen = 0;
	}
	nodes[s].pathlen = 1;
	nodes[s].pathw = 0;
	nodes[s].prev =  s;

	/* boucle dijsktra */
	while (1) {
		/** on cherche un noeud 'u' non visite minimisant d(u) */
		INDEX u = dijkstra_next_node(ws, nodes);
		if (u == MAX_NODES) {
			printf("Not connected\n");
			break ;
		}

		/* si on a atteint t, on affiche le resultat */
		if (u == t) {
			dijkstra_result(nodes, s, t);
			break ;
		}

		/* on definit 'u' comme visite */
		nodes[u].visited = 1;
		
		/* pour chaque sommet */
		for (v = 0 ; v < ws->n ; v++) {
			/* s'il est voisin de 'u' ET si ce nouveau chemin est plus court */
			int wuv = matrix_get(ws, u, v);
			if (wuv >= 0 && nodes[u].pathw + wuv < nodes[v].pathw) {
				/* on cree le nouveau chemin */
				nodes[v].pathw = nodes[u].pathw + wuv;
				nodes[v].prev = u;
				nodes[v].pathlen = nodes[u].pathlen + 1;
			}
		}
	}
	/* libere la mémoire */
	free(nodes);
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
	dijkstra(ws, s, t);

	/* libere la mémoire */
	matrix_delete(ws);

	return (EXIT_SUCCESS);
}
