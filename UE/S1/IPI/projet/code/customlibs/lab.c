# include "lab.h"

/** definition des téléporteurs */
wchar_t LAB_TP[MAX_TP] = L"*%$#&+-@^!£";

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
 *	@require : un caractère 'c' de la chaine 'LAB_TP'
 *	@ensure  : renvoie l'id 'i' dans le tableau t_lab.tps[i] correspondant
 *		   au teleporteur du caractère 'c', ou 'MAX_TP' si le teleporteur
 *		   n'existe pas.
 *	@assign  : --------------
 */
BYTE lab_get_tpID(wchar_t c) {
	switch (c) {
		case '*':
			return (0);
		case '%':
			return (1);
		case '$':
			return (2);
		case '#':
			return (3);
		case '&':
			return (4);
		case '+':
			return (5);
		case '-':
			return (6);
		case '@':
			return (7);
		case '^':
			return (8);
		case '!':
			return (9);
		case L'£':
			return (10);
		default:
			return (MAX_TP);
	};
	return (MAX_TP);
}

/**
 *	@require :	lab : un labyrinthe alloué via 'lab_new()'
 *	@ensure	 :	supprime le labyrinthe du tas
 *	@assign  :	-------
 */
void lab_delete(t_lab * lab) {
	INDEX y;
	for (y = 0 ; y < lab->height ; y++) {
		free(lab->map[y]);
	}
	free(lab->map);
	free(lab);
}

/**
 *	@require :	n : longueur (== largeur) du labyrinthe
 *	@ensure	 :	crée un nouveau labyrinthe de taille 'n'
 *	@assign  :	-------
 */
t_lab * lab_new(int l) {
	t_lab * lab = (t_lab *) malloc(sizeof(t_lab));
	if (lab == NULL) {
		return (NULL);
	}
	lab->map = (wchar_t **) malloc(sizeof(wchar_t *) * l);
	if (lab->map == NULL) {
		lab_delete(lab);
		return (NULL);
	}
	int y;
	for (y = 0 ; y < l ; y++) {
		lab->map[y] = (wchar_t *) malloc(sizeof(wchar_t) * (l + 1));
		if (lab->map[y] == NULL) {
			lab_delete(lab);
			return (NULL);
		}
	}
	lab->width = l;
	lab->height = l;

	t_pos out = {MAX_NODES, MAX_NODES};
	lab->key = out;
	lab->door = out;
	lab->entry = out;
	lab->exit = out;
	BYTE i;
	for (i = 0 ; i < MAX_TP ; i++) {
		lab->tps[i][0] = out;
		lab->tps[i][1] = out;
	}
	return (lab);
}
