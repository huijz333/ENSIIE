# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>


/** DEBUT: FONCTIONS SUR LES GRAPHES */

# define BYTE char
# define BITS_PER_BYTE (sizeof(BYTE))
# define MAX_EDGES (50)

typedef struct	s_node {
	unsigned int	path_len; 				/* nombre de sommets entre s et ce sommet */
	unsigned int	path[MAX_EDGES - 1];	/* le chemin (index des sommets) */
}				t_node;

typedef struct 	s_graph {
	unsigned int 	n; 			/* nombre de sommets */
	t_node 			* nodes; 	/* les sommets avec leur attributs */
	BYTE 			* arcs;		/* tableau de byte representant les arcs (un arc est codde sur un bit) */
}				t_graph;

/**
 *	@ensure
 *
 *
*/
static int graph_has_arc(t_graph * graph, unsigned int i, unsigned int j) {
	unsigned int bit = i * graph->n + j;
	
/*
	unsigned int byteID = bit / BITS_PER_BYTE;
	unsigned int bitID = bit % BITS_PER_BYTE;
	BYTE * bytes = graph->arcs;
	BYTE byte = bytes[byteID];
	return (byte & (1 << bitID) ? 1 : 0);
*/

	return (graph->arcs[bit / BITS_PER_BYTE] & (1 << (bit % BITS_PER_BYTE)) ? 1 : 0);
}

static void graph_set_arc(t_graph * graph, unsigned int i, unsigned int j) {
	unsigned int bit = i * graph->n + j;
/*
	unsigned int byteID = bit / BITS_PER_BYTE;
	unsigned int bitID = bit % BITS_PER_BYTE;
	BYTE * bytes = graph->arcs;
	BYTE * byte = bytes + byteID;
	*byte |= (1 << bitID);
*/
	*(graph->arcs + bit / BITS_PER_BYTE) |= (1 << bit % BITS_PER_BYTE);

}

static t_graph * graph_new(unsigned int n) {
	unsigned int nodesize = n * sizeof(t_node); /* nombre de bytes requis pour representer n structure t_node */

	unsigned int bitsize = n * n;																		/* 'bitsize' : nombre de bit requis pour representer tous les arcs */
	unsigned int arcsize = (n * n / BITS_PER_BYTE) + ((bitsize % BITS_PER_BYTE) != 0) * sizeof(BYTE);	/* 'arcsize' : nombre de bytes requit pour stocker 'bitsize' bits */
	BYTE * memory = (BYTE *) malloc(sizeof(t_graph) + nodesize + arcsize);								/* j'alloue toute la memoire requise avec un unique malloc */
	if (memory == NULL) {	/* si l'allocation a rate, on renvoie un graph nul */
		return (NULL);
	}

	t_graph * graph = (t_graph *) memory;
	graph->n = n;
	graph->nodes = (t_node *) (memory + sizeof(t_graph));
	graph->arcs = memory + sizeof(t_graph) + nodesize;

	memset(graph->arcs, 0, arcsize);
	return (graph);
}

static t_graph * graph_parse(void) {
	unsigned int n;
	scanf("%u", &n);
	t_graph * graph = graph_new(n);
	if (graph == NULL) {
		return (NULL);
	}

	unsigned int i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			int arc_exists;
			scanf("%d", &arc_exists);
			if (arc_exists) {
				graph_set_arc(graph, i, j);
			}
		}
	}
	return (graph);
}

static void graph_delete(t_graph * graph) {
	free(graph);
}

/*
static void graph_print_arcs(t_graph * graph) {
	unsigned int i, j;
	for (i = 0 ; i < graph->n ; i++) {
		for (j = 0 ; j < graph->n - 1; j++) {
			printf("%d ", graph_has_arc(graph, i, j));
		}
		printf("%d\n", graph_has_arc(graph, i, j));
	}

}
*/

/** FIN: FONCTIONS SUR LES ARCS (TABLEAUX DE BITS) */




/** DEBUT: PARCOURS EN PROFONDEUR */
static void visit(t_graph * graph, unsigned int i) {
	
	/* for each neighboors */
	unsigned int j;
	for (j = 0 ; j < graph->n ; j++) {
		/* update distance from 's' */
		if (graph_has_arc(graph, i, j)) {
			t_node * curr = graph->nodes + i;
			t_node * next = graph->nodes + j;
			if (curr->path_len + 1 < next->path_len) {
				next->path_len = curr->path_len + 1;
				memcpy(next->path, curr->path, sizeof(unsigned int) * curr->path_len);
				next->path[curr->path_len] = j;

				/** reset visited from this */
				visit(graph, j);
			}
		}
	}
}

static void depth_breadth_search(t_graph * graph, unsigned int s, unsigned int t) {
	/* algorithm initialisation: set every distances to INFINITY */
	unsigned int i;
	for (i = 0 ; i < graph->n ; i++) {
		graph->nodes[i].path_len = MAX_EDGES;
	}
	
	/* set origin distance to 0 */
	graph->nodes[s].path_len = 0;

	/* start recursivity */
	visit(graph, s);

	/** done, print result */
	unsigned int path_len = graph->nodes[t].path_len;
	unsigned int * path = graph->nodes[t].path;

	if (path_len < MAX_EDGES) {
		printf("%u\n", s + 1);
		for (i = 0 ; i < path_len ; i++) {
			printf("%u\n", path[i] + 1);
		}
	} else {
		printf("Not connected\n");
	}
}

/** FIN: PARCOURS EN PROFONDEUR */


int main(void) {
	/* arcs parsing */
	t_graph * graph = graph_parse();
	if (graph == NULL) {
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
	depth_breadth_search(graph, s, t);

	/* libere la mémoire */
	graph_delete(graph);

	return (EXIT_SUCCESS);
}

