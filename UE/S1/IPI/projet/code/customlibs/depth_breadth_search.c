# include "depth_breadth_search.h"

/**
 *	@require : un graphe, et un indice de sommet
 *	@ensure  : visite ce sommet, puis se propage recursivement sur les voisins
 *	@assign  : modifie le chemin entre le sommet 'i' et le sommet 's'
 */
static void visit(t_bitmap * arcs, t_node * nodes, INDEX i) {

	/* pour chaque sommet */
	INDEX j;
	for (j = 0 ; j < arcs->n ; j++) {
		/* si le sommet est voisin de i */
		if (bitmap_get(arcs, i, j)) {
			/* on teste voir si ce chemin est plus court */
			t_node * curr = nodes + i;
			t_node * next = nodes + j;
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
 *		   un tableau de sommet correspondant au resultat
 *	@assign  : ---------------------------------
 */
t_node * depth_breadth_search(t_bitmap * arcs, INDEX s) { 
	t_node * nodes = (t_node *) malloc(arcs->n * sizeof(t_node));
	if (nodes == NULL) {
		return (NULL);
	}
	
	/* initilisation: on definie toutes les distantes à +oo, sauf pour l'origine à 0 */
	INDEX i;
	for (i = 0 ; i < arcs->n ; i++) {
		nodes[i].pathlen = MAX_NODES;
	}
	nodes[s].pathlen = 0;

	/* debut de recursion */
	visit(arcs, nodes, s);
	return (nodes);
}
