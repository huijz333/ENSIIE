# include "lab.h"

/** definition des téléporteurs */
char LAB_TP[MAX_TP] = "*%$#&+-@^!£";

/** définition des directions possibles de deplacement. */
t_direction DIRECTIONS[MAX_DIRECTIONS] = {
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
	lab->key = MAX_NODES;
	lab->door = MAX_NODES;
	lab->entry = MAX_NODES;
	lab->exit = MAX_NODES;
	return (lab);
}
