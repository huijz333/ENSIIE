# include "dijkstra.h"

/**
 *	@require : le nombre de sommet 'n' et le sommet source 's'
 *	@ensure  : initialise et renvoie un tableau de 't_nodew' prêt à être
 *			utilisé dans l'algorithme de dijkstra
 *	@assign  : --------------
 */
static void dijkstra_init(t_array * nodes, INDEX s) {
	/** on definit toutes les distances de 's' sur '+oo' */
	INDEX n = nodes->size;
	INDEX i;
	for (i = 0 ; i < n ; i++) {
		t_nodew * node = (t_nodew *) array_get(nodes, i);
		node->super.pathlen = 0;
		node->pathw = UINT_MAX;
	}
	
	/* on initialise le sommet 'source' */
	t_nodew * source = (t_nodew *) array_get(nodes, s);
	source->super.pathlen = 1;
	source->super.prev =  s;
	source->pathw = 0;
}

/**
 *	@require : les sommets du graphe, et le tableau des sommets déjà visité
 *	@ensure  : renvoie l'index du prochain sommet a visité.
 *	@assign  : ----------------------------------------------------
 */
static INDEX dijkstra_next_nodew(t_array * nodes, t_array * unvisited) {
	/* on prends le 1er sommet non visité */
	INDEX i = 0;
	INDEX u = *((INDEX *) array_get(unvisited, i));
	t_nodew * un = (t_nodew *) array_get(nodes, u);

	/* pour chaque autre sommets non visités */
	INDEX j;
	for (j = 1 ; j < unvisited->size ; j++) {
		INDEX v = *((INDEX *) array_get(unvisited, j));
		t_nodew * vn = (t_nodew *) array_get(nodes, v);
		/** si ce sommet est de poids plus faible */
		if (vn->pathw < un->pathw) {
			u = v;
			un = vn;
			i = j;
		}
	}
	array_remove(unvisited, i);
	return (u);
}

/**
 *	@require : les sommets du graphe, les arcs et leur poids, et un indice du graphe
 *	@ensure  : construits des chemins plus court entre 's' et les voisins de 'u'
 *	@assign  : ----------------------------------------------------
 */
static void dijkstra_flood_fill(t_array * nodes, t_matrix * ws, INDEX u) {
	/* pour chaque sommet */
	t_nodew * un = (t_nodew *) array_get(nodes, u);
	INDEX v;
	for (v = 0 ; v < ws->n ; v++) {
		/** s'il est voisin de 'u' ET si ce nouveau
		 *  chemin passant par 'u' est plus court */
		int wuv = matrix_get(ws, u, v);
		t_nodew * vn = (t_nodew *) array_get(nodes, v);
		if (wuv >= 0 && un->pathw + wuv < vn->pathw) {
			/* on ecrase le chemin precedant par le nouveau chemin */
			vn->pathw = un->pathw + wuv;
			vn->super.prev = u;
			vn->super.pathlen = un->super.pathlen + 1;
		}
	}
}

/**
 *	@require : une matrice representant les arcs et leur poids, deux indices s et t
 *	@ensure  : resout le plus court chemin dans le graphe entre 's' et 't'.
 *			Renvoie 1 si un chemin a été trouvé, 0 sinon.
 *	@assign  : --------------
 */
int dijkstra(t_array * nodes, t_matrix * ws, INDEX s, INDEX t) {
	/* initialisation des sommets */
	INDEX n = nodes->size;
	dijkstra_init(nodes, s);

	/* tableau enregistrant les sommets non visités */
	t_array * unvisited = array_new(n, sizeof(INDEX));
	INDEX i;
	for (i = 0 ; i < n ; i++) {
		array_add(unvisited, &i);
	}

	/* boucle algorithm dijsktra */
	while (unvisited->size > 0) {
		/** on cherche un noeud 'u' non visite minimisant d(u) */
		INDEX u = dijkstra_next_nodew(nodes, unvisited);
		t_nodew * un = (t_nodew *) array_get(nodes, u);

		/* sinon, si on a atteint 't' */
		if (u == t) {
			/**
			 *  deux posibilités: si le poids du chemin vaux '+oo',
			 *  c'est qu'il se situe dans une autre partie connexe du
			 *  graphe => pas de chemin
			 *  sinon, on a trouvé un chemin
			 */
			array_delete(unvisited);
			return (un->pathw == UINT_MAX ? 0 : 1);
		}

		/* on minimise les chemins des voisins de 'un' */
		dijkstra_flood_fill(nodes, ws, u);
	}
	array_delete(unvisited);
	return (0);
}
