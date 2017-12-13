# include "exo1.h"

/**
 *	@require : un t_graph * 'graph', deux indices 'i' et 'j'
 *	@ensure  : renvoie 1 si l'arc de 'i' à 'j' existe dans le t_graph 'graph'
 *	@assign  : ---
*/
static BIT graph_has_arc(t_graph * graph, INDEX i, INDEX j) {
	unsigned int bit = i * graph->n + j;
	return (graph->arcs[bit / BITS_PER_BYTE] & (1 << (bit % BITS_PER_BYTE)) ? 1 : 0);
}

/**
 *	@require : un t_graph * 'graph', deux indices 'i' et 'j'
 *	@ensure  : cree un arc en 'i' et 'j'
 *	@assign  : inseres un arc ('i', 'j') dans 'graph'
*/
static void graph_set_arc(t_graph * graph, INDEX i, INDEX j) {
	unsigned int bit = i * graph->n + j;
	/**
	 *	nb :
	 *	on a ici une division, ce qui ralenti l'acces en lecture au donnee
	 *	cependant, c'est BITS_PER_BYTE est une puissance de 2, donc
	 *	le compilateur saura optimiser cette division par un decalage de bit
	 */
	*(graph->arcs + bit / BITS_PER_BYTE) |= (1 << (bit % BITS_PER_BYTE));
}

/**
 *	@require : 'n' : le nombre de sommets du graphe
 *	@ensure  : renvoie un nouveau graphe pouvant accueillir au plus 'n' sommets,
 *		   ou NULL si l'allocation a echoue
 *	@assign  : le graphe est initialement vide
*/
static t_graph * graph_new(INDEX n) {
	if (n > MAX_NODES) {
		return (NULL);
	}

	/* nombre de bytes requis pour representer n structure t_node */
	unsigned int nodesize = n * sizeof(t_node);
	;/* 'bitsize' : nombre de bit requis pour representer tous les arcs */
	unsigned int bitsize = n * n;
	/* 'arcsize' : nombre de bytes minimum requit pour stocker 'bitsize' bits */
	unsigned int arcsize = (bitsize / BITS_PER_BYTE) + ((bitsize % BITS_PER_BYTE) != 0) * sizeof(BYTE);
	/* j'alloue toute la memoire requise avec un unique malloc */
	BYTE * memory = (BYTE *) malloc(sizeof(t_graph) + nodesize + arcsize);	
	/* si l'allocation a rate, on renvoie un graph nul */
	if (memory == NULL) {	
		return (NULL);
	}

	t_graph * graph = (t_graph *) memory;
	graph->n = n;
	graph->nodes = (t_node *) (memory + sizeof(t_graph));
	graph->arcs = (BYTE *) (memory + sizeof(t_graph) + nodesize);

	memset(graph->nodes, 0, nodesize);
	memset(graph->arcs, 0, arcsize);
	return (graph);
}

/**
 *	@require : ---
 *	@ensure  : lit une matrice carre sur l'entree standart. Le 1er entier lu est la dimension 'n',
 *			suivi de n * n valeurs, où la k-ieme valeur correspond
 *			a la ligne 'k / n' et a la colonne 'k % n'
 *
 *			 0    1   ...   k   ...   n
 *			n+1  n+2  ...  k+n  ...  2n-1
 *			          ........
 *			          ........
 *		        	  ........
 *			n(n-1)    ........	 n.n
 *
 *	@assign  : le graphe est initialement vide
*/
static t_graph * graph_parse(void) {
	INDEX n;
	scanf("%hhu", &n);
	t_graph * graph = graph_new(n);
	if (graph == NULL) {
		return (NULL);
	}

	INDEX i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			BYTE arc_exists;
			scanf("%hhu", &arc_exists);
			if (arc_exists) {
				graph_set_arc(graph, i, j);
			}
		}
	}
	return (graph);
}

/**
 *	@require : un graphe préallablement alloué via graph_new()
 *	@ensure  : libere la mémoire alloué => graphe supprimé de la mémoire
 *	@assign  : ---
 */
static void graph_delete(t_graph * graph) {
	free(graph);
}

/**
 *	@require : un graphe, et un indice de sommet
 *	@ensure  : visite ce sommet, puis se propage recursivement sur les voisins
 *	@assign  : modifie le chemin entre le sommet 'i' et le sommet 's'
 */
static void graph_visit(t_graph * graph, INDEX i) {
	
	/* pour chaque sommet */
	INDEX j;
	for (j = 0 ; j < graph->n ; j++) {
		/* si le sommet est voisin de i */
		if (graph_has_arc(graph, i, j)) {
			/* on teste voir si ce chemin est plus court */
			t_node * curr = graph->nodes + i;
			t_node * next = graph->nodes + j;
			/** si ce chemin est plus court ... */
			if (curr->pathlen + 1 < next->pathlen) {
				/** on met a jour le chemin */
				memcpy(next->path, curr->path, sizeof(INDEX) * curr->pathlen);
				next->pathlen = curr->pathlen + 1;
				next->path[curr->pathlen] = i;

				/** on re-itere alors sur ce voisin */
				graph_visit(graph, j);
			}
		}
	}
}

/**
 *	@require : un graphe, et un indice de sommet source
 *	@ensure  : parcours le graphe en largeur, et trouve le chemin le plus court
 *		   entre chaque sommet et le sommet source
 *	@assign  : modifie les chemins des 't_node' du graphe
 */
static void graph_depth_breadth_search(t_graph * graph, unsigned int s) {
	/* initilisation: on definie toutes les distantes à +oo, sauf pour l'origine à 0 */
	INDEX i;
	for (i = 0 ; i < graph->n ; i++) {
		graph->nodes[i].pathlen = MAX_NODES;
	}
	graph->nodes[s].pathlen = 0;
	graph->nodes[s].path[0] = 1;

	/* debut de recursion */
	graph_visit(graph, s);
}

/**
 *	@require : un graphe, et un indice de somm
 *	@ensure  : visite ce sommet, puis se propage recursivement sur les voisins
 *	@assign  : modifie le chemin entre le sommet 'i' et le sommet 's'
 */
static void print_result(t_graph * graph, INDEX t) {
	INDEX pathlen = graph->nodes[t].pathlen;
	INDEX * path = graph->nodes[t].path;

	if (pathlen == MAX_NODES) {
		printf("Not connected\n");
	} else {
		INDEX i;
		for (i = 0 ; i < pathlen ; i++) {
			printf("%hhu\n", path[i] + 1);
		}
		printf("%hhu\n", t + 1);
	}
}

int main(void) {
	/* parsing de la matrice */
	t_graph * graph = graph_parse();
	if (graph == NULL) {
		fprintf(stderr, "Erreur d'allocations ou de parsing. Arrêt.\n");
		return (EXIT_FAILURE);
	}

	/* argument du parcours en profondeur */
	INDEX s, t;
	scanf("%hhu", &s);
	scanf("%hhu", &t);
	--s;
	--t;

	/* faire le parcours en profondeur */
	graph_depth_breadth_search(graph, s);
	print_result(graph, t);

	/* libere la mémoire */
	graph_delete(graph);

	return (EXIT_SUCCESS);
}

