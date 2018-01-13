# include "lab.h"

/** fonction interne qui compare 2 doubles (utile à la file de priorité) */
static int weightcmp(WEIGHT * a, WEIGHT * b) {
	return ((*a < *b) ? -1 : (*a > *b) ? 1 : 0);
}

/** heuristique : distance de manhattan */
static WEIGHT heuristic(t_pos v, t_pos t) {
	return (ABS((int)t.x - (int)v.x) + ABS((int)t.y - (int)v.y));
}

/** fonction interne, minimise le chemin de 's' à 'v', en regardant
  si celui passant par 'u' et l'arc de poids 'w' est plus court */
static void astar_test_path(t_pqueue * visitQueue, t_node * nodes,
		t_pqueue_node ** pqueue_nodes, WEIGHT w,
		INDEX u, INDEX v, INDEX t) {
	/** si ce nouveau chemin est de cout plus faible */
	if (nodes[u].f_cost + w < nodes[v].f_cost) {
		/* on ecrase le chemin precedant par le nouveau */
		nodes[v].f_cost = nodes[u].f_cost + w;

		/** poids de la fonction d'heuristique */
		nodes[v].cost = nodes[v].f_cost + heuristic(nodes[v].pos, nodes[t].pos);
		nodes[v].prev = u;
		/** on enregistre les sommets dans la file */
		if (pqueue_nodes[v] == NULL) {
			pqueue_nodes[v] = pqueue_insert(visitQueue, &(nodes[v].cost), &(nodes[v].index));
		} else {
			pqueue_decrease(visitQueue, pqueue_nodes[v], &(nodes[v].cost));
		}
	}
}

/**
 *	@require : 	'lab':		le labyrinthe
 *			'sPos':		la position de départ
 *			'tPos':		la position d'arrivé
 *			'timer':	le temps maximal de résolution voulu
 *	@ensure  : resout le plus court chemin dans le graphe entre 's' et 't'.
 *		   Renvoie 1 et affiche le chemin, si un chemin suffisement
 *		   court a été trouvé, sinon renvoie 0 et 'Not connected' est affiché
 *	@assign  : --------------------------------------------
 */
