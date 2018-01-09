# include "customlibs/exo2.h"

/**
 *	@require : les sommets, l'indice de départ, l'indice d'arrivé
 *	@ensure  : affiche le chemin si la résolution a réussi à l'aide
 *			de l'algorithme de remontée
 *	@assign  : --------
 */
static void print_path(t_node * nodes, INDEX s, INDEX t) {
	/** on construit le chemin en ajoutant les sommets sur une pile */
	t_list * path = list_new();
	INDEX u = t;
	while (u != s) {
		list_push(path, &u, sizeof(INDEX));
		u = nodes[u].prev;
	}
	
	/** on affiche */
	printf(INDEX_IDENTIFIER "\n", s + 1);
	LIST_ITERATE_START(path, INDEX *, nodeRef) {
		printf(INDEX_IDENTIFIER "\n", *nodeRef + 1);
	}
	LIST_ITERATE_STOP(path, INDEX *, nodeRef);
	
	/** libere la mémoire */
	list_delete(path);
}

/** fonction interne qui compare 2 nombres (utile à la file de priorité) */
static int weightcmp(WEIGHT * a, WEIGHT * b) {
	return ((*a < *b) ? -1 : (*a > *b) ? 1 : 0);
}

/**
 *	@require : 	'nodes':	un tableau de sommet
 *			'n':		taille du tableau
 *			's':		un indice sommet source
 *			't':		un indice de sommet de destination
 *	@ensure  : resout le plus court chemin dans le graphe entre 's' et 't'.
 *			Renvoie 1 si un chemin a été trouvé, 0 sinon.
 *	@assign  : 'nodes': les attributs des sommets peuvent être modifié
 */
static int dijkstra(t_node * nodes, INDEX n, INDEX s, INDEX t) {
	/** file de priorité, enregistrant les sommets a visité dans l'ordre */
	t_pqueue * visitQueue = pqueue_new((t_cmpf)weightcmp);
	/** tableau enregistrant les sommets de la file (pour les repositionner) */
	t_pqueue_node ** pqueue_nodes = (t_pqueue_node **) malloc(sizeof(t_pqueue_node *) * n);
	if (visitQueue == NULL || pqueue_nodes == NULL) {
		pqueue_delete(visitQueue);
		free(pqueue_nodes);
		fprintf(stderr, "Not enough memory.\n");
		return (0);
	}

	/** 1. INITIALISATION DE L'ALGORITHME */
	/** pour chaque sommets */
	INDEX i;
	for (i = 0 ; i < n ; i++) {
		/** on definit sa distance de 's' à '+oo' */
		nodes[i].cost = INF_WEIGHT;
		/** le sommet n'est pas dans la file: pas de position dans la file */
		pqueue_nodes[i] = NULL;
	}
	nodes[s].cost = 0;
	pqueue_nodes[s] = pqueue_insert(visitQueue, &(nodes[s].cost), &s);

	/** 2. BOUCLE DE L'ALGORITHME DIJKSTRA */
	/** Tant qu'il y a des sommets à visiter, on les visite */
	while (!pqueue_is_empty(visitQueue)) {
		/** 2.1. : on cherche un noeud 'u' non visite minimisant d(u).
		  ceci est optimisé à l'aide d'une file de priorité, l'opération
		  est donc ici trivial */
		t_pqueue_node node = pqueue_pop(visitQueue);
		INDEX u = *((INDEX *) node.value);
		/** le sommet n'est plus dans la file */
		pqueue_nodes[u] = NULL;

		/** si on a atteint 't', ou si on est dans une autre partie connexe ... */
		if (u == t || nodes[u].cost == INF_WEIGHT) {
			break ;
		}

		/** 2.2 : on minimise les chemins voisins de 'u' */
		/* pour chaque successeur de 'u' */
		LIST_ITERATE_START(nodes[u].arcs, t_arc *, arc) {
			/** successeur 'v' de 'u' */
			INDEX v = arc->to;
			/** on teste voir si le chemin de 's' à 't' passant
			  par 'u' et 'v' est de poids plus faible que le chemin
			  précèdemment enregistré */
			if (nodes[u].cost + arc->cost < nodes[v].cost) {
				/* on ecrase le chemin precedant par le nouveau chemin */
				nodes[v].cost = nodes[u].cost + arc->cost;
				nodes[v].prev = u;
				/** on enregistre les sommets dans la file de priorité */
				if (pqueue_nodes[v] == NULL) {
					pqueue_nodes[v] = pqueue_insert(visitQueue, &(nodes[v].cost), &(arc->to));
				} else {
					pqueue_decrease(visitQueue, pqueue_nodes[v], &(nodes[v].cost));
				}
			}
		}
		LIST_ITERATE_STOP(nodes[u].arcs, t_arc *, arc);
	}

	/** libère la mémoire */
	pqueue_delete(visitQueue);
	free(pqueue_nodes);
	return (nodes[t].cost != INF_WEIGHT);
}

int main(void) {
	/* lecture des données */
	INDEX n;
	scanf(INDEX_IDENTIFIER, &n);

	/* on crée 'n' sommets */
	t_node * nodes = (t_node *) malloc(sizeof(t_node) * n);
	if (nodes == NULL) {
		fprintf(stderr, "Pas assez de mémoire\n");
		return(EXIT_FAILURE);
	}

	/* on lit la matrice et on definit les successeurs et les poids */
	INDEX i, j;
	for (i = 0 ; i < n ; i++) {
		t_node * u = nodes + i;
		u->arcs = NULL;
		for (j = 0 ; j < n ; j++) {
			/* on recupere le poids */
			int value;
			scanf("%d", &value);
			/** si negatif, ils ne sont pas voisins */
			if (value < 0) {
				continue ;
			}
			/** sinon, on initialise la liste des successeurs */
			if (u->arcs == NULL) {
				u->arcs = list_new();
			}
			t_arc arc;
			arc.cost = (WEIGHT)value;
			arc.to = j;
			list_add(u->arcs, &arc, sizeof(t_arc));
		}
	}

	/* argument du parcours */
	INDEX s, t;
	scanf(INDEX_IDENTIFIER, &s);
	scanf(INDEX_IDENTIFIER, &t);
	--s;
	--t;

	/* faire l'algorithme de dijkstra */
	if (dijkstra(nodes, n, s, t)) {
		print_path(nodes, s, t);
	} else {
		printf("Not connected\n");
	}
	
	/* libere la mémoire */
	for (i = 0 ; i < n ; i++) {
		list_delete(nodes[i].arcs);
	}
	free(nodes);

	return (EXIT_SUCCESS);
}
