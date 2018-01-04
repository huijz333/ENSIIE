# include "astar_worker.h"

/** l'état du processus */
t_worker worker;

/** strucutres utile à la recherche */
/** variable globale pour pouvoir être free dans le callback du signal */
t_lab		* lab;
t_pqueue	* visit_queue;
t_pqueue_node	** pqueue_nodes;
t_node		* nodes;
t_node		* s;
t_node		* t;

/** entrée du pipe */
int fd;

/**
 *	@require : un ID de packet, et un temps
 *	@ensure  : envoie le packet au père
 *	@assign  : --------------------
 */
static void sendPacket(BYTE packetID, WEIGHT time) {
	t_packet packet;
	memset(&packet, 0, sizeof(t_packet));
	packet.id = packetID;
	packet.workerID = worker.id;
	packet.time = time;
	write(fd, &packet, sizeof(t_packet));
}

/** callback pour le signal indiquant au processus d'afficher
    son résultat et de se stopper */
static void sig_print(int signum) {
	(void)signum;
	worker.state = WORKER_STATE_PRINT;
}

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

static void astar_test_path(t_node * u, t_node * v, t_node * t, WEIGHT w) {
	/** si ce nouveau chemin est de cout plus faible */
	if (u->f_cost + w < v->f_cost) {
		/* on ecrase le chemin precedant par le nouveau */
		v->f_cost = u->f_cost + w;

		/** poids de la fonction d'heuristique */
		v->cost = v->f_cost + heuristic(v->pos, t->pos);
		v->prev = u;

		/* si on a mis 't' à jour, on notifie le pere */
		if (v == t) {
			/** on crée et on envoit le packet */
			sendPacket(PACKET_ID_PATHTIME, t->f_cost);
		}
		/** on enregistre les sommets dans la file */
		if (pqueue_nodes[v->index] == NULL) {
			pqueue_nodes[v->index] = pqueue_insert(visit_queue, &(v->cost), &(v->index));
		} else {
			pqueue_decrease(visit_queue, pqueue_nodes[v->index], &(v->cost));
		}
	}
}

/**
 *	@require : 	'lab':	le labyrinthe
 *			'sPos':	la position de départ
 *			'tPos':	la position d'arrivé
 *			'p':	le pipe dans lequel écrire les packets
 *			'workerID': l'id de l'enfant (voir WORKER_* (astar_worker.h))
 *	@ensure  : crée un nouveau processus qui cherche un chemin entre 's' et 't',
 *                 dans le labyrinthe, et qui écrit la longueur du chemin
 *		   dans le pipe 'p',
 *		   renvoie le pid du processus fils créé.
 *		   le processus se suicide à la fin de la recherche
 *	@assign  : --------------------------------------------
 */
t_worker astar_worker(t_lab * theLab, t_pos sPos, t_pos tPos, int p[2], BYTE workerID) {
	/** creation du processus fils */
	worker.pid	= fork();
	worker.id	= workerID;
	worker.time	= INF_WEIGHT;
	worker.state	= WORKER_STATE_RUNNING;
	/** si on est dans le père, on renvoit le worker */
	if (worker.pid) {
		return (worker);
	}

	/** sinon, l'enfant tente de résoudre */
	/** prepare la communication inter-processus */
	signal(SIG_PRINT, sig_print);
	close(p[0]);
	fd = p[1];

	/** si on va de la clef à la porte, ou de la porte à la sortie
	    on considère que l'on a la clef en poche */
	BYTE hasKey = (workerID == WORKER_a_A) || (workerID == WORKER_A_S);

	/** nombre de case dans le labyrinthe */
	lab = theLab;
	INDEX n = lab->width * lab->height;
	
	/** file de priorité pour déterminer le prochain sommet à visiter */
	visit_queue = pqueue_new((t_cmpf)weightcmp);
	pqueue_nodes = (t_pqueue_node **) malloc(sizeof(t_pqueue_node) * n);
	nodes = (t_node *) malloc(sizeof(t_node) * n);
	if (visit_queue == NULL || pqueue_nodes == NULL || nodes == NULL) {
		fprintf(stderr, "Not enough memory (%d)\n", getpid());
		pqueue_delete(visit_queue);
		free(pqueue_nodes);
		free(nodes);
		lab_delete(lab);
		exit(EXIT_FAILURE);
	}

	/** id dans le tableau 'nodes' des sommets 's' et 't' */
	INDEX sID = sPos.y * lab->width + sPos.x;
	INDEX tID = tPos.y * lab->width + tPos.x;
	s = nodes + sID;
	t = nodes + tID;
	
	/** prepare la communication inter-processus */
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
	/** Tant que la recherche doit s'affiner,
	    et qu'il y a des sommets a visité, on les visite */
	while (worker.state == WORKER_STATE_RUNNING && !pqueue_is_empty(visit_queue)) {
		/** 2.1. : on cherche un noeud 'u' non visite minimisant d(u).
		  ceci est optimisé à l'aide d'une file de priorité */
		t_pqueue_node node = pqueue_pop(visit_queue);
		/** l'indice de 'u' */
		INDEX uID = *((INDEX *)node.value);
		t_node * u = nodes + uID;

		/** le sommet n'est plus dans la file */
		pqueue_nodes[uID] = NULL;

		/** si on a atteint une autre partie connexe ... */
		if (u->f_cost == INF_WEIGHT) {
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
			/** si c'est un mur, ou une porte fermée */
			if (c == LAB_WALL || (c == LAB_DOOR && !hasKey)) {
				/** on passe à la case suivante */
				continue ;
			}
		
			/** sinon, on est sur une case vide, ou sur la clef */
			
			/** index de 'v' */
			INDEX vID = vy * lab->width + vx;
			t_node * v = nodes + vID;
			/** on teste ce nouveau chemin */
			astar_test_path(u, v, t, 1);
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
			astar_test_path(u, v, t, 0);
		}
	}
	
	/** libere la mémoire */
	pqueue_delete(visit_queue);
	free(pqueue_nodes);


	/** on notifie le pere qu'on a fini */
	sendPacket(PACKET_ID_ENDED, INF_WEIGHT);

	/** si le processus a calculé tous les chemins */
	if (worker.state == WORKER_STATE_RUNNING) {
	/** sinon, c'est qu'on ne pourra pas trouvé un chemin
	    plus court, on attends la réponse du père */
		worker.state = WORKER_STATE_ENDED;

		/** on attends un signal du père,
		  savoir si le chemin doit être affiché ou non */
		/** NB: ceci pourrait être remplacé par un autre
		    pipe, où le fils lit une réponse du pere.
		    J'ai preféré le faire sous forme de signal */
		while (worker.state == WORKER_STATE_ENDED);
	}
	/** affiche le resultat */
	lab_print_path(lab, s, t);
	/**  libère la mémoire */
	free(nodes);
	lab_delete(lab);
	/** fin du processus */
	exit(EXIT_SUCCESS);
	return (worker);
}
