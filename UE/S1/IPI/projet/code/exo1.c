#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


/** GENERIC MATRIX FUNCTIONS */
typedef struct	s_matrix {
	unsigned int n;
}		t_matrix;

# define MAX_EDGES (50)

typedef struct	s_edge {
	unsigned char	visited;
	unsigned int	path_len;
	unsigned int	path[MAX_EDGES];
}		t_edge;

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

/**
  EXO 1 (find minimum path between s and t
	recurcively visit every edges, using a flood filling algoritm
*/
static void dijkstra_visit(t_matrix * matrix, t_edge * edges, unsigned int i) {
	/* if this edge has already been visited, stop recursivity */
	if (edges[i].visited) {
		return ;
	}
	/* else, set this edge as visited */
	edges[i].visited = 1;
	
	/* for each neighboors */
	unsigned int j;
	for (j = 0 ; j < matrix->n ; j++) {
		/* update distance from 's' */
		if (matrix_get(matrix, i, j)) {
			t_edge * curr = edges + i;
			t_edge * next = edges + j;
			if (curr->path_len + 1 < next->path_len) {
				next->path_len = curr->path_len + 1;
				memcpy(next->path, curr->path, sizeof(unsigned int) * curr->path_len);
				next->path[curr->path_len] = j;

				/** reset visited from this */
				unsigned int k;
				for (k = 0 ; k < matrix->n ; k++) {
					edges[k].visited = 0;
				}
				for (k = 0 ; k < curr->path_len ; k++) {
					edges[curr->path[k]].visited = 1;
				}
				dijkstra_visit(matrix, edges, j);
			}
		}
	}
}

static void dijkstra(t_matrix * matrix, unsigned int s, unsigned int t) {
	/* allocate per-edges attributes */
	t_edge * edges = (t_edge *)malloc(sizeof(t_edge) * matrix->n);
	if (edges == NULL) {
		return ;
	}
	
	/* dijkstra initialisation: set every distances to INFINITY */
	unsigned int i;
	for (i = 0 ; i < matrix->n ; i++) {
		edges[i].visited = 0;
		edges[i].path_len = MAX_EDGES;
	}
	
	/* set origin distance to 0 */
	edges[s].path_len = 1;
	edges[s].path[0] = s;

	/* start recursivity */
	dijkstra_visit(matrix, edges, s);

	unsigned int path_len = edges[t].path_len;
	unsigned int * path = edges[t].path;

	if (path_len < MAX_EDGES) {
		for (i = 0 ; i < path_len ; i++) {
			printf("%u\n", path[i] + 1);
		}
	} else {
		printf("Not connected\n");
	}

	free(edges);
}

int main() {
	t_matrix * matrix = matrix_parse();
	if (matrix == NULL) {
		fprintf(stderr, "Allocation or parsing failed.\n");
		return (EXIT_FAILURE);
	}
	
	unsigned int s, t;
	scanf("%u", &s);
	scanf("%u", &t);
	--s;
	--t;

	dijkstra(matrix, s, t);

	matrix_delete(matrix);
	return (EXIT_SUCCESS);
}

