# include "depth_breadth_search.h"

/**
 *	@require : un graphe, et un indice de sommet
 *	@ensure  : visite ce sommet, puis se propage recursivement sur les voisins
 *	@assign  : modifie le chemin entre le sommet 'i' et le sommet 's'
 */
static void visit(t_bitmap * arcs, t_array * nodes, INDEX i) {

	/* pour chaque sommet */
	INDEX j;
	for (j = 0 ; j < arcs->n ; j++) {
		/* si le sommet est voisin de i */
		if (bitmap_get(arcs, i, j)) {
			/* on teste voir si ce chemin est plus court */
			t_node * curr = array_get(nodes, i);
			t_node * next = array_get(nodes, j);
			/** si ce chemin est plus court ... */
			if (curr->pathlen + 1 < next->pathlen) {
				/** on met a jour le chemin */
				next->prev = i;
				next->pathlen = curr->pathlen + 1;

				/** on re-itere alors sur ce voisin */
				visit(arcs, nodes, j);
			}
		}
	}
}

/**
 *	@require : une bitmap representant les arcs, et un indice de sommet source
 *	@ensure  : parcours le graphe en largeur, et trouve le chemin le plus court
 *		   entre chaque sommet et le sommet source. Renvoie
 *		   un tableau de 't_node', correspondant au resultat
 *	@assign  : ---------------------------------
 */
t_array * depth_breadth_search(t_bitmap * arcs, INDEX s) {
	t_array * nodes = array_new(arcs->n, sizeof(t_node));
	if (nodes == NULL) {
		return (NULL);
	}
	
	/* initilisation: on definie toutes les distantes à +oo (MAX_NODES suffit),
	   sauf pour l'origine à 0 */
	t_node init;
	init.pathlen = MAX_NODES;
	init.prev = -1;
	array_addn(nodes, &init, arcs->n); /* ajoutes 'n' sommets vide */
	((t_node *)array_get(nodes, s))->pathlen = 0; /* source à 0*/

	/* debut de recursion */
	visit(arcs, nodes, s);
	return (nodes);
}
