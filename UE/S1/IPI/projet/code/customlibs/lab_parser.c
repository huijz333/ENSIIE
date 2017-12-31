# include "lab.h"

/**
 *	Fonction interne
 *
 *	@require : le labyrinthe, le tableau des sommets du graphe,
 *			le tableau faisant la bijection entre les sommets
 *			du graphe et la position (x, y) sur la carte,
 *			et le tableau des teleporteurs.
 *	@ensure  : lit la carte sur l'entrée standart, et initialise
 *			les structures passés en parametre
 *	@assign  :	- le tableau 'nodes' est rempli des sommets
 *			- le tableau 'nodesID' est rempli par l'indice des sommets
 *			nodesID[y * l + x] = indice du sommet à la position (x, y)
 *			- le tableau 'tps' est rempli par l'indice des sommets.
 *			La 1ère dimension est l'indice du teleporteur (char, voir LAB_TP)
 *			et la 2ème enregistre l'indice dans le graphe des
 *			2 sommets du teleporteur.
 *
 *	NB: L'indice MAX_NODES indique que le sommet n'existe pas.
 */
static void lab_parse_map(t_lab * lab, t_array * nodes,
				INDEX * nodesID, INDEX tps[MAX_TP][2]) {
	/** initialise les teleporteurs */
	INDEX i;
	for (i = 0 ; i < MAX_TP ; i++) {
		tps[i][0] = MAX_NODES;
		tps[i][1] = MAX_NODES;
	}

	/** on n'a besoin que de 1 ligne à la fois pour lire la carte */
	/** on lit toute la carte du labyrinthe */
	char * line = (char *) malloc((lab->l + 1) * sizeof(char));
	char * tp;

	/** pour chaque case de la carte */
	INDEX x, y;
	for (y = 0 ; y < lab->l ; y++) {
		if (!scanf("%s\n", line)) {
			fprintf(stderr, "Error reading map l:"INDEX_IDENTIFIER"\n", y);
			exit(EXIT_FAILURE);
		}
		for (x = 0 ; x < lab->l ; x++) {
			/** on recupere la case de la carte qui correspond */
			int caseID = y * lab->l + x;
			char c = line[x];

			/** on crée un nouveau sommet */
			t_nodel node;
			node.x = x;
			node.y = y;
			node.super.ws = NULL;
			node.super.super.successors = NULL;
			node.id = c;

			switch(c) {
				/** si on est sur un mur, pas de sommet */
				case LAB_WALL:
					nodesID[caseID] = MAX_NODES;
					continue ;

				/** si on est sur une case vide, on ajoute le sommet */
				case LAB_EMPTY:
					nodesID[caseID] = array_add(nodes, &node);
					continue ;
				/** si on est sur l'entrée, on définit l'entrée 's' */
				case LAB_ENTRY:
					nodesID[caseID] = array_add(nodes, &node);
					lab->entry = nodesID[caseID];
					continue ;
				/** si on est sur la sortie, on définit la sortie 't' */
				case LAB_EXIT:
					nodesID[caseID] = array_add(nodes, &node);
					lab->exit = nodesID[caseID];
					break ;
				/** si on est sur la clef, on l'enregistre */
				case LAB_KEY:
					nodesID[caseID] = array_add(nodes, &node);
					lab->key = nodesID[caseID];
					break ;
				/** si on est sur la porte, on l'enregistre */
				case LAB_DOOR:
					nodesID[caseID] = array_add(nodes, &node);
					lab->door = nodesID[caseID];
					break ;
				default:
				/** sinon */
					/** si on est sur un teleporteur */
					tp = strchr(LAB_TP, c);
					if (tp != NULL) {
						nodesID[caseID] = array_add(nodes, &node);
						int tpID = (int)(tp - LAB_TP);
						if (tps[tpID][0] == MAX_NODES) {
							tps[tpID][0] = nodesID[caseID];
						} else {
							tps[tpID][1] = nodesID[caseID];
						}
					} else {
					/** si on est sur une case inconnue */
						fprintf(stderr,	"unknown character : %c,"\
								"assumed '.'\n", c);
						nodesID[caseID] = array_add(nodes, &node);
					}


					break ;
			}

		}
	}
	/** libere la mémoire */
	free(line);
}

/**
 *	Fonction interne
 *
 *	@require : le labyrinthe, le tableau des sommets du graphe,
 *			le tableau faisant la bijection entre les sommets
 *			du graphe et la position (x, y) sur la carte,
 *			et le tableau des teleporteurs.
 *	@ensure  :	- crée des arcs entre les sommets representant des cases vides.
 *			- lie les teleporteurs entre eux par des arcs de poids nulles
 *			- ne lie pas la porte aux autres sommets, mais l'index
 *			de ses voisins sont enregistrés pour la suite
 *	@assign  : les attributs 'successors' et 'ws' des sommets
 *			du graphe sont initialisés.
 *
 *	NB: L'indice MAX_NODES indique que le sommet n'existe pas.
 */
