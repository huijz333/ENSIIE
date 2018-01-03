# include "lab.h"

/** fonction interne qui compare 2 doubles (utile à la file de priorité) */
static int weightcmp(size_t * a, size_t * b) {
	if (*a < *b) {
		return (-1);
	}
	if (*a > *b) {
		return (1);
	}
	return (0);
}

# ifndef ABS
#	define ABS(X) ((X) < 0 ? -(X) : (X))
# endif


/** heuristique de manhattan */
static size_t heuristic(t_pos v, t_pos t) {
	int vx = (int)v.x;
	int vy = (int)v.y;
	int tx = (int)t.x;
	int ty = (int)t.y;
	return (ABS(tx - vx) + ABS(ty - vy));
}

/**
 *	@require : 	'nodes':	un tableau de sommet
 *			'heuristic':	la fonction d'heuristique
 *			's':		un indice sommet source
 *			't':		un indice de sommet de destination
 *	@ensure  : resout le plus court chemin dans le graphe entre 's' et 't'.
 *			Renvoie 1 si un chemin a été trouvé, 0 sinon.
 *	@assign  : 'nodes': les attributs des sommets peuvent être modifié
 */
static t_list * lab_astar(t_lab * lab, t_pos sPos, t_pos tPos, size_t timer) {
	INDEX n = lab->width * lab->height;
	
	/** file de priorité pour déterminer le prochain sommet à visiter */
	t_pqueue * unvisited = pqueue_new((t_cmpf)weightcmp);
	t_pqueue_node ** pqueue_nodes = (t_pqueue_node **) malloc(sizeof(t_pqueue_node) * n);
	t_node * nodes = (t_node *) malloc(sizeof(t_node) * n);
	if (unvisited == NULL || nodes == NULL) {
		pqueue_delete(unvisited);
		free(nodes);
		fprintf(stderr, "Not enough memory\n");
		return (NULL);
	}
	INDEX sID = sPos.y * lab->width + sPos.x;
	INDEX tID = tPos.y * lab->width + tPos.x;

	
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
			node->g_cost = INF_WEIGHT;
			node->index = i;
			
			/** prepare la file de priorité */
			pqueue_nodes[i] = NULL;

			++i;
		}
	}

	/** on initialise le sommet source */
	t_node * s = nodes + sID;
	s->f_cost = 0; /* poids réel du chemin */
	s->g_cost = heuristic(sPos, tPos);
	pqueue_nodes[sID] = pqueue_insert(unvisited, &(s->g_cost), &(s->index));

	t_node * t = nodes + tID;

	/** 2. BOUCLE DE L'ALGORITHME A* */
	/** Tant qu'il y a des sommets a visité, on les visite */
	while (!pqueue_is_empty(unvisited)) {
		/** 2.1. : on cherche un noeud 'u' non visite minimisant d(u).
		  ceci est optimisé à l'aide d'une file de priorité */
		t_pqueue_node node = pqueue_pop(unvisited);
		/** l'indice de 'u' */
		INDEX uID = *((INDEX *)node.value);
		t_node * u = nodes + uID;

		/** le sommet n'est plus dans la file */
		pqueue_nodes[uID] = NULL;

		/** si on a atteint 't' avec un cout suffisant,
		    ou si on a atteint une autre partie connexe ... */
		if (t->f_cost < timer || u->f_cost == INF_WEIGHT) {
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
			BYTE inWall = lab->map[vy][vx] == LAB_WALL;
			/** si on est dans un mur */
			if (inWall) {
				/** on passe à la direction suivante */
				continue ;
			}

			/** index de 'v' */
			INDEX vID = vy * lab->width + vx;
			t_node * v = nodes + vID;
			
			/** si ce nouveau chemin est de cout plus faible */
			if (u->f_cost + 1 < v->f_cost) {
				/* on ecrase le chemin precedant par le nouveau chemin */
				v->f_cost = u->f_cost + 1;

				/** poids de la fonction d'heuristique */
				v->g_cost = heuristic(v->pos, t->pos);
				v->prev = u;
				/** on enregistre les sommets dans la file de priorité */
				if (pqueue_nodes[vID] == NULL) {
					pqueue_nodes[vID] = pqueue_insert(unvisited,
									&(v->g_cost),
									&(v->index));
				} else {
					pqueue_decrease(unvisited,
							pqueue_nodes[vID],
							&(v->g_cost));
				}
			}
		}
		/** si c'est un teleporteur, alors crée un chemin vers l'autre teleporteur */
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
			/** si ce nouveau chemin est de cout plus faible */
			if (u->f_cost < v->f_cost) {
				/* on ecrase le chemin precedant par le nouveau chemin */
				v->f_cost = u->f_cost;

				/** poids de la fonction d'heuristique */
				v->g_cost = heuristic(v->pos, t->pos);
				v->prev = u;
				/** on enregistre les sommets dans la file de priorité */
				if (pqueue_nodes[vID] == NULL) {
					pqueue_nodes[vID] = pqueue_insert(unvisited,
									&(v->g_cost),
									&(v->index));
				} else {
					pqueue_decrease(unvisited,
							pqueue_nodes[vID],
							&(v->g_cost));
				}
			}
		}

	}

	/** plus besoin de la file de priorité: on libere la mémoire */
	pqueue_delete(unvisited);
	free(pqueue_nodes);

	/** on crée le chemin allant de 's' à 't' */
	t_list * path;
	if (t->f_cost > timer) {
		path = NULL;
	} else {
		path = list_new();
		while (t != s) {
			list_push(path, &(t->pos), sizeof(t_pos));
			t = t->prev;
		}
		list_push(path, &sPos, sizeof(t_pos));
	}
	/** libère la mémoire */
	free(nodes);
	return (path);
}

