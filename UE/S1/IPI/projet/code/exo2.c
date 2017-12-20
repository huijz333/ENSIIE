# include "exo2.h"

/** DEBUT: FONCTIONS GENERIQUES SUR LES MATRICES */
typedef struct	s_matrix {
	unsigned int n;
}		t_matrix;

static int * matrix_addr(t_matrix * matrix, unsigned int i, unsigned int j) {
	int * values = (int *) (matrix + 1);
	return (values + matrix->n * j + i);
}

static int matrix_get(t_matrix * matrix, unsigned int i, unsigned int j) {
	return *(matrix_addr(matrix, i , j));
}

static t_matrix * matrix_new(unsigned int n) {
	t_matrix * matrix = (t_matrix *) malloc(sizeof(t_matrix) + n * n * sizeof(int));
	if (matrix == NULL) {
		return (NULL);
	}
	matrix->n = n;
	memset(matrix + 1, 0, n * n * sizeof(int));
	return (matrix);
}

static t_matrix * matrix_parse(void) {
	unsigned int n;
	scanf("%u", &n);
	t_matrix * matrix = matrix_new(n);
	if (matrix == NULL) {
		return (NULL);
	}

	unsigned int i, j;
	for (i = 0; i < n; i++) {
		for(j = 0; j < n; j++) {
			scanf("%d", matrix_addr(matrix, i, j));
		}
	}
	return (matrix);
}

static void matrix_delete(t_matrix * matrix) {
	free(matrix);
}

/** FIN: FONCTIONS GENERIQUES SUR LES MATRICES */
/** DEBUT: DIJKSTRA */

static void dijkstra(t_matrix * ws, unsigned int s, unsigned int t) {
	t_node * nodes = (t_node *) malloc(ws->n * sizeof(t_node));
	if (nodes == NULL) {
		return ;
	}

	unsigned int v;
	for (v = 0 ; v < ws->n ; v++) {
		nodes[v].visited = 0;
		nodes[v].d = UINT_MAX;
		nodes[v].path_len = 0;
	}
	nodes[s].d = 0;
	nodes[s].path_len = 1;
	nodes[s].path[0] = s;

	while (1) {
		/** on cherche un noeud 'u' non visite minimisant d(u) */
		unsigned int u = MAX_NODES;
		unsigned int v;
		for (v = 0 ; v < ws->n ; v++) {
			if (!nodes[v].visited && (u == MAX_NODES || nodes[v].d < nodes[u].d)) {
				u = v;
			}
		}

		/* si 'u' n'a pas ete trouve, OU s'il n'y a pas de chemin */
		if (u == MAX_NODES || nodes[u].path_len == 0) {
			printf("Not connected\n");
			break ;
		}

		/* si on a atteint t */
		if (u == t) {
			/** on affiche le resultat */
			for (v = 0 ; v < nodes[t].path_len ; v++) {
				printf("%u\n", nodes[t].path[v] + 1);
			}
			break ;
		}

		/* on definit 'u' comme visite */
		nodes[u].visited = 1;
		
		/* pour chaque sommet */
		for (v = 0 ; v < ws->n ; v++) {
			/* s'il est voisin de 'u' ET si ce nouveau chemin est plus court */
			int duv = matrix_get(ws, u, v);
			if (duv >= 0 && nodes[u].d + duv < nodes[v].d) {
				/* on cree le nouveau chemin */
				nodes[v].d = nodes[u].d + duv;
				memcpy(nodes[v].path, nodes[u].path, nodes[u].path_len * sizeof(unsigned int));
				nodes[v].path[nodes[u].path_len]= v;
				nodes[v].path_len = nodes[u].path_len + 1;
			}
		}
	}
	free(nodes);
}
/** FIN  : DIJKSTRA */

int main(void) {
	/* Matrix parsing */
	t_matrix * ws = matrix_parse();
	if (ws == NULL) {
		fprintf(stderr, "Allocation or parsing failed.\n");
		return (EXIT_FAILURE);
	}
	
	/* argument du parcours en profondeur */
	unsigned int s, t;
	scanf("%u", &s);
	scanf("%u", &t);
	--s;
	--t;

	/* faire le parcours en profondeur */
	dijkstra(ws, s, t);

	/* libere la mémoire */
	matrix_delete(ws);

	return (EXIT_SUCCESS);
}
