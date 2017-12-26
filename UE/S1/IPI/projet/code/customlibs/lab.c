# include "lab.h"

/**
 *	@require :	lab : un labyrinthe alloué via 'lab_new()'
 *	@ensure	 :	supprime le labyrinthe du tas
 *	@assign  :	-------
 */
void lab_delete(t_lab * lab) {
	free(lab->map);
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
	lab->map = (char *) malloc((l * l + 1) * sizeof(char));
	if (lab->teleporters == NULL || lab->doors == NULL || lab->map == NULL) {
		lab_delete(lab);
		return (NULL);
	}
	lab->map[l * l] = 0;
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

	INDEX i;
	for (i = 0 ; i < l ; i++) {
		fscanf(stream, "%s\n", lab->map + i * l);
	}
	return (lab);
}

/**
 *	@require :	un labyrinthe et les coordonnées d'une case
 *	@ensure	 :	renvoie la lettre dans la case
 *	@assign  :	--------------------
 */
char lab_char_at(t_lab * lab, INDEX x, INDEX y) {
	return (lab->map[y * lab->l + x]);
}

WEIGHT h(t_array * nodes, INDEX uID, INDEX vID, INDEX sID, INDEX tID) {
	

	return 0;//-(dx * dx + dy * dy);
}

void lab_solve(t_lab * lab, unsigned int timer) {

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

	/** on crée un sommet pour chaque case non-mur du labyrinthe.
	 On a donc au plus 'l * l' case maximum, on reduira le taille du tableau après*/
	t_array * nodes = array_new(lab->l * lab->l, sizeof(t_nodel));
	if (nodes == NULL) {
		return ;
	}

	/** pour chaque case */
	INDEX maxNodeID = lab->l * lab->l;
	INDEX * nodesID = (INDEX *) malloc(sizeof(INDEX) * maxNodeID);
	INDEX x, y;
	for (y = 0 ; y < lab->l ; y++) {
		for (x = 0 ; x < lab->l ; x++) {
			/** on recupere le sommet qui correspond */
			INDEX caseID = y * lab->l + x;
			if (lab->map[caseID] == LAB_CHAR_WALL) {
				nodesID[caseID] = maxNodeID;
			} else {
				t_nodel node;
				node.x = x;
				node.y = y;
				node.super.ws = NULL;
				node.super.super.successors = NULL;
				nodesID[caseID] = array_add(nodes, &node);
			}
		}
	}
	/** on a 'n' sommets non-mur */
	INDEX n = nodes->size;
	/** pour chaque sommets */
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
				u->super.ws = array_new(4, sizeof(unsigned int));
				u->super.super.successors = array_new(4, sizeof(INDEX));
			}
			unsigned int w = 1;
			array_add(u->super.ws, &w);
			array_add(u->super.super.successors, &vNodeID);
		}
	}

	free(nodesID);
	int r = astar(nodes, h, 0, n - 1);
	t_array * path = node_build_path(nodes, 0, n - 1);
	if (path == NULL) {
		array_delete(nodes);
		return ;
	}

	INDEX i;
	t_nodel * prev = array_get(nodes, *((INDEX *) array_get(path, 0)));
	for (i = 1 ; i < path->size ; i++) {
		INDEX nextID = *((INDEX *) array_get(path, i));
		t_nodel * next = (t_nodel *) array_get(nodes, nextID);
		int dx = (int)next->x - (int)prev->x;
		int dy = (int)next->y - (int)prev->y;
		if (dx == 1) {
			puts("DROITE");
		} else if (dx == -1) {
			puts("GAUCHE");
		} else if (dy == 1) {
			puts("BAS");
		} else if (dy == -1) {
			puts("HAUT");
		} else {
			printf("wtf: %d:%d ; %d:%d ; %d:%d\n", next->x, next->y, prev->x, prev->y, dx, dy);
		}
		prev = next;
	}

	/* libere la mémoire */
	array_delete(path);
	for (i = 0 ; i < nodes->size ; i++) {
		t_nodew * node = (t_nodew *) array_get(nodes, i);
		array_delete(node->ws);
		array_delete(node->super.successors);
	}
	array_delete(nodes);
	(void)timer;
}
