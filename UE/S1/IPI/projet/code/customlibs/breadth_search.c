# include "breadth_search.h"

/**
 *	@require : 	'nodes' : un tableau de structure 't_node' (ou en héritant)
 *		 	'arcs' : une bitmap representant les arcs
 *			's' : indice de sommet source
 *			't' : indice de sommet destination
 *	@ensure  : parcours le graphe en largeur, et trouve le chemin le plus court
 *		   entre chaque sommet et le sommet source
 *	@assign  : modifies le tableau 'nodes' en lui appliquant un algorithme
 *		   de parcours en largeur sur ses sommets
 */
void breadth_search(t_array * nodes, t_bitmap * arcs, INDEX s, INDEX t) {

	/* initilisation: on definie toutes les distantes à +oo (MAX_NODES suffit),
	   sauf pour l'origine à 0 */
	INDEX n = nodes->size;
	INDEX i;
	for (i = 0 ; i < n ; i++) {
		t_node * node = (t_node *) array_get(nodes, i);
		node->pathlen = MAX_NODES;
		node->prev = -1;
		node->visited = 0;
	}
	((t_node *)array_get(nodes, s))->pathlen = 0; /* source à 0 */

	/** on crée une file */
	t_list * queue = list_new();
	if (queue == NULL) {
		return ;
	}
	/** on ajoute la source a la file */
	list_add(queue, &s, sizeof(INDEX));

	/** tant que la file n'est pas vide */
	while (queue->size > 0) {
		/** on pop la tête de file */
		INDEX currindex = *((INDEX *) list_head(queue));
		list_remove_head(queue);
			
		/** on recupere le sommet correspondant */
		t_node * currnode = (t_node *) array_get(nodes, currindex);

		INDEX v;
		for (v = 0 ; v < n ; v++) {
			if (bitmap_get2(arcs, currindex, v, n)) {
				/** alors 'v' est voisin de 'node' */
				t_node * neighbor = (t_node *) array_get(nodes, v);
				if (neighbor->visited) {
					continue ;
				}
				neighbor->visited = 1;
				neighbor->prev = currindex;
				neighbor->pathlen = currnode->pathlen + 1;
				if (v == t) {
					break ;
				}
				list_add(queue, &v, sizeof(INDEX));
			}
		}
	}
	list_delete(queue);
}
