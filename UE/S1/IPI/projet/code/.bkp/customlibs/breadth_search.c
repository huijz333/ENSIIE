# include "breadth_search.h"

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
int breadth_search(t_array * nodes, INDEX s, INDEX t) {

	/** 1 : INITIALISATION */
	/** 1.1 : on crée la file de visite */
	INDEX n = nodes->size;
	t_list * unvisited = list_new();
	t_bitmap * visited = bitmap_new(n);
	if (unvisited == NULL || visited == NULL) {
		bitmap_delete(visited);
		list_delete(unvisited);
		return (0);
	}

	/** 1.2 : initilisation: on definie toutes
	   les distantes à +oo (MAX_NODES suffit),
	   sauf pour l'origine à 0 */
	INDEX i;
	for (i = 0 ; i < n ; i++) {
		t_node * node = (t_node *) array_get(nodes, i);
		node->pathlen = MAX_NODES;
		node->prev = -1;
	}
	((t_node *)array_get(nodes, s))->pathlen = 0; /* source à 0 */

	/** on ajoute la source a la file */
	list_add(unvisited, &s, sizeof(INDEX));

	/** 2 : tant que la file n'est pas vide */
	while (unvisited->size > 0) {
		/** 2.1 : on pop la tête de file */
		INDEX uID = *((INDEX *) list_head(unvisited));
		list_remove_head(unvisited);	

		/** on recupere le sommet correspondant */
		t_node * u = (t_node *) array_get(nodes, uID);

		/** 2.2 : on visite chaque successeurs 'v' de 'u' */
		/** s'il n'a pas de successeurs, on continue de vider la file */
		if (u->successors == NULL) {
			continue ;
		}

		/** sinon, pour chaque successeur */
		INDEX i;
		for (i = 0 ; i < u->successors->size ; i++) {
			/** 'v' est successeur de 'u' */
			INDEX vID = *((INDEX *) array_get(u->successors, i));
			
			/* si on a déjà visité 'v', on passe au voisin suivant */
			if (bitmap_get(visited, vID)) {
				continue ;
			}

			/* sinon, on visite 'v' */
			t_node * v = (t_node *) array_get(nodes, vID);	
			bitmap_set(visited, vID);

			v->prev = uID;
			v->pathlen = u->pathlen + 1;
			/* si on a atteint 't', on a trouvé le chemin */
			if (vID == t) {
				bitmap_delete(visited);
				list_delete(unvisited);
				return (1);
			}
			/* sinon, on ajoute 'v' à la file */
			list_add(unvisited, &vID, sizeof(INDEX));
		}
	}
	/** sinon, 's' et 't' sont dans des parties connexes distinctes */
	bitmap_delete(visited);
	list_delete(unvisited);
	return (0);
}
