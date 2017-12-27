# include "lab.h"

/** une direction possible de deplacement */
typedef struct	s_direction {
	char const	* name;
	int		dx;
	int		dy;
}		t_direction;

static t_direction directions[4] = {
	{"DROITE",	 1,  0},
	{"GAUCHE",	-1,  0},
	{"HAUT",	 0,  1},
	{"BAS",		 0, -1}
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
	hmap_delete(lab->teleporters); /*TODO : free keys and values */
	hmap_delete(lab->doors);
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
	lab->teleporters = hmap_new(16, (t_hashf)inthash, (t_cmpf)intcmp);
	lab->doors = hmap_new('z' - 'a' + 1, (t_hashf)inthash, (t_cmpf)intcmp);
	lab->nodes = array_new(lab->l * lab->l, sizeof(t_nodel));
	if (lab->teleporters == NULL || lab->doors == NULL || lab->nodes == NULL) {
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

	/** les sommets du graphe */
	t_array * nodes = lab->nodes;
	
	/** tableau faisant le lien entre index(x, y) dans la carte
	    et index dans le graphe du sommet */
	INDEX maxNodeID = lab->l * lab->l;
	INDEX * nodesID = (INDEX *) malloc(sizeof(INDEX) * maxNodeID);
	size_t max_tp = strlen(LAB_CHARSET_TELEPORTERS);
	INDEX teleporters[max_tp][2];
	for (i = 0 ; i < max_tp ; i++) {
		teleporters[i][0] = maxNodeID;
		teleporters[i][1] = maxNodeID;
	}

	/** pour chaque case */
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
			/** si on est sur un mur, pas de sommets a ajouté dans le graphe */
			if (c == LAB_CHAR_WALL) {
				nodesID[caseID] = maxNodeID;
			} else if (c == LAB_CHAR_EMPTY || c == LAB_CHAR_ENTRY
					|| c == LAB_CHAR_EXIT) {
			/** sinon si on est sur une case vide , on ajoute le sommet */
				nodesID[caseID] = array_add(nodes, &node);
			} else {
			/** sinon, on regarde si on est sur un teleporteur */
				char * tp = strchr(LAB_CHARSET_TELEPORTERS, c);
				/** si oui */
				if (tp != NULL) {
					/** on l'ajoute aux teleporteurs trouvés */
					/* et on ajoute un sommet */
					nodesID[caseID] = array_add(nodes, &node);
					int tpID = (int)(tp - LAB_CHARSET_TELEPORTERS);
					if (teleporters[tpID][0] == maxNodeID) {
						teleporters[tpID][0] = nodesID[caseID];
					} else {
						teleporters[tpID][1] = nodesID[caseID];
					}
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
		for (i = 0 ; i < 4 ; i++) {
			int dx = directions[i].dx;
			int dy = directions[i].dy;
			int nx = (int)u->x + dx;
			int ny = (int)u->y + dy;
			/** si on est hors de la carte, on continue */
			if (nx < 0 || ny < 0 || nx >= (int)lab->l || ny >= (int)lab->l) {
				continue ;
			}
			/** si on est dans un mur, on continue */
			INDEX vCaseID = ny * lab->l + nx;
			INDEX vNodeID = nodesID[vCaseID];
			if (vNodeID == maxNodeID) {
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
	for (i = 0 ; i < max_tp ; i++) {
		INDEX tp1ID = teleporters[i][0];
		INDEX tp2ID = teleporters[i][1];
		if (tp1ID == maxNodeID || tp2ID == maxNodeID) {
			continue ;
		}
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

		WEIGHT w = 1;
		array_add(tp1->ws, &w);
		array_add(tp1->super.successors, &tp2ID);

		array_add(tp2->ws, &w);
		array_add(tp2->super.successors, &tp1ID);
	}

	free(nodesID);
	free(map);
	return (lab);
}

void lab_solve(t_lab * lab, unsigned int timer) {
	t_array * nodes = lab->nodes;
	INDEX n = nodes->size;
	INDEX s = 0;
	INDEX t = n - 1;

	int r = astar(nodes, heuristic_euclidian, s, t);
/*	int r = astar(nodes, heuristic_zero, s, t); */
	if (r == 0) {
		puts("Not connected");
		return ;
	}
	
	t_array * path = node_build_path(nodes, s, t);
	if (path == NULL) {
		return ;
	}

	INDEX i;
	t_nodel * prev = array_get(nodes, *((INDEX *) array_get(path, 0)));
	for (i = 1 ; i < path->size ; i++) {
		INDEX nextID = *((INDEX *) array_get(path, i));
		t_nodel * next = (t_nodel *) array_get(nodes, nextID);
		int dx = (int)next->x - (int)prev->x;
		int dy = (int)next->y - (int)prev->y;
		if (dx == 1 && dy == 0) {
			puts("DROITE");
		} else if (dx == -1 && dy == 0) {
			puts("GAUCHE");
		} else if (dx == 0 && dy == 1) {
			puts("BAS");
		} else if (dx == 0 && dy == -1) {
			puts("HAUT");
		} else {
			puts("TP");
		}
		prev = next;
	}

	/* libere la mémoire */
	array_delete(path);
	(void)timer;
}
