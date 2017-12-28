# include "lab.h"

/** definition des téléporteurs */
char LAB_TP[MAX_TP] = "*%$#&+-@^!£";

/** définition des directions possibles de deplacement. */
t_direction DIRECTIONS[DIRECTIONS_COUNT] = {
	{"DROITE",	 1,  0},
	{"GAUCHE",	-1,  0},
	{"HAUT",	 0, -1},
	{"BAS",		 0,  1}
/*	{"BAS/GAUCHE",	-1,  1},
	{"BAS/DROITE",	 1,  1},
	{"HAUT/GAUCHE",	-1, -1},
	{"HAUT/DROITE",	 1, -1}*/
};


/**
 *	@require :	lab : un labyrinthe alloué via 'lab_new()'
 *	@ensure	 :	supprime le labyrinthe du tas
 *	@assign  :	-------
 */
void lab_delete(t_lab * lab) {
	INDEX i;
	for (i = 0 ; i < lab->nodes->size ; i++) {
		t_nodew * node = (t_nodew *) array_get(lab->nodes, i);
		array_delete(node->ws);
		array_delete(node->super.successors);
	}
	array_delete(lab->nodes);
	free(lab);
}

/**
 *	@require :	n : longueur (== largeur) du labyrinthe
 *	@ensure	 :	crée un nouveau labyrinthe de taille 'n'
 *	@assign  :	-------
 */
t_lab * lab_new(INDEX l) {
	t_lab * lab = (t_lab *) malloc(sizeof(t_lab));
	if (lab == NULL) {
		return (NULL);
	}
	lab->l = l;
	lab->nodes = array_new(lab->l * lab->l, sizeof(t_nodel));
	if (lab->nodes == NULL) {
		lab_delete(lab);
		return (NULL);
	}
	return (lab);
}

/**
 *	@require :	un flux, contenant des données supposées bien formatté
 *	@ensure	 :	alloue et lit un nouveau labyrinthe sur le flux donnée
 *	@assign  :	-------
 */
