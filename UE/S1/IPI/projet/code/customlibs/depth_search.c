# include "breadth_search.h"

/**
 *	@require : un graphe, et un indice de sommet
 *	@ensure  : visite ce sommet, puis se propage recursivement sur les voisins
 *	@assign  : modifie le chemin entre le sommet 'i' et le sommet 's'
 */
static void visit(t_array * nodes, t_bitmap * arcs, INDEX i) {

	/* pour chaque sommet */
	INDEX n = nodes->size;
	INDEX j;
	for (j = 0 ; j < n ; j++) {
		/* si le sommet est voisin de i */
		if (bitmap_get2(arcs, i, j, n)) {
			/* on teste voir si ce chemin est plus court */
			t_node * curr = array_get(nodes, i);
			t_node * next = array_get(nodes, j);
			/** si ce chemin est plus court ... */
			if (curr->pathlen + 1 < next->pathlen) {
				/** on met a jour le chemin */
				next->prev = i;
				next->pathlen = curr->pathlen + 1;

				/** on re-itere alors sur ce voisin */
				visit(nodes, arcs, j);
			}
		}
	}
}

/**
 *	@require : 	'nodes' : un tableau de structure 't_node' (ou en héritant)
 *		 	'arcs' : une bitmap representant les arcs
 *			's' : indice de sommet source
 *	@ensure  : parcours le graphe en profondeur, et trouve le chemin le plus court
 *		   entre chaque sommet et le sommet source
 *	@assign  : modifies le tableau 'nodes' en lui appliquant un algorithme
 *		   de parcours en profondeur sur ses sommets
 */
void depth_search(t_array * nodes, t_bitmap * arcs, INDEX s) {

	/* initilisation: on definie toutes les distantes à +oo (MAX_NODES suffit),
	   sauf pour l'origine à 0 */
	INDEX n = nodes->size;
	INDEX i;
	for (i = 0 ; i < n ; i++) {
		t_node * node = (t_node *) array_get(nodes, i);
		node->pathlen = MAX_NODES;
		node->prev = -1;
	}
	((t_node *)array_get(nodes, s))->pathlen = 0; /* source à 0 */

	/* debut de recursion */
	visit(nodes, arcs, s);
}
