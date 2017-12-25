# include "lab.h"

/**
 *	NB : ce constructeur est relativement long, car on s'assure
 *		qu'il n'y ait pas de fuite mémoire en cas d'erreur
 *
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
	if (lab->teleporters == NULL) {
		free(lab);
		return (NULL);
	}
	lab->doors = hmap_new('z' - 'a' + 1, (t_hashf)inthash, (t_cmpf)intcmp);
	if (lab->doors == NULL) {
		hmap_delete(lab->doors);
		free(lab);
		return (NULL);
	}
	lab->map = (char *) malloc((l * l + 1) * sizeof(char));
	if (lab->map == NULL) {
		hmap_delete(lab->doors);
		hmap_delete(lab->teleporters);
		free(lab);
		return (NULL);
	}

	lab->map[l * l] = 0;
	return (lab);
}

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

	/** on crée un sommet pour chaque case non-mur du labyrinthe */
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
				continue ;
			}
			t_nodel node;
			node.x = x;
			node.y = y;
			nodesID[caseID] = array_add(nodes, &node);
		}
	}
	/** on a 'n' sommets non-mur */
	INDEX n = nodes->size;
	
	/** on crée les arcs entre ces sommets */
	t_bitmap * arcs = bitmap_new2(n, n);
	if (arcs == NULL) {
		array_delete(nodes);
		return ;
	}

	/** pour chaque sommets */
	ARRAY_ITERATE_START(nodes, t_nodel *, node, nodeID) {
		/** pour chaque voisins */
		INDEX x = node->x;
		INDEX y = node->y;
		BYTE i;
		for (i = 0 ; i < 4 ; i++) {
			int dx = directions[i].dx;
			int dy = directions[i].dy;
			int nx = (int)x + dx;
			int ny = (int)y + dy;
			if (nx < 0 || ny < 0 || nx >= (int)lab->l || ny >= (int)lab->l) {
				continue ;
			}
			INDEX neighborCaseID = ny * lab->l + nx;
			INDEX neighborNodeID = nodesID[neighborCaseID];
			if (neighborNodeID == maxNodeID) {
				continue ;
			}
			/** sinon, on définit des arcs */
			bitmap_set2(arcs, nodeID, neighborNodeID, n);
			bitmap_set2(arcs, neighborNodeID, nodeID, n);
		}
	};
	ARRAY_ITERATE_STOP(nodes, t_nodel *, node, nodeID);


	free(nodesID);

	breadth_search(nodes, arcs, 0, n - 1);
	t_array * path = node_build_path(nodes, 0, n - 1);
	if (path == NULL) {
		array_delete(nodes);
		bitmap_delete(arcs);
		return ;
	}

	t_nodel * prev = array_get(nodes, *((INDEX *) array_get(path, 0)));
	INDEX i;
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
		} else {
			puts("HAUT");
		}
		prev = next;
	}
	array_delete(path);
	array_delete(nodes);
	bitmap_delete(arcs);
	(void)timer;
}
