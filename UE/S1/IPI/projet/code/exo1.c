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




/** DEBUT: PARCOURS EN PROFONDEUR */

# define MAX_EDGES (50)

typedef struct	s_node {
	unsigned char	visited; /* 0 ou 1 si deja visite ou non */
	unsigned int	path_len; /* distance entre s et ce sommet */
	unsigned int	path[MAX_EDGES]; /* le chemin (index des sommets) */
}		t_node;

static void visit(t_matrix * arcs, t_node * nodes, unsigned int i) {
	/* if this node has already been visited, stop recursivity */
	if (nodes[i].visited) {
		return ;
	}
	/* else, set this node as visited */
	nodes[i].visited = 1;
	
	/* for each neighboors */
	unsigned int j;
	for (j = 0 ; j < arcs->n ; j++) {
		/* update distance from 's' */
		if (matrix_get(arcs, i, j)) {
			t_node * curr = nodes + i;
			t_node * next = nodes + j;
			if (curr->path_len + 1 < next->path_len) {
				next->path_len = curr->path_len + 1;
				memcpy(next->path, curr->path, sizeof(unsigned int) * curr->path_len);
				next->path[curr->path_len] = j;

				/** reset visited from this */
				unsigned int k;
				for (k = 0 ; k < arcs->n ; k++) {
					nodes[k].visited = 0;
				}
				visit(arcs, nodes, j);
			}
		}
	}
}

static void depth_breadth_search(t_matrix * arcs, unsigned int s, unsigned int t) {
	/* allocate per-nodes attributes */
	t_node * nodes = (t_node *)malloc(sizeof(t_node) * arcs->n);
	if (nodes == NULL) {
		return ;
	}
	
	/* algorithm initialisation: set every distances to INFINITY */
	unsigned int i;
	for (i = 0 ; i < arcs->n ; i++) {
		nodes[i].visited = 0;
		nodes[i].path_len = MAX_EDGES;
	}
	
	/* set origin distance to 0 */
	nodes[s].path_len = 1;
	nodes[s].path[0] = s;

	/* start recursivity */
	visit(arcs, nodes, s);

	/** done, print result */
	unsigned int path_len = nodes[t].path_len;
	unsigned int * path = nodes[t].path;

	if (path_len < MAX_EDGES) {
		for (i = 0 ; i < path_len ; i++) {
			printf("%u\n", path[i] + 1);
		}
	} else {
		printf("Not connected\n");
	}

	free(nodes);
}

/** FIN: PARCOURS EN PROFONDEUR */


int main(void) {
	/* Matrix parsing */
	t_matrix * arcs = matrix_parse();
	if (arcs == NULL) {
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
	depth_breadth_search(arcs, s, t);

	/* libere la mémoire */
	matrix_delete(arcs);

	return (EXIT_SUCCESS);
}

