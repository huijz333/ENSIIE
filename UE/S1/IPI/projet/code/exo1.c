# include "customlibs/exo1.h"

/**
 *	@require : un nombre de ligne et de colonne n
 *	@ensure	 : renvoie une bitmap 2D de dimension (n, n), lu sur l'entrée standart
 *	@assign	 : ----------------------
 */
static t_bitmap * parse_arcs(INDEX n) {
	t_bitmap * arcs = bitmap_new2(n, n);
	if (arcs == NULL) {
		return (NULL);
	}

	INDEX i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			BYTE is_one;
			scanf("%hhu", &is_one);
			if (is_one) {
				bitmap_set2(arcs, i, j, n);
			}
		}
	}
	return (arcs);
}

int main(void) {
	/* lecture de la matrice */
	INDEX n;
	scanf(INDEX_IDENTIFIER, &n);
	t_bitmap * arcs = parse_arcs(n); 
	if (arcs == NULL) {
		fprintf(stderr, "Erreur d'allocations ou de parsing. Arrêt.\n");
		return (EXIT_FAILURE);
	}
	
	/* argument du parcours en largeur */
	INDEX s, t;
	scanf(INDEX_IDENTIFIER, &s);
	scanf(INDEX_IDENTIFIER, &t);
	--s;
	--t;

	/* faire le parcours en largeur. */
	t_array * nodes = nodes_new(n, sizeof(t_node));
	if (nodes == NULL) {
		fprintf(stderr, "Pas assez de mémoire.\n");
		return (EXIT_FAILURE);
	}
	breadth_search(nodes, arcs, s, t); /* parcours en largeur */
	bitmap_delete(arcs); /* plus besoin des arcs, on libere la mémoire */
	
	/* construit et affiche le chemin entre les 's' et 't' */
	node_solve_path(nodes, s, t, stdout);
	array_delete(nodes);
	return (EXIT_SUCCESS);
}

