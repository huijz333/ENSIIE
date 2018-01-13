# include "astar_client.h"

/** l'état du processus */
t_client client;

/** strucutres utile à la recherche */
/** variable globale pour pouvoir être free dans le callback du signal */
t_lab		* lab;
t_pqueue	* visitQueue;
t_pqueue_node	** pqueue_nodes;
t_node		* nodes;
t_node		* s;
t_node		* t;

/** un boolean qui vaux 1 si l'enfant doit écrire son résultat */
BYTE shouldPrint = 0;

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
	packet.clientID = client.id;
	packet.time = time;
	write(fd, &packet, sizeof(t_packet));
}

/** callback pour le signal indiquant au processus d'afficher
    son résultat et de se stopper */
static void sig_print(int signum) {
	(void)signum;
	shouldPrint = 1;
}

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
static void astar_test_path(INDEX u, INDEX v, INDEX t, WEIGHT w) {
	/** si ce nouveau chemin est de cout plus faible */
	if (nodes[u].f_cost + w < nodes[v].f_cost) {
		/* on ecrase le chemin precedant par le nouveau */
		nodes[v].f_cost = nodes[u].f_cost + w;

		/** poids de la fonction d'heuristique */
		nodes[v].cost = nodes[v].f_cost + heuristic(nodes[v].pos, nodes[t].pos);
		nodes[v].prev = u;

		/* si on a mis 't' à jour, on notifie le pere */
		if (v == t) {
			/** on crée et on envoit le packet */
			sendPacket(PACKET_ID_PATHTIME, nodes[t].f_cost);
		}

		/** on enregistre les sommets dans la file */
		if (pqueue_nodes[v] == NULL) {
			pqueue_nodes[v] = pqueue_insert(visitQueue, &(nodes[v].cost), &(nodes[v].index));
		} else {
			pqueue_decrease(visitQueue, pqueue_nodes[v], &(nodes[v].cost));
		}
	}
}

/**
 *	@require : 	'lab':	le labyrinthe
 *			'sPos':	la position de départ
 *			'tPos':	la position d'arrivé
 *			'p':	le pipe dans lequel écrire les packets
 *			'clientID': l'id de l'enfant (voir CLIENT_* (astar_client.h))
 *	@ensure  : crée un nouveau processus qui cherche un chemin entre 's' et 't',
 *                 dans le labyrinthe, et qui écrit la longueur du chemin
 *		   dans le pipe 'p',
 *		   renvoie le pid du processus fils créé.
 *		   le processus se suicide à la fin de la recherche
 *	@assign  : --------------------------------------------
 */
t_client astar_client(t_lab * theLab, t_pos sPos, t_pos tPos, int p[2], BYTE clientID) {
	/** creation du processus fils */
	client.pid	= fork();
	client.id	= clientID;
	client.time	= INF_WEIGHT;
	/** si on est dans le père, on renvoit le client */
	if (client.pid) {
		return (client);
	}

	/** sinon, l'enfant tente de résoudre */
	/** prepare la communication inter-processus */
	signal(SIG_PRINT, sig_print);
	close(p[0]);
	fd = p[1];

	/** si on va de la clef à la porte, ou de la porte à la sortie
	    on considère que l'on a la clef en poche */
	BYTE hasKey = (clientID == CLIENT_C_P) || (clientID == CLIENT_P_S);

	/** nombre de case dans le labyrinthe */
	lab = theLab;
	INDEX n = lab->width * lab->height;
	
	/** file de priorité pour déterminer le prochain sommet à visiter */
	visitQueue = pqueue_new((t_cmpf)weightcmp);
	pqueue_nodes = (t_pqueue_node **) malloc(sizeof(t_pqueue_node) * n);
	nodes = (t_node *) malloc(sizeof(t_node) * n);
	if (visitQueue == NULL || pqueue_nodes == NULL || nodes == NULL) {
		fprintf(stderr, "Not enough memory (%d)\n", getpid());
		pqueue_delete(visitQueue);
		free(pqueue_nodes);
		free(nodes);
		lab_delete(lab);
		exit(EXIT_FAILURE);
	}

	/** id dans le tableau 'nodes' des sommets 's' et 't' */
	INDEX s = sPos.y * lab->width + sPos.x;
	INDEX t = tPos.y * lab->width + tPos.x;
	
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
			node->prev = MAX_NODES;
			/** on definit sa distance de 's' à '+oo' */
			node->f_cost = INF_WEIGHT;
			node->cost = INF_WEIGHT;

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
	/** Tant que la recherche doit s'affiner,
	    et qu'il y a des sommets a visité, on les visite */
	while (!shouldPrint && !pqueue_is_empty(visitQueue)) {
		/** 2.1. : on cherche un noeud 'u' non visite minimisant d(u).
		  ceci est optimisé à l'aide d'une file de priorité */
		t_pqueue_node node = pqueue_pop(visitQueue);
		/** l'indice de 'u' */
		INDEX u = *((INDEX *)node.value);

		/** le sommet n'est plus dans la file */
		pqueue_nodes[u] = NULL;

		/** si on a atteint une autre partie connexe ... */
		if (nodes[u].f_cost == INF_WEIGHT) {
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
			/** si c'est un mur, ou une porte fermée */
			if (c == LAB_WALL || (c == LAB_DOOR && !hasKey)) {
				/** on passe à la case suivante */
				continue ;
			}
		
			/** sinon, on est sur une case vide, ou sur la clef */
			
			/** index de 'v' */
			INDEX v = vy * lab->width + vx;
			/** on teste ce nouveau chemin */
			astar_test_path(u, v, t, 1);
		}
		/** si c'est un teleporteur, alors on essaye le chemin
		    vers l'autre teleporteur */
		BYTE tpID = lab_get_tpID(lab->map[nodes[u].pos.y][nodes[u].pos.x]);
		if (tpID != MAX_TP) {
			/** on recupere les 2 cases du teleporteurs */
			t_pos * tp = lab->tps[tpID];
			INDEX tp0ID = tp[0].y * lab->width + tp[0].x;
			INDEX tp1ID = tp[1].y * lab->width + tp[1].x;
			INDEX v;
			/** si 'u' correspond à la 1ere case */
			if (u == tp0ID) {
				/** alors 'v' est la 2ème */
				v = tp1ID;
			} else {
			/** sinon, 'u' est la 2ème et 'v' la 1ère */
				v = tp0ID;
			}
			/** on teste ce nouveau chemin */
			astar_test_path(u, v, t, 0);
		}
	}
	
	/** libere la mémoire */
	pqueue_delete(visitQueue);
	free(pqueue_nodes);

	/** on notifie le pere qu'on a fini */
	sendPacket(PACKET_ID_ENDED, INF_WEIGHT);

	/** si le processus a calculé tous les chemins,
	    mais n'a pas recu de demande d'affichage, alors
	    on attends un signal du père,
	    savoir si le chemin doit être affiché ou non */
	while (!shouldPrint);
	
	/** affiche le resultat */
	if (shouldPrint) {
		lab_print_path(lab, nodes, s, t);
	}
	/**  libère la mémoire */
	free(nodes);
	lab_delete(lab);
	/** fin du processus */
	exit(EXIT_SUCCESS);
	return (client);
}
