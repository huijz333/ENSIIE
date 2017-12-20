# include "exo2.h"

/** DEBUT: FONCTIONS GENERIQUES SUR LES MATRICES */

/* matrice carre n*n */
typedef struct	s_matrix {
	INDEX	n;
}		t_matrix;

/**
 *	@require : la taille de la matrice 'n'
 *	@ensure  : alloue une nouvelle matrice sur le tas, et la renvoie
 *	@assign  : la matrice est initialisé à 0
 */
static t_matrix * matrix_new(INDEX n) {
	t_matrix * matrix = (t_matrix *) malloc(sizeof(t_matrix) + n * n * sizeof(int));
	if (matrix == NULL) {
		return (NULL);
	}
	matrix->n = n;
	memset(matrix + 1, 0, n * n * sizeof(int));
	return (matrix);
}

/**
 *	@require : une matrice 'matrix', deux index (i, j)
 *	@ensure  : renvoie l'addresse de l'element (i, j) dans la matrice
 *	@assign  : ---
 */
static int * matrix_addr(t_matrix * matrix, INDEX i, INDEX j) {
	int * values = (int *) (matrix + 1);
	return (values + matrix->n * j + i);
}

/**
 *	@require : une matrice 'matrix', deux index (i, j)
 *	@ensure  : renvoie la valeur de l'element (i, j) dans la matrice
 *	@assign  : ---
 */
static int matrix_get(t_matrix * matrix, INDEX i, INDEX j) {
	return *(matrix_addr(matrix, i, j));
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
static t_matrix * matrix_parse(void) {
	INDEX n;
	scanf(INDEX_IDENTIFIER, &n);
	t_matrix * matrix = matrix_new(n);
	if (matrix == NULL) {
		return (NULL);
	}

	INDEX i, j;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			scanf("%d", matrix_addr(matrix, i, j));
		}
	}
	return (matrix);
}

/**
 *	@require : une matrice préallablement alloué via 'matrix_new()'
 *	@ensure  : libere la memoire utilise par la matrice du tas
 *	@assign  : ------
 */
static void matrix_delete(t_matrix * matrix) {
	free(matrix);
}

/** FIN: FONCTIONS GENERIQUES SUR LES MATRICES */


/** DEBUT: DIJKSTRA */

/**
 *	@require : une matrice representant les arcs et leur poids, et les sommets
 *	@ensure  : renvoie l'index du sommet non visité avec le chemin de poids minimum
 *			si le chemin n'existe pas, MAX_NODES est renvoyé
 *	@assign  : ------
 */
static INDEX dijkstra_minimum(t_matrix * ws, t_node * nodes) {
	INDEX u = MAX_NODES;
	INDEX v;
	for (v = 0 ; v < ws->n ; v++) {
		if (!nodes[v].visited && (u == MAX_NODES
					|| nodes[v].pathw < nodes[u].pathw)) {
			u = v;
		}
	}

	/* si 'u' n'a pas ete trouve, OU s'il n'y a pas de chemin */
	if (u == MAX_NODES || nodes[u].pathlen == 0) {
		return (MAX_NODES) ;
	}
	return (u);
}

/**
 *	@require : une matrice representant les arcs et leur poids, deux indices s et t
 *	@ensure  : affiche le plus court chemin trouve entre 's' et 't'
 *	@assign  : ------
 */
static void dijkstra_result(t_node * nodes, INDEX s, INDEX t) {
	/* la pile contenant le chemin */
	INDEX pathlen = nodes[t].pathlen;
	INDEX * path = (INDEX *) malloc(sizeof(INDEX) * pathlen);
	if (path == NULL) {
		fprintf(stderr, "not enough memory\n");
		return ;
	}
	/* on remplit la pile par la fin */
	INDEX i = pathlen;
	/* on construit le chemin */
	INDEX j = t;
	path[--i] = t;
	while (j != s) {
		t_node node = nodes[j];
		j = node.prev;
		path[--i] = j;
	}
	/* on affiche le resultat */
	for (i = 0 ; i < pathlen ; i++) {
		printf(INDEX_IDENTIFIER, path[i] + 1);
		printf("\n");
	}
	free(path);
}

/**
 *	@require : une matrice representant les arcs et leur poids, deux indices s et t
 *	@ensure  : resout les plus courts chemin dans le graphe
 *	@assign  : ------
 */
static void dijkstra(t_matrix * ws, INDEX s, INDEX t) {
	/* les sommets */
	t_node * nodes = (t_node *) malloc(ws->n * sizeof(t_node));
	if (nodes == NULL) {
		return ;
	}

	/* initialisation */
	INDEX v;
	for (v = 0 ; v < ws->n ; v++) {
		nodes[v].visited = 0;
		nodes[v].pathw = UINT_MAX;
		nodes[v].pathlen = 0;
	}
	nodes[s].pathlen = 1;
	nodes[s].pathw = 0;
	nodes[s].prev = s;

	/* boucle dijsktra */
	while (1) {
		/** on cherche un noeud 'u' non visite minimisant d(u) */
		INDEX u = dijkstra_minimum(ws, nodes);
		if (u == MAX_NODES) {
			printf("Not connected\n");
			break ;
		}

		/* si on a atteint t, on affiche le resultat */
		if (u == t) {
			dijkstra_result(nodes, s, t);
			break ;
		}

		/* on definit 'u' comme visite */
		nodes[u].visited = 1;
		
		/* pour chaque sommet */
		for (v = 0 ; v < ws->n ; v++) {
			/* s'il est voisin de 'u' ET si ce nouveau chemin est plus court */
			int wuv = matrix_get(ws, u, v);
			if (wuv >= 0 && nodes[u].pathw + wuv < nodes[v].pathw) {
				/* on cree le nouveau chemin */
				nodes[v].pathw = nodes[u].pathw + wuv;
				nodes[v].prev = u;
				nodes[v].pathlen = nodes[u].pathlen + 1;
			}
		}
	}
	/* libere la mémoire */
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
	INDEX s, t;
	scanf(INDEX_IDENTIFIER, &s);
	scanf(INDEX_IDENTIFIER, &t);
	--s;
	--t;

	/* faire le parcours en profondeur */
	dijkstra(ws, s, t);

	/* libere la mémoire */
	matrix_delete(ws);

	return (EXIT_SUCCESS);
}
