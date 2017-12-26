# include "customlibs/exo1.h"

static t_array * nodes_new(void) {
	/* lecture des données */
	INDEX n;
	scanf(INDEX_IDENTIFIER, &n);

	/* on crée 'n' sommets */
	t_array * nodes = array_new(n, sizeof(t_node));
	if (nodes == NULL) {
		fprintf(stderr, "Pas assez de mémoire\n");
		exit(EXIT_FAILURE);
	}
	array_addempty(nodes, n); /* on ajoute 'n' sommets non initialisé */

	/* on lit la matrice et on definit les successeurs */
	INDEX i, j;
	for (i = 0 ; i < n ; i++) {
		t_node * u = (t_node *)array_get(nodes, i);
		u->successors = NULL;
		for (j = 0 ; j < n ; j++) {
			/* on recupere l'arc */
			BYTE is_one;
			scanf("%hhu", &is_one);
			/* s'il y a un arc entre i et j */
			if (is_one) {
				if (u->successors == NULL) {
					u->successors = array_new(1, sizeof(INDEX));
				}
				array_add(u->successors, &j);
			}
		}
	}
	return (nodes);
}

static void nodes_delete(t_array * nodes) {
	INDEX i;
	for (i = 0 ; i < nodes->size ; i++) {
		t_node * node = (t_node *) array_get(nodes, i);
		array_delete(node->successors);
	}
	array_delete(nodes);
}

int main(void) {
	/* lecture du graphe */
	t_array * nodes = nodes_new();
	
	/* argument du parcours en largeur */
	INDEX s, t;
	scanf(INDEX_IDENTIFIER, &s);
	scanf(INDEX_IDENTIFIER, &t);
	--s;
	--t;

	/* faire le parcours en largeur. */
	breadth_search(nodes, s, t);
	
	/* construit et affiche le chemin entre les 's' et 't' */
	node_solve_path(nodes, s, t, stdout);
	nodes_delete(nodes);
	return (EXIT_SUCCESS);
}

