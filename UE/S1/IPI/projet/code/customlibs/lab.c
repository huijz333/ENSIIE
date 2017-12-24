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
 
	/**

	Algorithme:

	- On cherche le plus court chemin en supposant que l'on possède déjà toutes les clefs
	- On enregistre les portes que l'on a du traverser
		1 clef, 2 clef... x clefs.
		=> on crée une bitmap et on résout à l'aide de l'exo 1
		=> s'il existe et s'il est suffisement court, fin

	- Sinon, on
	*/


	typedef struct	s_direction {
		char const	* name;
		int		offx;
		int		offy;
	}		t_direction;

	static t_direction directions[4] = {
		{"DROITE",	 1,  0},
		{"GAUCHE",	-1,  0},
		{"HAUT",	 0,  1},
		{"BAS",		 0, -1}
	};


	(void)lab;
	(void)timer;
}
