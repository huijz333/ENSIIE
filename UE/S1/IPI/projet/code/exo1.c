# include "customlibs/exo1.h"

/**
 *	@require : un t_graph * 'graph', deux indices 'i' et 'j'
 *	@ensure  : renvoie 1 si l'arc de 'i' à 'j' existe dans le t_graph 'graph'
 *	@assign  : ---
*/
static BIT graph_has_arc(t_graph * graph, INDEX i, INDEX j) {
	/**
	 * NB1:	on a ici une division, ce qui ralentirait théoriquement l'acces
	 *	en lecture et en ecriture aux donnees.
	 *	Cependant, BITS_PER_BYTE est une puissance de 2, donc
	 *	le compilateur saura optimiser cette division par un decalage de bit.
	 *	(BITS_PER_BYTE vaux 8, diviser par 8 est équivalent a un decalage
	 *	de 3 bits vers la droite...)
	 *
	 * NB2:	De plus, utilisé moins de mémoire permet de rendre le programme
	 *	'cache-friendly', à savoir, que la mémoire du tas est copié
	 *	dans la mémoire cache du processeur, la rendant très rapide d'accès.
	 *	En réduisant la mémoire, on s'évite ainsi de nombreux aller/retour
	 *	dans la mémoire.
	 */
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

	/* allocation du graphe, si elle echoue, on renvoie un graph nul */
	t_graph * graph = (t_graph *) malloc(sizeof(t_graph));
	if (graph == NULL) {	
		return (NULL);
	}

	/* alocation des sommets */
	graph->nodes = (t_node *) malloc(n * sizeof(t_node));
	if (graph->nodes == NULL) {
		free(graph);
		return (NULL);
	}
	graph->n = n;
	memset(graph->nodes, 0, n * sizeof(t_node));

	/* allocation des arcs */
	/* 'bitsize' : nombre de bit requis pour representer tous les arcs */
	unsigned int bitsize = n * n;
	/* 'arcsize' : nombre de bytes minimum requit pour stocker 'bitsize' bits */
	unsigned int arcsize = (bitsize / BITS_PER_BYTE) + ((bitsize % BITS_PER_BYTE) != 0);
	graph->arcs = (BYTE *) malloc(arcsize * sizeof(BYTE));
	memset(graph->arcs, 0, arcsize);
	return (graph);
}

/**
 *	@require : ---
 *	@ensure  : lit une matrice carre sur l'entree standart.
 *			Le 1er entier lu est la dimension 'n',
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
	scanf(INDEX_IDENTIFIER, &n);
	t_graph * graph = graph_new(n);
	if (graph == NULL) {
		return (NULL);
	}

	INDEX i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			BYTE arc_exists;
			scanf(INDEX_IDENTIFIER, &arc_exists);
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
				next->prev = i;
				next->pathlen = curr->pathlen + 1;

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

	/* debut de recursion */
	graph_visit(graph, s);
}

/**
 *	@require : un graphe, et un indice de somm
 *	@ensure  : visite ce sommet, puis se propage recursivement sur les voisins
 *	@assign  : modifie le chemin entre le sommet 'i' et le sommet 's'
 */
static void graph_print_path(t_graph * graph, INDEX s, INDEX t) {
	INDEX pathlen = graph->nodes[t].pathlen;

	if (pathlen == MAX_NODES) {
		printf("Not connected\n");
		return ;
	}
	/* la file contenant le chemin */
	t_array * array = array_new(pathlen, sizeof(INDEX));
	if (array == NULL) {
		fprintf(stderr, "not enough memory\n");
		return ;
	}

	/* on construit le chemin */
	INDEX i = t;
	while (i != s) {
		i = graph->nodes[i].prev;
		INDEX value = i + 1;
		array_add(array, &value);
	}

	/* on affiche le chemin */
	int j;
	for (j = array->size - 1; j >= 0 ; j--) {
		printf(INDEX_IDENTIFIER, *((INDEX *)array_get(array, j)));
		printf("\n");
	}
	printf(INDEX_IDENTIFIER, t + 1);
	printf("\n");
	
	/* libere la mémoire */
	array_delete(array);
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
	scanf(INDEX_IDENTIFIER, &s);
	scanf(INDEX_IDENTIFIER, &t);
	--s;
	--t;

	/* faire le parcours en profondeur */
	graph_depth_breadth_search(graph, s);
	graph_print_path(graph, s, t);

	/* libere la mémoire */
	graph_delete(graph);

	return (EXIT_SUCCESS);
}

