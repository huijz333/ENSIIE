# include "customlibs/exo2.h"

static t_array * nodes_new(void) {
	/* lecture des données */
	INDEX n;
	scanf(INDEX_IDENTIFIER, &n);

	/* on crée 'n' sommets */
	t_array * nodes = array_new(n, sizeof(t_nodew));
	if (nodes == NULL) {
		fprintf(stderr, "Pas assez de mémoire\n");
		exit(EXIT_FAILURE);
	}
	array_addempty(nodes, n); /* on ajoute 'n' sommets non initialisé */

	/* on lit la matrice et on definit les successeurs et les poids */
	INDEX i, j;
	for (i = 0 ; i < n ; i++) {
		t_nodew * u = (t_nodew *)array_get(nodes, i);
		u->ws = NULL;
		u->super.successors = NULL;
		for (j = 0 ; j < n ; j++) {
			/* on recupere le poids */
			int value;
			scanf("%d", &value);
			if (value < 0) {
				/** si negatif, ils ne sont pas voisins */
				continue ;
			}

			/** sinon, on initialise la liste des successeurs */
			if (u->super.successors == NULL) {
				u->ws = array_new(1, sizeof(WEIGHT));
				u->super.successors = array_new(1, sizeof(INDEX));
			}

			WEIGHT w = (WEIGHT)value;
			array_add(u->ws, &w);
			array_add(u->super.successors, &j);
		}
	}
	return (nodes);
}

static void nodes_delete(t_array * nodes) {
	INDEX i;
	for (i = 0 ; i < nodes->size ; i++) {
		t_nodew * node = (t_nodew *) array_get(nodes, i);
		array_delete(node->super.successors);
		array_delete(node->ws);
	}
	array_delete(nodes);
}

int main(void) {
	/** lit l'entrée */
	t_array * nodes = nodes_new();

	/* argument du parcours */
	INDEX s, t;
	scanf(INDEX_IDENTIFIER, &s);
	scanf(INDEX_IDENTIFIER, &t);
	--s;
	--t;

	/* faire l'algorithme de dijkstra */
	int r = dijkstra(nodes, s, t);
	/* ce qui est équivalent à faire, avec 'zero' une fonction qui renvoie 0 */
	/* int r = astar(nodes, zero, s, t); */
	if (r) {
		node_solve_path(nodes, s, t, stdout);
	} else {
		printf("Not connected\n");
	}
	
	/* libere la mémoire */
	nodes_delete(nodes);

	return (EXIT_SUCCESS);
}
