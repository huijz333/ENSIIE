# include "customlibs/exo1.h"

int main(void) {
	/* parsing de la matrice */
	t_bitmap * arcs = bitmap_parse(stdin);
	if (arcs == NULL) {
		fprintf(stderr, "Erreur d'allocations ou de parsing. Arrêt.\n");
		return (EXIT_FAILURE);
	}
	
	/* argument du parcours en profondeur */
	INDEX s, t;
	scanf(INDEX_IDENTIFIER, &s);
	scanf(INDEX_IDENTIFIER, &t);
	--s;
	--t;

	/* faire le parcours en profondeur */
	t_array * nodes = depth_breadth_search(arcs, s);
	bitmap_delete(arcs);
	
	/* construit et affiche le chemin entre les 's' et 't' */
	node_solve_path(nodes, s, t, stdout);
	free(nodes);
	return (EXIT_SUCCESS);
}

