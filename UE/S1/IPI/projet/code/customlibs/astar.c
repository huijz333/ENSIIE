# include "lab.h"

/** fonction interne qui compare 2 doubles (utile à la file de priorité) */
static int weightcmp(WEIGHT * a, WEIGHT * b) {
	if (*a < *b) {
		return (-1);
	}
	if (*a > *b) {
		return (1);
	}
	return (0);
}

/** heuristique : distance de manhattan */
static WEIGHT heuristic(t_pos v, t_pos t) {
	int vx = (int)v.x;
	int vy = (int)v.y;
	int tx = (int)t.x;
	int ty = (int)t.y;
	return (ABS(tx - vx) + ABS(ty - vy));
}

static void astar_test_path(t_pqueue * visit_queue, t_pqueue_node ** pqueue_nodes,
		t_node * u, t_node * v, t_node * t, WEIGHT w) {
	/** si ce nouveau chemin est de cout plus faible */
	if (u->f_cost + w < v->f_cost) {
		/* on ecrase le chemin precedant par le nouveau */
		v->f_cost = u->f_cost + w;

		/** poids de la fonction d'heuristique */
		v->cost = v->f_cost + heuristic(v->pos, t->pos);
		v->prev = u;
		/** on enregistre les sommets dans la file */
		if (pqueue_nodes[v->index] == NULL) {
			pqueue_nodes[v->index] = pqueue_insert(visit_queue,
								&(v->cost),
								&(v->index));
		} else {
			pqueue_decrease(visit_queue,
					pqueue_nodes[v->index],
					&(v->cost));
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
	t_pqueue * visit_queue = pqueue_new((t_cmpf)weightcmp);
	t_pqueue_node ** pqueue_nodes = (t_pqueue_node **) malloc(sizeof(t_pqueue_node) * n);
	t_node * nodes = (t_node *) malloc(sizeof(t_node) * n);
	if (visit_queue == NULL || nodes == NULL) {
		pqueue_delete(visit_queue);
		free(nodes);
		fprintf(stderr, "Not enough memory\n");
		return (0);
	}
	INDEX sID = sPos.y * lab->width + sPos.x;
	INDEX tID = tPos.y * lab->width + tPos.x;
	t_node * s = nodes + sID;
	t_node * t = nodes + tID;

	
	/** 1. INITIALISATION DE L'ALGORITHME */
	/** pour chaque sommets */
	INDEX x, y;
	INDEX i = 0;
	for (y = 0 ; y < lab->height; y++) {
		for (x = 0 ; x < lab->width ; x++) {
			/** on recupere le sommet (x, y) */
			t_node * node = nodes + i;
			node->pos.x = x;
			node->pos.y = y;
			node->index = i;
			/** pas de predecesseurs */
			node->prev = NULL;
			/** on definit sa distance de 's' à '+oo' */
			node->f_cost = INF_WEIGHT;
			node->cost = INF_WEIGHT;
			node->index = i;

			/** prepare la file de priorité */
			pqueue_nodes[i] = NULL;

			++i;
		}
	}

	/** on initialise le sommet source */
	s->f_cost = 0; /* poids réel du chemin */
	s->cost = heuristic(sPos, tPos);
	pqueue_nodes[sID] = pqueue_insert(visit_queue, &(s->cost), &(s->index));

	/** 2. BOUCLE DE L'ALGORITHME A* */
	/** Tant qu'il y a des sommets a visité, on les visite */
	while (!pqueue_is_empty(visit_queue)) {
		/** 2.1. : on cherche un noeud 'u' non visite minimisant d(u).
		  ceci est optimisé à l'aide d'une file de priorité */
		t_pqueue_node node = pqueue_pop(visit_queue);
		/** l'indice de 'u' */
		INDEX uID = *((INDEX *)node.value);
		t_node * u = nodes + uID;

		/** le sommet n'est plus dans la file */
		pqueue_nodes[uID] = NULL;

		/** si on a atteint 't' avec un cout suffisant,
		  ou si on a atteint une autre partie connexe ... */
		if (t->f_cost <= timer || u->f_cost == INF_WEIGHT) {
			break ;
		}

		/** 2.2 : on minimise les chemins voisins de 'u' */
		/* pour chaque voisin de 'u' */
		BYTE i;
		for (i = 0 ; i < MAX_DIRECTIONS ; i++) {
			/** position de v */
			t_direction d = DIRECTIONS[i];
			/** si on sort de la carte */
			BYTE underx = (u->pos.x == 0)               && (d.x < 0);
			BYTE overx  = (u->pos.x == lab->width - 1)  && (d.x > 0);
			BYTE undery = (u->pos.y == 0)               && (d.y < 0);
			BYTE overy  = (u->pos.y == lab->height - 1) && (d.y > 0);
			if (underx || overx || undery || overy) {
				/** on passe à la direction suivante */
				continue ;
			}

			INDEX vx = u->pos.x + d.x;
			INDEX vy = u->pos.y + d.y;
			/** on regarde la case que l'on souhaite visiter */
			wchar_t c = lab->map[vy][vx];
			/** si c'est un mur, ou une porte (que l'on considère fermée) */
			if (c == LAB_WALL || c == LAB_DOOR) {
				/** on passe à la case suivante */
				continue ;
			}
		
			/** sinon, on est sur une case vide, ou sur la clef */

			/** index de 'v' */
			INDEX vID = vy * lab->width + vx;
			t_node * v = nodes + vID;
			/** on teste ce nouveau chemin */
			astar_test_path(visit_queue, pqueue_nodes, u, v, t, 1);
		}
		/** si c'est un teleporteur, alors on essaye le chemin
		    vers l'autre teleporteur */
		BYTE tpID = lab_get_tpID(lab->map[u->pos.y][u->pos.x]);
		if (tpID != MAX_TP) {
			/** on recupere les 2 cases du teleporteurs */
			t_pos * tp = lab->tps[tpID];
			INDEX tp0ID = tp[0].y * lab->width + tp[0].x;
			INDEX tp1ID = tp[1].y * lab->width + tp[1].x;
			INDEX vID;
			/** si 'u' correspond à la 1ere case */
			if (uID == tp0ID) {
				/** alors 'v' est la 2ème */
				vID = tp1ID;
			} else {
			/** sinon, 'u' est la 2ème et 'v' la 1ère */
				vID = tp0ID;
			}
			t_node * v = nodes + vID;
			/** on teste ce nouveau chemin */
			astar_test_path(visit_queue, pqueue_nodes, u, v, t, 0);
		}
	}

	/** plus besoin de la file de priorité: on libere la mémoire */
	pqueue_delete(visit_queue);
	free(pqueue_nodes);

	/** on crée le chemin allant de 's' à 't' */
	if (t->f_cost <= timer) {
		lab_print_path(lab, s, t);
		free(nodes);
		return (1);
	}
	/** libère la mémoire */
	printf("Not connected\n");
	free(nodes);
	return (0);
}
