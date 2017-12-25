# include "customlibs/exo2.h"

int main(void) {
	/* lecture de la matrice */
	t_matrix * ws = matrix_parse();
	if (ws == NULL) {
		fprintf(stderr, "Allocation or parsing failed.\n");
		return (EXIT_FAILURE);
	}
	INDEX n = ws->n;
	
	/* argument du parcours */
	INDEX s, t;
	scanf(INDEX_IDENTIFIER, &s);
	scanf(INDEX_IDENTIFIER, &t);
	--s;
	--t;

	/* faire l'algorithme de dijkstra */
	t_array * nodes = nodes_new(n, sizeof(t_nodew));
	int r = dijkstra(nodes, ws, s, t);
	if (r) {
		node_solve_path(nodes, s, t, stdout);
	} else {
		printf("Not connected\n");
	}
	array_delete(nodes);

	/* libere la mémoire */
	matrix_delete(ws);

	return (EXIT_SUCCESS);
}