static void lab_print_path(t_lab * lab, t_list * path) {
	t_list_node * first = path->head->next;
	t_list_node * second = path->head->next->next;
	while (second != path->head) {
		t_pos * u = (t_pos *)(first + 1);
		t_pos * v = (t_pos *)(second + 1);
		if (lab->map[u->y][u->x] == lab->map[v->y][v->x]
				&& lab->map[u->y][u->x] != LAB_EMPTY) {
			printf("TP\n");
		} else {
			BYTE i;
			int dx = (int)v->x - (int)u->x;
			int dy = (int)v->y - (int)u->y;
			for (i = 0 ; i < MAX_DIRECTIONS ; i++) {
				t_direction d = DIRECTIONS[i];
				if (dx == d.x && dy == d.y) {
					printf("%s\n", d.name);
					break ;
				}
			}
		}
		first = first->next;
		second = second->next;
	}
}


# define PATH_DIRECT	(0)
# define PATH_KEY	(1)
# define PATH_DOOR	(2)

int lab_solve(t_lab * lab, size_t timer) {

	/** s'il n'y a pas de portes */
	if (lab->door.x == MAX_NODES) {
		/** on résout */
		t_list * path = lab_astar(lab, lab->entry, lab->exit, timer);
		if (path == NULL) {
			printf("Not connected\n");
			return (0);
		}
		lab_print_path(lab, path);
		list_delete(path);
		return (1);
	}


	/** sinon, on essaye de résoudre en parallèle sans passer par la porte,
	    et en passant par la porte */
	int pipeDirect[2], pipeKey[2], pipeDoor[2];
	pipe(pipeDirect);
	pipe(pipeKey);
	pipe(pipeDoor);

	pid_t childPID = fork();
	if (childPID) {
		/** check path */
		/** on résout */
		t_list * path = lab_astar(lab, lab->entry, lab->exit, timer);
		if (path != NULL) {
			/* TODO : stop others */
			kill(childPID, SIGKILL);
			puts("killed from parent");
			lab_print_path(lab, path);
			list_delete(path);
		}
	} else {
		/** s'il y a une porte, on essaye de résoudre en passant par la porte */
		pid_t key = fork();
		if (key) {
			lab_delete(lab);
			exit(EXIT_SUCCESS);
		} else {
			lab_delete(lab);
			exit(EXIT_SUCCESS);
		}
	}

	/* TODO : wait key and door, if failed, then no 'Not connected'*/
	
	return (1);
}