int astar(t_lab * lab, t_pos sPos, t_pos tPos, WEIGHT timer) {
	/** nombre de case dans le labyrinthe */
	INDEX n = lab->width * lab->height;
	
	/** file de priorité pour déterminer le prochain sommet à visiter */
	t_pqueue * visitQueue = pqueue_new((t_cmpf)weightcmp);
	t_pqueue_node ** pqueue_nodes = (t_pqueue_node **) malloc(sizeof(t_pqueue_node) * n);
	t_node * nodes = (t_node *) malloc(sizeof(t_node) * n);
	if (visitQueue == NULL || nodes == NULL) {
		pqueue_delete(visitQueue);
		free(nodes);
		fprintf(stderr, "Not enough memory\n");
		return (0);
	}
	/** indice de 's' et 't' dans le tableau 'nodes' */
	INDEX s = sPos.y * lab->width + sPos.x;
	INDEX t = tPos.y * lab->width + tPos.x;

	/** 1. INITIALISATION DE L'ALGORITHME */
	/** pour chaque sommets */
	INDEX x, y;
	INDEX i = 0;
	for (y = 0 ; y < lab->height; y++) {
		for (x = 0 ; x < lab->width ; x++) {
			/** on recupere le sommet (x, y) */
			nodes[i].pos.x = x;
			nodes[i].pos.y = y;
			/** on a besoin de stocker l'index pour la file de priorité */
			nodes[i].index = i; 
			/** pas de predecesseurs */
			nodes[i].prev = MAX_NODES;
			/** on definit sa distance de 's' à '+oo' */
			nodes[i].f_cost = INF_WEIGHT;
			nodes[i].cost = INF_WEIGHT;

			/** prepare la file de priorité */
			pqueue_nodes[i] = NULL;

			++i;
		}
	}

	/** on initialise le sommet source */
	nodes[s].f_cost = 0; /* poids réel du chemin */
	nodes[s].cost = heuristic(sPos, tPos);
	pqueue_nodes[s] = pqueue_insert(visitQueue, &(nodes[s].cost), &(nodes[s].index));

	/** 2. BOUCLE DE L'ALGORITHME A* */
	/** Tant qu'il y a des sommets a visité, on les visite */
	while (!pqueue_is_empty(visitQueue)) {
		/** 2.1. : on cherche un noeud 'u' non visite minimisant d(u).
		  ceci est optimisé à l'aide d'une file de priorité */
		t_pqueue_node node = pqueue_pop(visitQueue);
		/** l'indice de 'u' */
		INDEX u = *((INDEX *)node.value);
		/** le sommet n'est plus dans la file */
		pqueue_nodes[u] = NULL;

		/** si on a atteint 't' avec un cout suffisant,
		  ou si on a atteint une autre partie connexe ... */
		if (nodes[t].f_cost <= timer || nodes[u].f_cost == INF_WEIGHT) {
			break ;
		}

		/** 2.2 : on minimise les chemins voisins de 'u' */
		/* pour chaque voisin de 'u' */
		BYTE i;
		for (i = 0 ; i < MAX_DIRECTIONS ; i++) {
			/** position de v */
			t_direction d = DIRECTIONS[i];
			/** si on sort de la carte */
			BYTE underx = (nodes[u].pos.x == 0)               && (d.x < 0);
			BYTE overx  = (nodes[u].pos.x == lab->width - 1)  && (d.x > 0);
			BYTE undery = (nodes[u].pos.y == 0)               && (d.y < 0);
			BYTE overy  = (nodes[u].pos.y == lab->height - 1) && (d.y > 0);
			if (underx || overx || undery || overy) {
				/** on passe à la direction suivante */
				continue ;
			}

			INDEX vx = nodes[u].pos.x + d.x;
			INDEX vy = nodes[u].pos.y + d.y;
			/** on regarde la case que l'on souhaite visiter */
			wchar_t c = lab->map[vy][vx];
			/** si c'est un mur, ou une porte (que l'on considère fermée) */
			if (c == LAB_WALL || c == LAB_DOOR) {
				/** on passe à la case suivante */
				continue ;
			}
		
			/** sinon, on est sur une case vide, ou sur la clef */

			/** index de 'v' */
			INDEX v = vy * lab->width + vx;
			/** on teste ce nouveau chemin */
			astar_test_path(visitQueue, nodes, pqueue_nodes, 1, u, v, t);
		}
		/** si on est sur un teleporteur, alors on essaye
		    le chemin vers l'autre teleporteur */
		BYTE tpID = lab_get_tpID(lab->map[nodes[u].pos.y][nodes[u].pos.x]);
		if (tpID != MAX_TP) {
			/** on recupere les 2 cases du teleporteurs */
			t_pos * tp = lab->tps[tpID];
			INDEX tp0 = tp[0].y * lab->width + tp[0].x;
			INDEX tp1 = tp[1].y * lab->width + tp[1].x;
			INDEX v;
			/** si 'u' correspond à la 1ere case */
			if (u == tp0) {
				/** alors 'v' est la 2ème */
				v = tp1;
			} else {
			/** sinon, 'u' est la 2ème et 'v' la 1ère */
				v = tp0;
			}
			/** on teste ce nouveau chemin */
			astar_test_path(visitQueue, nodes, pqueue_nodes, 0, u, v, t);
		}
	}

	/** plus besoin de la file de priorité: on libere la mémoire */
	pqueue_delete(visitQueue);
	free(pqueue_nodes);

	/** on crée le chemin allant de 's' à 't' */
	if (nodes[t].f_cost <= timer) {
		lab_print_path(lab, nodes, s, t);
		free(nodes);
		return (1);
	}
	/** libère la mémoire */
	printf("Not connected\n");
	free(nodes);
	return (0);
}
