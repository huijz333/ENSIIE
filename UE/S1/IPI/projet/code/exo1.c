# include "customlibs/exo1.h"

/**
 *	@require : les sommets, l'indice de départ, l'indice d'arrivé
 *	@ensure  : affiche le chemin si la résolution a réussi à l'aide
 *			de l'algorithme de remontée
 *	@assign  : --------
 */
static void print_path(t_node * nodes, INDEX sID, INDEX tID) {
	INDEX uID = tID;
	t_node * u = nodes + tID;
	/** on construit le chemin */
	t_list * path = list_new();
	while (uID != sID) {
		list_push(path, &uID, sizeof(INDEX));
		uID = u->prev;
		u = nodes + uID;
	}
	/** on affiche */
	printf(INDEX_IDENTIFIER "\n", sID + 1);
	LIST_ITERATE_START(path, INDEX *, nodeRef) {
		printf(INDEX_IDENTIFIER "\n", *nodeRef + 1);
	}
	LIST_ITERATE_STOP(path, INDEX *, nodeRef);
	/** libere la mémoire */
	list_delete(path);
}

/**
 *	@require : 	'nodes' : un tableau de structure 't_node' (ou en héritant)
 *		 	'arcs' : une bitmap representant les arcs
 *			's' : indice de sommet source
 *			't' : indice de sommet destination
 *	@ensure  : parcours le graphe en largeur, et trouve le chemin le plus court
 *		   entre chaque sommet et le sommet source
 *			Renvoie 1 si un chemin a été trouvé, 0 sinon.
 *	@assign  : modifies le tableau 'nodes' en lui appliquant un algorithme
 *		   de parcours en largeur sur ses sommets
 */
int breadth_search(t_node * nodes, INDEX n, INDEX sID, INDEX tID) {

	/** 1 : INITIALISATION */
	/** 1.1 : on crée la file de visite */
	t_list * unvisited = list_new();
	if (unvisited == NULL) {
		list_delete(unvisited);
		return (0);
	}

	/** 1.2 : initilisation: on definie toutes
	   les distantes à +oo (MAX_NODES suffit),
	   sauf pour l'origine à 0 */
	INDEX i;
	for (i = 0 ; i < n ; i++) {
		t_node * node = nodes + i;
		node->pathlen = MAX_NODES;
		node->prev = MAX_NODES;
	}
	nodes[sID].pathlen = 0; /* source à 0 */

	/** on ajoute la source a la file */
	list_add(unvisited, &sID, sizeof(INDEX));

	/** 2 : tant que la file n'est pas vide */
	while (unvisited->size > 0) {
		/** 2.1 : on pop la tête de file */
		INDEX uID = *((INDEX *) list_head(unvisited));
		list_remove_head(unvisited);	
		/* si on a atteint 't', on a trouvé le chemin */
		if (uID == tID) {
			list_delete(unvisited);
			return (1);
		}

		/** on recupere le sommet correspondant */
		t_node * u = nodes + uID;

		/** 2.2 : on visite chaque successeurs 'v' de 'u' */
		LIST_ITERATE_START(u->successors, INDEX *, vIDref) { 
			/** 'v' est successeur de 'u' */
			INDEX vID = *vIDref;
			t_node * v = nodes + vID;
			/** on visite 'v' */
			if (u->pathlen + 1 < v->pathlen) {
				v->prev = uID;
				v->pathlen = u->pathlen + 1;
				/* sinon, on ajoute 'v' à la file */
				list_add(unvisited, &vID, sizeof(INDEX));
			}
		}
		LIST_ITERATE_STOP(u->successors, INDEX *, vIDref);
	}

	/** sinon, 's' et 't' sont dans des parties connexes distinctes */
	list_delete(unvisited);
	return (0);
}

int main(void) {
	/* lecture du graphe */
	INDEX n;
	scanf(INDEX_IDENTIFIER, &n);

	/* on crée 'n' sommets */
	t_node * nodes = (t_node *) malloc(sizeof(t_node) * n);
	if (nodes == NULL) {
		fprintf(stderr, "Pas assez de mémoire\n");
		return(EXIT_FAILURE);
	}

	/* on lit la matrice et on definit les successeurs */
	INDEX i, j;
	for (i = 0 ; i < n ; i++) {
		t_node * u = nodes + i;
		u->successors = NULL;
		for (j = 0 ; j < n ; j++) {
			/* on recupere l'arc */
			BYTE is_one;
			scanf("%hhu", &is_one);
			/* s'il y a un arc entre i et j */
			if (is_one) {
				if (u->successors == NULL) {
					u->successors = list_new();
				}
				list_add(u->successors, &j, sizeof(INDEX));
			}
		}
	}
	
	/* argument du parcours en largeur */
	INDEX s, t;
	scanf(INDEX_IDENTIFIER, &s);
	scanf(INDEX_IDENTIFIER, &t);
	--s;
	--t;

	/* faire le parcours en largeur et afficher résultat */
	if (breadth_search(nodes, n, s, t)) {
		/** si succès, on effectue l'algorithme de remontée */
		print_path(nodes, s, t);
	} else {
		printf("Not connected\n");
	}
	
	/** libere la mémoire */
	for (i = 0 ; i < n ; i++) {
		list_delete(nodes[i].successors);
	}
	free(nodes);
	return (EXIT_SUCCESS);
}