static void lab_link_nodes(t_lab * lab, t_array * nodes,
		INDEX * nodesID, INDEX tps[MAX_TP][2]) {
	/** on a 'n' sommets non-mur */
	INDEX n = nodes->size;
	/** pour chaque sommets, on crée des arcs avec les cases voisines vides */
	INDEX uNodeID;
	for (uNodeID = 0 ; uNodeID < n ; uNodeID++) {
		/** si le sommet est la porte, on ne crée pas d'arc, car
		  on ne sait pas enore si on ira chercher la clef */
		if (uNodeID == lab->door) {
			continue ;
		}

		t_nodel * u = (t_nodel *) array_get(nodes, uNodeID);
		/** pour chaque cases voisines */
		BYTE i;
		for (i = 0 ; i < MAX_DIRECTIONS ; i++) {
			t_direction d = DIRECTIONS[i];
			int nx = (int)u->x + d.x;
			int ny = (int)u->y + d.y;
			/** si on est hors de la carte, on continue */
			if (nx < 0 || ny < 0 || nx >= (int)lab->l || ny >= (int)lab->l) {
				continue ;
			}
			/** on recupere le voisin */
			INDEX vCaseID = ny * lab->l + nx;
			INDEX vNodeID = nodesID[vCaseID];

			/** si le voisin est un mur, ou une porte,
			  on passe au voisin suivant */
			if (vNodeID == MAX_NODES || vNodeID == lab->door) {
				continue ;
			}

			/** sinon, 'v' est voisin 'u' */
			if (u->super.ws == NULL) {
				u->super.ws = array_new(4, sizeof(WEIGHT));
				u->super.super.successors = array_new(4, sizeof(INDEX));
			}
			WEIGHT w = 1;
			array_add(u->super.ws, &w);
			array_add(u->super.super.successors, &vNodeID);
		}
	}


	/** on enregistre les sommets voisins de la porte */
	/** s'il y a une porte */
	if (lab->door != MAX_NODES) {
		t_nodel * door = (t_nodel *) array_get(nodes, lab->door);
		BYTE i;
		for (i = 0 ; i < MAX_DIRECTIONS ; i++) {
			t_direction d = DIRECTIONS[i];
			int nx = (int)door->x + d.x;
			int ny = (int)door->y + d.y;
			/** si on est hors de la carte */
			if (nx < 0 || ny < 0 || nx >= (int)lab->l || ny >= (int)lab->l) {
				/** pas de voisins dans cette direction */
				lab->vdoor[i] = MAX_NODES;
				continue ;
			}
			/** on recupere le voisin */
			INDEX vCaseID = ny * lab->l + nx;
			INDEX vNodeID = nodesID[vCaseID];
			lab->vdoor[i] = vNodeID;
		}
	}
	/** pour chaque teleporteurs */
	INDEX i;
	for (i = 0 ; i < MAX_TP ; i++) {
		/** on regarde s'il est présent dans le graphe */
		INDEX tp1ID = tps[i][0];
		INDEX tp2ID = tps[i][1];
		if (tp1ID == MAX_NODES || tp2ID == MAX_NODES) {
			continue ;
		}

		/** si oui, on le recupere, et on lie les 2 sommets par un arc */
		t_nodew * tp1 = (t_nodew *) array_get(nodes, tp1ID);
		t_nodew * tp2 = (t_nodew *) array_get(nodes, tp2ID);

		if (tp1->ws == NULL) {
			tp1->ws = array_new(4, sizeof(WEIGHT));
			tp1->super.successors = array_new(4, sizeof(INDEX));
		}

		if (tp2->ws == NULL) {
			tp2->ws = array_new(4, sizeof(WEIGHT));
			tp2->super.successors = array_new(4, sizeof(INDEX));
		}

		/** ce sommet est de poids '0' car l'action 'TP' prends
		  0 seconde à s'executer */
		WEIGHT w = 0;
		array_add(tp1->ws, &w);
		array_add(tp1->super.successors, &tp2ID);

		array_add(tp2->ws, &w);
		array_add(tp2->super.successors, &tp1ID);
	}
}



/**
 *	@require :	un flux, contenant des données supposées bien formatté
 *	@ensure	 :	alloue et lit un nouveau labyrinthe sur le flux donnée
 *	@assign  :	-------
 */
t_lab * lab_parse(void) {

	/** on alloue en mémoire le labyrinthe */
	INDEX l;
	if (!scanf(INDEX_IDENTIFIER "\n", &l)) {
		return (NULL);
	}
	t_lab * lab = lab_new(l);
	if (lab == NULL) {
		return (NULL);
	}
	/** les sommets du graphe que l'on va construire */
	t_array * nodes = lab->nodes;
	
	/** tableau faisant le lien entre index(x, y) d'une case
	    de la carte et de l'index du sommet dans le graphe */
	INDEX * nodesID = (INDEX *) malloc(sizeof(INDEX) * lab->l * lab->l);
	if (nodesID == NULL) {
		lab_delete(lab);
		return (NULL);
	}

	/** les teleporteurs */
	INDEX tps[MAX_TP][2];
	
	/** on lit la carte et on crée les sommets du graphe */
	lab_parse_map(lab, nodes, nodesID, tps);
	
	/** on lit les sommets par des arcs */
	lab_link_nodes(lab, nodes, nodesID, tps);

	/** libere la mémoire */
	free(nodesID);
	return (lab);
}

