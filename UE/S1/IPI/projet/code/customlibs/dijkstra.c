# include "dijkstra.h"

/**
 *	@require : 	'nodes' :    	- le tableau des sommets
 *			'unvisited': 	- le tableau des sommets non visités
 *			'pathws':    	- le tableau des poids totaux des chemins
 *				    	de 's' vers les sommets
 *			's':		- le sommet source
 *	@ensure  : initialise les arguments pour préparer l'algorithme de dijkstra
 *	@assign  : 	'unvisited' est rempli de tous les sommets
 *			'pathws' voit tous ses poids mis à '+oo'
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
	pathws[s] = 0;
}

/**
 *	@require :	'unvisited': 	- le tableau des sommets non visités
 *			'pathws':    	- le tableau des poids totaux des chemins
 *	@ensure  : renvoie l'indice dans le tableau 'unvisited' du
 /		   du prochain sommet à visiter
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
 *	
 *	@require :	'nodes': 	- le tableau des sommets du graphe
 *			'pathws':    	- le tableau des poids totaux des chemins
 *			'uID':		- le numéro du sommet que l'on visite
 *	@ensure  : applique un algorithme de propagation sur le sommet 'uID':
 *			pour chacun de ses successeurs, on essaye de construire
 *			un chemin plus court venant de 'u'
 *	@assign  : 'pathws' peut être modifié au index des successeurs de 'u'
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
 *	@require : 	'nodes':	un tableau de sommet
 *			's':		un indice sommet source
 *			't':		un indice de sommet de destination
 *	@ensure  : resout le plus court chemin dans le graphe entre 's' et 't'.
 *			Renvoie 1 si un chemin a été trouvé, 0 sinon.
 *	@assign  : 'nodes': les attributs des sommets peuvent être modifié
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
			WEIGHT pathw = pathws[uID];
			free(pathws);
			return (pathw == INF_WEIGHT ? 0 : 1);
		}

		/* on minimise les chemins des voisins de 'un' */
		dijkstra_flood_fill(nodes, pathws, uID);
	}
	array_delete(unvisited);
	free(pathws);
	return (0);
}
