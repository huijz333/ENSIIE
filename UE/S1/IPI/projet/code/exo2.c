# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>

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
typedef struct	s_node {
	char		visited;
	unsigned int	d;
}		t_node;

# define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

static void dijkstra(t_matrix * ws, unsigned int s, unsigned int t) {
	t_node * nodes = (t_node *) malloc(ws->n * sizeof(t_node));
	if (nodes == NULL) {
		return ;
	}

	unsigned int v;
	for (v = 0 ; v < ws->n ; v++) {
		nodes[v].visited = 0;
		nodes[v].d = UINT_MAX;
	}
	nodes[s].d = 0;

	while (1) {
		unsigned int u;
		int wmin = INT_MAX;
		for (v = 0 ; v < ws->n ; v++) {
			if (nodes[v].visited) {
				continue ;
			}
			int w = matrix_get(ws, s, v);
			if (w >= 0 && w < wmin) {
				u = v;
				wmin = w;
			}
		}

		nodes[u].visited = 1;
		if (u == t) {
			break ;
		}

		for (v = 0 ; v < ws->n ; v++) {
			int w = matrix_get(ws, u, v);
			if (w >= 0) {
				unsigned int d = nodes[u].d + w;
				if (d < nodes[v].d) {
					nodes[v].d = d;
				}
			}
		}
	}
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
