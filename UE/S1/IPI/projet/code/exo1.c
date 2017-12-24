# include "customlibs/exo1.h"

/**
 *	@require : un graphe, et un indice de somm
 *	@ensure  : visite ce sommet, puis se propage recursivement sur les voisins
 *	@assign  : modifie le chemin entre le sommet 'i' et le sommet 's'
 */
static void print_path(t_node * nodes, INDEX s, INDEX t) {
	INDEX pathlen = nodes[t].pathlen;

	if (pathlen == MAX_NODES) {
		printf("Not connected\n");
		return ;
	}
	/* la file contenant le chemin */
	t_array * array = array_new(pathlen + 1, sizeof(INDEX));
	if (array == NULL) {
		fprintf(stderr, "not enough memory\n");
		return ;
	}
	
	/* on construit le chemin */
	INDEX i = t;
	while (i != s) {
		array_add(array, &i);
		i = nodes[i].prev;
	}
	array_add(array, &s);

	/* on affiche le chemin */
	int j;
	for (j = array->size - 1; j >= 0 ; j--) {
		INDEX * value = (INDEX *)array_get(array, j);
		printf(INDEX_IDENTIFIER "\n", *value + 1);
	}
	
	/* libere la mémoire */
	array_delete(array);
}

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
	t_node * nodes = depth_breadth_search(arcs, s);
	bitmap_delete(arcs);
	
	/* affiche le resultat */
	print_path(nodes, s, t);
	free(nodes);

	return (EXIT_SUCCESS);
}