t_lab * lab_parse(FILE * stream) {

	/** on alloue en mémoire le labyrinthe */
	INDEX l;
	fscanf(stream, INDEX_IDENTIFIER "\n", &l);
	t_lab * lab = lab_new(l);
	if (lab == NULL) {
		return (NULL);
	}

	/** on lit toute la carte du labyrinthe */
	char * map = (char *) malloc((l * l + 1) * sizeof(char));
	INDEX i;
	for (i = 0 ; i < l ; i++) {
		fscanf(stream, "%s\n", map + i * l);
	}

	/** les sommets du graphe que l'on va construire */
	t_array * nodes = lab->nodes;
	
	/** tableau faisant le lien entre index(x, y) d'une case
	    de la carte et de l'index du sommet dans le graphe */
	INDEX * nodesID = (INDEX *) malloc(sizeof(INDEX) * lab->l * lab->l);

	/** initialise les teleporteurs */
	INDEX tps[MAX_TP][2];
	for (i = 0 ; i < MAX_TP ; i++) {
		tps[i][0] = MAX_NODES;
		tps[i][1] = MAX_NODES;
	}

	/** pour chaque case de la carte */
	INDEX x, y;
	for (y = 0 ; y < lab->l ; y++) {
		for (x = 0 ; x < lab->l ; x++) {
			/** on crée un nouveau sommet */
			t_nodel node;
			node.x = x;
			node.y = y;
			node.super.ws = NULL;
			node.super.super.successors = NULL;

			/** on recupere la case de la carte qui correspond */
			INDEX caseID = y * lab->l + x;
			char c = map[caseID];
			switch(c) { 
				/** si on est sur une case vide, on ajoute le sommet */
				case LAB_EMPTY:
					nodesID[caseID] = array_add(nodes, &node);
					continue ;
				/** si on est sur un mur, pas de sommet */
				case LAB_WALL:
					nodesID[caseID] = MAX_NODES;
					continue ;
				/** si on est sur l'entrée, on définit l'entrée 's' */
				case LAB_ENTRY:
					nodesID[caseID] = array_add(nodes, &node);
					lab->entry = nodesID[caseID];
					//TODO
					continue ;
				/** si on est sur la sortie, on définit la sortie 't' */
				case LAB_EXIT:
					nodesID[caseID] = array_add(nodes, &node);
					lab->exit = nodesID[caseID];
					//TODO
					continue ;
				/** si on est sur la clef, on l'enregistre */
				case LAB_KEY:
					nodesID[caseID] = array_add(nodes, &node);
					lab->key = nodesID[caseID];
					continue ;
				/** si on est sur la porte, on l'enregistre */
				case LAB_DOOR:
					nodesID[caseID] = array_add(nodes, &node);
					lab->door = nodesID[caseID];
					continue ;
				default:
					break ;
			}
			
			/** si on est sur un teleporteur */
			char * tp = strchr(LAB_TP, c);
			if (tp != NULL) {
				nodesID[caseID] = array_add(nodes, &node);
				int tpID = (int)(tp - LAB_TP);
				if (tps[tpID][0] == MAX_NODES) {
					tps[tpID][0] = nodesID[caseID];
				} else {
					tps[tpID][1] = nodesID[caseID];
				}
			}
		}
	}
	/** on a 'n' sommets non-mur */
	INDEX n = nodes->size;
	/** pour chaque sommets, on crée des arcs avec les cases voisines vides */
	INDEX uNodeID;
	for (uNodeID = 0 ; uNodeID < n ; uNodeID++) {
		t_nodel * u = (t_nodel *) array_get(nodes, uNodeID);
		/** pour chaque cases voisines */
		BYTE i;
		for (i = 0 ; i < DIRECTIONS_COUNT ; i++) {
			t_direction d = DIRECTIONS[i];
			int nx = (int)u->x + d.x;
			int ny = (int)u->y + d.y;
			/** si on est hors de la carte, on continue */
			if (nx < 0 || ny < 0 || nx >= (int)lab->l || ny >= (int)lab->l) {
				continue ;
			}
			/** si on est dans un mur, on continue */
			INDEX vCaseID = ny * lab->l + nx;
			INDEX vNodeID = nodesID[vCaseID];
			if (vNodeID == MAX_NODES) {
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

	/** pour chaque teleporteurs */
	for (i = 0 ; i < MAX_TP ; i++) {
		/** on regarde s'il est présent dans le graphe */
		INDEX tp1ID = tps[i][0];
		INDEX tp2ID = tps[i][1];
		if (tp1ID == MAX_NODES || tp2ID == MAX_NODES) {
			continue ;
		}

		/** si oui, on le recupere, et on les lie par un arc */
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

		WEIGHT w = 0;
		array_add(tp1->ws, &w);
		array_add(tp1->super.successors, &tp2ID);

		array_add(tp2->ws, &w);
		array_add(tp2->super.successors, &tp1ID);
	}

	free(nodesID);
	free(map);
	return (lab);
}


/** fonction heuristic pour l'algorithme A* */
/** https://fr.wikipedia.org/wiki/Distance_de_Manhattan */
static WEIGHT heuristic_manhattan(t_array * nodes,
		INDEX uID, INDEX vID, INDEX sID, INDEX tID) {
	(void)uID;
	(void)sID;
	t_nodel * v = (t_nodel *) array_get(nodes, vID);
	t_nodel * t = (t_nodel *) array_get(nodes, tID);	
	return (t->x - v->x + t->y - v->y);
}

/**
 *	@require : le labyrinthe, et un chemin
 *	@ensure  : affiche les directions successives du chemin
 *	@assign  : --------------
 */
static void lab_print_path(t_array * nodes, t_array * path) {
	INDEX i;
	t_nodel * prev = array_get(nodes, *((INDEX *) array_get(path, 0)));
	for (i = 1 ; i < path->size ; i++) {
		BYTE found = 0;
		INDEX nextID = *((INDEX *) array_get(path, i));
		t_nodel * next = (t_nodel *) array_get(nodes, nextID);
		int dx = (int)next->x - (int)prev->x;
		int dy = (int)next->y - (int)prev->y;
		int j;
		for (j = 0 ; j < DIRECTIONS_COUNT ; j++) {
			t_direction d = DIRECTIONS[j];
			if (dx == d.x && dy == d.y) {
				printf("%s\n", d.name);
				found = 1;
				break ;
			}
		}
		if (!found) {
			printf("TP\n");
		}
		prev = next;
	}
}

static int lab_try_solve(t_array * nodes, INDEX s, INDEX t, unsigned int timer) {
	/** on essaye de résoudre avec A* et une heuristique de distance */
	int r = astar(nodes, heuristic_manhattan, s, t);
	r = dijkstra(nodes, s, t);
	/** si la resolution a echoué, on essaye de resoudre avec djikstra */
	if (r == 0) {
		r = dijkstra(nodes, s, t);
		/** si dijkstra echoué, pas de solution */
		if (r == 0) {
			return (0);
		}
	}

	/** on verifie que le chemin est assez court */
	WEIGHT pathtime = ((t_nodew *)array_get(nodes, t))->pathw;
	if (pathtime > timer) {
		return (0);
	}

	/** on construit le chemin */
	t_array * path = node_build_path(nodes, s, t);
	if (path == NULL) {
		array_delete(path);
		return (0);
	}
	
	lab_print_path(nodes, path);
	array_delete(path);

	return (1);
}

/**
 *	@require : un labyrinthe valide (renvoyé via 'lab_parse()'), et un temps 'timer'
 *	@ensure  : tente de resoudre le labyrinthe en moins de 'timer' opérations.
 *			Renvoie 1 en cas de succes, 0 sinon
 *	@assign  : les sommets du graphe representant le labyrinthe peuvent
 *			être modifié par l'algorithme a*
 */
int lab_solve(t_lab * lab, unsigned int timer) {
	/** les sommets du graphe du labyrinthe */
	t_array * nodes = lab->nodes;
	INDEX s = lab->entry;	/** indice du sommet de départ (== 0)   */
	INDEX t = lab->exit;	/** indice du sommet de sortie (== n-1) */

	/** on essaye de le resoudre en considerant les portes comme des murs */
	if (lab_try_solve(nodes, s, t, timer)) {
		/** si succès fin */
		return (1);
	}

	/** sinon, on essaye de resoudre en allant chercher la clef d'abord */






	/** si meme en allant chercher les clefs, la resolution a echoué... */
	printf("Not connected\n");
	return (0);
}
