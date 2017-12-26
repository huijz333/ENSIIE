# include "dijkstra.h"

/**
 *	@require : le nombre de sommet 'n' et le sommet source 's'
 *	@ensure  : initialise et renvoie un tableau de 't_nodew' prêt à être
 *			utilisé dans l'algorithme de dijkstra
 *	@assign  : --------------
 */
static void dijkstra_init(t_array * nodes, t_array * unvisited, WEIGHT * pathws, INDEX s) {
	/** pour chaque sommet */
	INDEX n = nodes->size;
	INDEX i;
	for (i = 0 ; i < n ; i++) {
		t_nodew * node = (t_nodew *) array_get(nodes, i);
		node->super.pathlen = 0;

		/** on definit sa distance de 's' à '+oo' */
		pathws[i] = INF_WEIGHT;

		/** on definit ce sommet comme 'non visité' */
		array_add(unvisited, &i);

	}
	
	/* on initialise le sommet 'source' */
	t_nodew * source = (t_nodew *) array_get(nodes, s);
	source->super.pathlen = 1;
	source->super.prev =  s;
	pathws[s] = 0;
}

/**
 *	@require : les sommets du graphe, et le tableau des sommets déjà visité
 *	@ensure  : renvoie l'index du prochain sommet a visité.
 *	@assign  : ----------------------------------------------------
 */
static INDEX dijkstra_next_node(t_array * unvisited, WEIGHT * pathws) {
	/* on prends le 1er sommet non visité */
	INDEX i = 0;
	INDEX uID = *((INDEX *) array_get(unvisited, i));

	/* pour chaque autre sommets non visités */
	INDEX j;
	for (j = 1 ; j < unvisited->size ; j++) {
		INDEX vID = *((INDEX *) array_get(unvisited, j));
		/** si ce sommet est de poids plus faible */
		if (pathws[vID] < pathws[uID]) {
			uID = vID;
			i = j;
		}
	}
	return (i);
}

/**
 *	@require : les sommets du graphe, les arcs et leur poids, et un indice du graphe
 *	@ensure  : construits des chemins plus court entre 's' et les voisins de 'u'
 *	@assign  : ----------------------------------------------------
 */
static void dijkstra_flood_fill(t_array * nodes, WEIGHT * pathws, INDEX uID) {
	t_nodew * u = (t_nodew *) array_get(nodes, uID);
	if (u->super.successors == NULL) {
		return ;
	}

	/* pour chaque successeur de 'u' */
	INDEX i;
	for (i = 0 ; i < u->super.successors->size ; i++) {
		/** successeur 'v' de 'u' */
		INDEX vID = *((INDEX *) array_get(u->super.successors, i));
		t_node * v = (t_node *) array_get(nodes, vID);
		/** poids de l'arc allant de 'u' à 'v' */
		WEIGHT w = *((WEIGHT *)array_get(u->ws, i));
		/** si ce nouveau chemin est de poids plus faible */
		if (pathws[uID] + w < pathws[vID]) {
			/* on ecrase le chemin precedant par le nouveau chemin */
			pathws[vID] = pathws[uID] + w;
			v->prev = uID;
			v->pathlen = u->super.pathlen + 1;
		}
	}
}

/**
 *	@require : une matrice representant les arcs et leur poids, deux indices s et t
 *	@ensure  : resout le plus court chemin dans le graphe entre 's' et 't'.
 *			Renvoie 1 si un chemin a été trouvé, 0 sinon.
 *	@assign  : --------------
 */
int dijkstra(t_array * nodes, INDEX s, INDEX t) {
	
	INDEX n = nodes->size;
	
	/** tableau enregistrant les sommets non visités */
	t_array * unvisited = array_new(n, sizeof(INDEX));
	if (unvisited == NULL) {
		return (0);
	}

	/** tableau enregistrant le poids des chemins trouvés */
	WEIGHT * pathws = (WEIGHT *) malloc(n * sizeof(WEIGHT));
	if (pathws == NULL) {
		array_delete(unvisited);
		return (0);
	}

	/* initialisation des sommets */
	dijkstra_init(nodes, unvisited, pathws, s);

	/* boucle algorithm dijsktra */
	while (unvisited->size > 0) {
		/** on cherche un noeud 'u' non visite minimisant d(u) */
		INDEX i = dijkstra_next_node(unvisited, pathws);
		INDEX uID = *((INDEX *) array_get(unvisited, i));
		array_remove(unvisited, i);

		/* sinon, si on a atteint 't' */
		if (uID == t) {
			/**
			 *  deux posibilités: si le poids du chemin vaux '+oo',
			 *  c'est qu'il se situe dans une autre partie connexe du
			 *  graphe => pas de chemin
			 *  sinon, on a trouvé un chemin
			 */
			array_delete(unvisited);
			free(pathws);
			return (pathws[uID] == INF_WEIGHT ? 0 : 1);
		}

		/* on minimise les chemins des voisins de 'un' */
		dijkstra_flood_fill(nodes, pathws, uID);
	}
	array_delete(unvisited);
	free(pathws);
	return (0);
}
